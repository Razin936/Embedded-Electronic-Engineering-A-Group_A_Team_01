#include <stdio.h>
#include "rsu_common.h"

/* -------------------------------------------------------------------------
 * Vehicle tasks V0..V5
 *
 * UPPAAL Vehicle FSM mapping:
 *   Idle / Outside      -> wait until next period
 *   WaitingInQueue      -> send V2I request and wait for I2V grant
 *   V2V_Enter / Crossing-> crossing delay
 *   V2V_Leave           -> send leave/done message to RSU
 * ------------------------------------------------------------------------- */

static TickType_t traffic_delay(uint16_t traffic_ticks)
{
    return pdMS_TO_TICKS((uint32_t)traffic_ticks * TRAFFIC_TICK_MS);
}

void vVehicleTask(void *pvParameters)
{
    const VehicleSpec_t *spec = (const VehicleSpec_t *)pvParameters;
    RsuMessage_t request;
    RsuMessage_t leave;
    GrantMessage_t grant;

    configASSERT(spec != NULL);
    configASSERT(spec->id < N_VEHICLES);

    /* Spread first releases slightly so the log is readable. */
    vTaskDelay(pdMS_TO_TICKS(50U * spec->id));

    for (;;) {
        /* Periodic arrival of vehicle request. */
        vTaskDelay(traffic_delay(spec->period_ticks));

        request.type = MSG_REQUEST;
        request.vehicle_id = spec->id;
        request.tick = xTaskGetTickCount();

        printf("[VEH %u] request: %-28s origin=%s turn=%s emergency=%u\n",
               spec->id,
               spec->name,
               origin_to_str(spec->origin),
               turn_to_str(spec->turn),
               (unsigned)spec->emergency);

        (void)xQueueSend(g_reqQueue, &request, portMAX_DELAY);

        /* Wait until RSU grants this vehicle. */
        if (xQueueReceive(g_grantQueue[spec->id], &grant, portMAX_DELAY) == pdTRUE) {
            if (grant.grant != 0U) {
                printf("[VEH %u] V2V_ENTER -> crossing for %u traffic ticks\n",
                       spec->id, (unsigned)spec->cross_ticks);

                vTaskDelay(traffic_delay(spec->cross_ticks));

                leave.type = MSG_LEAVE;
                leave.vehicle_id = spec->id;
                leave.tick = xTaskGetTickCount();

                printf("[VEH %u] V2V_LEAVE -> done\n", spec->id);
                (void)xQueueSend(g_leaveQueue, &leave, portMAX_DELAY);
            }
        }
    }
}

void vehicle_create_tasks(void)
{
    static char names[N_VEHICLES][8];

    for (uint8_t i = 0; i < N_VEHICLES; i++) {
        (void)snprintf(names[i], sizeof(names[i]), "Veh%u", (unsigned)i);

        BaseType_t ok = xTaskCreate(vVehicleTask,
                                    names[i],
                                    configMINIMAL_STACK_SIZE + 256,
                                    (void *)&g_vehicleSpec[i],
                                    tskIDLE_PRIORITY + 1,
                                    NULL);
        configASSERT(ok == pdPASS);
    }
}
