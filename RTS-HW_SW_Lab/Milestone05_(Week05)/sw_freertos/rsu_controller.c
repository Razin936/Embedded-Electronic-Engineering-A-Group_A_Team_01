#include <stdio.h>
#include "rsu_common.h"

/* -------------------------------------------------------------------------
 * RSU Controller Task
 *
 * SW partition responsibilities:
 *   - receive V2I requests
 *   - receive V2V/I2V leave/done messages
 *   - keep waiting[], crossing[], crossingCount, age[]
 *   - apply emergency > main > side priority
 *   - apply aging fairness to avoid V5 starvation
 *   - call hw_safe_to_grant(i) instead of doing conflict loops in software
 * ------------------------------------------------------------------------- */

static bool is_main(uint8_t i)
{
    return (g_vehicleSpec[i].origin == ORIGIN_EAST) ||
           (g_vehicleSpec[i].origin == ORIGIN_WEST);
}

const char *origin_to_str(Origin_t o)
{
    switch (o) {
    case ORIGIN_EAST:  return "EAST";
    case ORIGIN_WEST:  return "WEST";
    case ORIGIN_NORTH: return "NORTH";
    case ORIGIN_SOUTH: return "SOUTH";
    default:           return "UNKNOWN";
    }
}

const char *turn_to_str(Turn_t t)
{
    switch (t) {
    case TURN_STRAIGHT: return "STRAIGHT";
    case TURN_RIGHT:    return "RIGHT";
    case TURN_LEFT:     return "LEFT";
    default:            return "UNKNOWN";
    }
}

/* Lower value means higher priority. This matches the Week 4 explanation:
 * emergency first, then main street, then side street, right before straight
 * before left inside side traffic.
 */
int16_t priority_value(uint8_t i)
{
    const VehicleSpec_t *v = &g_vehicleSpec[i];

    if (v->emergency != 0U) {
        return 0;                 /* V0 emergency */
    }

    if (is_main(i)) {
        return 11;                /* V1, V2 main street */
    }

    if (v->turn == TURN_RIGHT) {
        return 20;                /* V4 */
    }

    if (v->turn == TURN_STRAIGHT) {
        return 21;                /* V3 */
    }

    return 22;                    /* V5 left turn, lowest static priority */
}

int16_t effective_priority(uint8_t i, uint16_t age)
{
    return (int16_t)(priority_value(i) - (int16_t)(AGING_K * age));
}

static bool emergency_waiting(void)
{
    for (uint8_t i = 0; i < N_VEHICLES; i++) {
        if (g_rsuState.waiting[i] && (g_vehicleSpec[i].emergency != 0U)) {
            return true;
        }
    }
    return false;
}

static bool better_priority(uint8_t a, uint8_t b)
{
    const int16_t pa = effective_priority(a, g_rsuState.age[a]);
    const int16_t pb = effective_priority(b, g_rsuState.age[b]);

    if (pa < pb) {
        return true;
    }
    if (pa > pb) {
        return false;
    }

    /* deterministic tie-break */
    return a < b;
}

static bool emergency_rule_allows(uint8_t i)
{
    if (emergency_waiting() && (g_vehicleSpec[i].emergency == 0U)) {
        return false;
    }
    return true;
}

static bool exists_better_grantable(uint8_t candidate)
{
    for (uint8_t j = 0; j < N_VEHICLES; j++) {
        if (j == candidate) {
            continue;
        }
        if (!g_rsuState.waiting[j]) {
            continue;
        }
        if (!emergency_rule_allows(j)) {
            continue;
        }
        if (!hw_safe_to_grant(j)) {
            continue;
        }
        if (better_priority(j, candidate)) {
            return true;
        }
    }
    return false;
}

static bool can_grant(uint8_t i)
{
    if (!g_rsuState.waiting[i]) {
        return false;
    }

    /* SW: emergency hold/yield policy */
    if (!emergency_rule_allows(i)) {
        return false;
    }

    /* HW: capacity + conflict check */
    if (!hw_safe_to_grant(i)) {
        return false;
    }

    /* SW: priority + fairness policy */
    if (exists_better_grantable(i)) {
        return false;
    }

    return true;
}

static void mirror_state_to_hw(void)
{
    hw_write_state(g_vehicleSpec,
                   g_rsuState.crossing,
                   g_rsuState.crossing_count,
                   g_rsuState.mode);
}

static void assert_safety(void)
{
    const uint8_t max_servers = (g_rsuState.mode == MODE_MM2) ? 2U : 1U;

    configASSERT(g_rsuState.crossing_count <= max_servers);
    configASSERT(!(g_rsuState.crossing[4] && g_rsuState.crossing[5]));
    configASSERT(hw_no_collision());
    configASSERT(!hw_collision_alarm());
}

static void handle_request(const RsuMessage_t *msg)
{
    const uint8_t i = msg->vehicle_id;

    if (i >= N_VEHICLES) {
        return;
    }

    if (!g_rsuState.waiting[i] && !g_rsuState.crossing[i]) {
        g_rsuState.waiting[i] = true;
        g_rsuState.age[i] = 0;
        printf("[RSU] queued V%u  basePrio=%d\n", i, (int)priority_value(i));
    }
}

static void handle_leave(const RsuMessage_t *msg)
{
    const uint8_t i = msg->vehicle_id;

    if (i >= N_VEHICLES) {
        return;
    }

    if (g_rsuState.crossing[i]) {
        g_rsuState.crossing[i] = false;
        if (g_rsuState.crossing_count > 0U) {
            g_rsuState.crossing_count--;
        }
        printf("[RSU] V%u finished. crossingCount=%u\n",
               i, (unsigned)g_rsuState.crossing_count);
    }
}

static void drain_input_queues(void)
{
    RsuMessage_t msg;

    while (xQueueReceive(g_reqQueue, &msg, 0) == pdTRUE) {
        if (msg.type == MSG_REQUEST) {
            handle_request(&msg);
        }
    }

    while (xQueueReceive(g_leaveQueue, &msg, 0) == pdTRUE) {
        if (msg.type == MSG_LEAVE) {
            handle_leave(&msg);
        }
    }
}

static void update_aging(void)
{
    for (uint8_t i = 0; i < N_VEHICLES; i++) {
        if (g_rsuState.waiting[i]) {
            if (g_rsuState.age[i] < 1000U) {
                g_rsuState.age[i]++;
            }
        }
    }
}

static void grant_vehicle(uint8_t i)
{
    GrantMessage_t grant;

    g_rsuState.waiting[i] = false;
    g_rsuState.crossing[i] = true;
    g_rsuState.crossing_count++;
    g_rsuState.age[i] = 0;

    grant.vehicle_id = i;
    grant.grant = 1U;

    printf("[RSU] GRANT V%u  effPrio=%d  crossingCount=%u  CWC=0x%02X\n",
           i,
           (int)effective_priority(i, g_rsuState.age[i]),
           (unsigned)g_rsuState.crossing_count,
           (unsigned)hw_conflicts_with_crossing_vec());

    (void)xQueueSend(g_grantQueue[i], &grant, 0);
}

static void schedule_one_or_more(void)
{
    bool granted;
    const uint8_t max_servers = (g_rsuState.mode == MODE_MM2) ? 2U : 1U;

    do {
        granted = false;
        mirror_state_to_hw();

        for (uint8_t i = 0; i < N_VEHICLES; i++) {
            if (can_grant(i)) {
                grant_vehicle(i);
                mirror_state_to_hw();
                assert_safety();
                granted = true;
                break;  /* re-scan after state change */
            }
        }
    } while (granted && (g_rsuState.crossing_count < max_servers));
}

void vRSUTask(void *pvParameters)
{
    (void)pvParameters;

    printf("[RSU] controller started\n");

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(TRAFFIC_TICK_MS));

        xSemaphoreTake(g_stateMutex, portMAX_DELAY);

        drain_input_queues();
        update_aging();

        if (emergency_waiting()) {
            printf("[RSU] I2V_YIELD broadcast: emergency waiting, no new normal grants\n");
        }

        mirror_state_to_hw();
        assert_safety();
        schedule_one_or_more();
        assert_safety();

        xSemaphoreGive(g_stateMutex);
    }
}

void rsu_create_tasks(void)
{
    BaseType_t ok = xTaskCreate(vRSUTask,
                                "RSU",
                                configMINIMAL_STACK_SIZE + 512,
                                NULL,
                                tskIDLE_PRIORITY + 3,
                                NULL);
    configASSERT(ok == pdPASS);
}
