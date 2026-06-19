#ifndef RSU_COMMON_H
#define RSU_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* ========================= Project constants ========================= */
#define N_VEHICLES          6U
#define RSU_QUEUE_LEN       16U
#define TRAFFIC_TICK_MS     200U   /* 1 simulated traffic tick = 200 ms demo time */
#define AGING_K             3U     /* Fairness gain: effectivePriority = base - k*age */

/* ========================= Enums and data types ========================= */
typedef enum {
    ORIGIN_EAST  = 0,
    ORIGIN_WEST  = 1,
    ORIGIN_NORTH = 2,
    ORIGIN_SOUTH = 3
} Origin_t;

typedef enum {
    TURN_STRAIGHT = 0,
    TURN_RIGHT    = 1,
    TURN_LEFT     = 2
} Turn_t;

typedef enum {
    MODE_MM1 = 0,   /* one server: only one vehicle can cross */
    MODE_MM2 = 1    /* two servers: two non-conflicting vehicles can cross */
} MMMode_t;

typedef enum {
    MSG_REQUEST = 1,
    MSG_LEAVE   = 2
} MsgType_t;

typedef struct {
    uint8_t  id;
    Origin_t origin;
    Turn_t   turn;
    uint8_t  emergency;
    uint16_t period_ticks;
    uint16_t cross_ticks;
    const char *name;
} VehicleSpec_t;

typedef struct {
    MsgType_t type;
    uint8_t   vehicle_id;
    TickType_t tick;
} RsuMessage_t;

typedef struct {
    uint8_t vehicle_id;
    uint8_t grant;
} GrantMessage_t;

typedef struct {
    bool     waiting[N_VEHICLES];
    bool     crossing[N_VEHICLES];
    uint8_t  crossing_count;
    uint16_t age[N_VEHICLES];
    MMMode_t mode;
} RsuState_t;

/* ========================= Global objects ========================= */
extern const VehicleSpec_t g_vehicleSpec[N_VEHICLES];
extern QueueHandle_t g_reqQueue;
extern QueueHandle_t g_leaveQueue;
extern QueueHandle_t g_grantQueue[N_VEHICLES];
extern SemaphoreHandle_t g_stateMutex;
extern RsuState_t g_rsuState;
extern MMMode_t g_mmMode;

/* ========================= Task creation ========================= */
void rsu_create_tasks(void);
void vehicle_create_tasks(void);
void vRSUTask(void *pvParameters);
void vVehicleTask(void *pvParameters);

/* ========================= Hardware interface =========================
 * In Week 5 software demo this file is a C stub of the VHDL coprocessor.
 * Later, replace hw_interface.c with the real memory-mapped register driver.
 */
void hw_init(void);
void hw_write_state(const VehicleSpec_t specs[N_VEHICLES],
                    const bool crossing[N_VEHICLES],
                    uint8_t crossing_count,
                    MMMode_t mode);
bool hw_safe_to_grant(uint8_t candidate);
bool hw_no_collision(void);
uint8_t hw_conflicts_with_crossing_vec(void);
bool hw_collision_alarm(void);
void hw_clear_alarm(void);

/* ========================= Utility / policy helpers ========================= */
int16_t priority_value(uint8_t i);
int16_t effective_priority(uint8_t i, uint16_t age);
const char *origin_to_str(Origin_t o);
const char *turn_to_str(Turn_t t);

#endif /* RSU_COMMON_H */
