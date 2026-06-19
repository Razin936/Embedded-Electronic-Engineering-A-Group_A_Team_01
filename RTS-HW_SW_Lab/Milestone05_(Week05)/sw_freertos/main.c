#include <stdio.h>
#include <string.h>
#include "rsu_common.h"

/* -------------------------------------------------------------------------
 * Week 5 FreeRTOS SW partition for the RSU intersection controller.
 *
 * This software keeps:
 *   - vehicle tasks V0..V5
 *   - RSU controller task
 *   - request / leave / grant queues
 *   - priority + emergency + aging policy
 *
 * The expensive conflict check is called through hw_interface.c.
 * For a real board, replace hw_interface.c with a register driver.
 * ------------------------------------------------------------------------- */

/* Verified fleet from the Week 4 proposal. */
const VehicleSpec_t g_vehicleSpec[N_VEHICLES] = {
    /* id, origin,        turn,          emergency, period, cross, name */
    {0, ORIGIN_EAST,  TURN_STRAIGHT, 1, 30, 4, "V0 emergency EAST straight"},
    {1, ORIGIN_EAST,  TURN_STRAIGHT, 0,  6, 5, "V1 main EAST straight"},
    {2, ORIGIN_WEST,  TURN_STRAIGHT, 0,  8, 5, "V2 main WEST straight"},
    {3, ORIGIN_NORTH, TURN_STRAIGHT, 0, 10, 5, "V3 side NORTH straight"},
    {4, ORIGIN_NORTH, TURN_RIGHT,    0, 12, 5, "V4 side NORTH right"},
    {5, ORIGIN_SOUTH, TURN_LEFT,     0, 12, 5, "V5 side SOUTH left"}
};

QueueHandle_t g_reqQueue = NULL;
QueueHandle_t g_leaveQueue = NULL;
QueueHandle_t g_grantQueue[N_VEHICLES] = { NULL };
SemaphoreHandle_t g_stateMutex = NULL;

RsuState_t g_rsuState;

/* Change this to MODE_MM1 when you want one-at-a-time service. */
MMMode_t g_mmMode = MODE_MM2;

static void create_kernel_objects(void)
{
    memset(&g_rsuState, 0, sizeof(g_rsuState));
    g_rsuState.mode = g_mmMode;

    g_reqQueue = xQueueCreate(RSU_QUEUE_LEN, sizeof(RsuMessage_t));
    g_leaveQueue = xQueueCreate(RSU_QUEUE_LEN, sizeof(RsuMessage_t));
    configASSERT(g_reqQueue != NULL);
    configASSERT(g_leaveQueue != NULL);

    for (uint8_t i = 0; i < N_VEHICLES; i++) {
        g_grantQueue[i] = xQueueCreate(1, sizeof(GrantMessage_t));
        configASSERT(g_grantQueue[i] != NULL);
    }

    g_stateMutex = xSemaphoreCreateMutex();
    configASSERT(g_stateMutex != NULL);
}

int main(void)
{
    printf("\n=== Week 5 FreeRTOS RSU Controller: SW partition ===\n");
    printf("Mode: %s\n", (g_mmMode == MODE_MM2) ? "M/M/2" : "M/M/1");
    printf("Traffic tick: %u ms\n\n", (unsigned)TRAFFIC_TICK_MS);

    create_kernel_objects();
    hw_init();

    rsu_create_tasks();
    vehicle_create_tasks();

    vTaskStartScheduler();

    /* Should never reach here unless heap is too small. */
    for (;;) {
    }
}

/* Optional hooks. Keep them simple for lab debugging. */
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    printf("ERROR: FreeRTOS malloc failed. Increase configTOTAL_HEAP_SIZE.\n");
    for (;;) {}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    taskDISABLE_INTERRUPTS();
    printf("ERROR: stack overflow in task %s\n", pcTaskName);
    for (;;) {}
}
