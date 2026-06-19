#include <string.h>
#include "rsu_common.h"

/* -------------------------------------------------------------------------
 * hw_interface.c
 *
 * Week 5 software-side driver/stub for the Conflict & Safety Coprocessor.
 *
 * In the final HW/SW codesign, the VHDL block in ModelSim computes the same
 * outputs through memory-mapped registers:
 *   STATE_VEH[i], MODE, CAND, CTRL, ST_SAFE, ST_NOCOL, ST_CWC, ST_ALARM
 *
 * For the FreeRTOS software demo, this C file behaves like the hardware model.
 * Later you can replace only this file with a real AXI/register driver.
 * ------------------------------------------------------------------------- */

typedef struct {
    VehicleSpec_t specs[N_VEHICLES];
    bool crossing[N_VEHICLES];
    uint8_t crossing_count;
    MMMode_t mode;
    bool alarm_latched;
} HwModel_t;

static HwModel_t hw;

static bool is_main_road(Origin_t o)
{
    return (o == ORIGIN_EAST) || (o == ORIGIN_WEST);
}

static bool is_opposite(Origin_t a, Origin_t b)
{
    return ((a == ORIGIN_EAST)  && (b == ORIGIN_WEST))  ||
           ((a == ORIGIN_WEST)  && (b == ORIGIN_EAST))  ||
           ((a == ORIGIN_NORTH) && (b == ORIGIN_SOUTH)) ||
           ((a == ORIGIN_SOUTH) && (b == ORIGIN_NORTH));
}

static bool both_straight(const VehicleSpec_t *a, const VehicleSpec_t *b)
{
    return (a->turn == TURN_STRAIGHT) && (b->turn == TURN_STRAIGHT);
}

/* Direct C reference of the planned VHDL conflict(i,j) cell.
 * Rule set used for this project:
 *   1. Emergency vehicle gets exclusive crossing.
 *   2. Same origin conflicts.
 *   3. Main road opposite straight vehicles can overlap in M/M/2: V1 + V2.
 *   4. Side road opposite straight vehicles can also overlap.
 *   5. Main-vs-side or turning movements conflict by default.
 *   6. Critical regression: V4 NORTH-RIGHT and V5 SOUTH-LEFT conflict.
 */
static bool conflict_pair(uint8_t a, uint8_t b)
{
    const VehicleSpec_t *va = &hw.specs[a];
    const VehicleSpec_t *vb = &hw.specs[b];

    if (a == b) {
        return false;
    }

    if ((va->emergency != 0U) || (vb->emergency != 0U)) {
        return true;
    }

    if (va->origin == vb->origin) {
        return true;
    }

    /* Explicit critical conflict from the verified model. */
    if (((a == 4U) && (b == 5U)) || ((a == 5U) && (b == 4U))) {
        return true;
    }

    const bool a_main = is_main_road(va->origin);
    const bool b_main = is_main_road(vb->origin);

    if ((a_main == b_main) && is_opposite(va->origin, vb->origin) && both_straight(va, vb)) {
        return false;   /* non-conflicting opposite straight movement */
    }

    return true;
}

static bool conflicts_with_crossing(uint8_t candidate)
{
    for (uint8_t j = 0; j < N_VEHICLES; j++) {
        if (hw.crossing[j] && conflict_pair(candidate, j)) {
            return true;
        }
    }
    return false;
}

void hw_init(void)
{
    memset(&hw, 0, sizeof(hw));
}

void hw_write_state(const VehicleSpec_t specs[N_VEHICLES],
                    const bool crossing[N_VEHICLES],
                    uint8_t crossing_count,
                    MMMode_t mode)
{
    memcpy(hw.specs, specs, sizeof(hw.specs));
    memcpy(hw.crossing, crossing, sizeof(hw.crossing));
    hw.crossing_count = crossing_count;
    hw.mode = mode;

    if (!hw_no_collision()) {
        hw.alarm_latched = true;
    }
}

bool hw_safe_to_grant(uint8_t candidate)
{
    if (candidate >= N_VEHICLES) {
        return false;
    }

    if (hw.crossing[candidate]) {
        return false;
    }

    if (hw.mode == MODE_MM1) {
        return (hw.crossing_count == 0U);
    }

    /* M/M/2: capacity two, but only if candidate does not conflict. */
    if (hw.crossing_count >= 2U) {
        return false;
    }

    return !conflicts_with_crossing(candidate);
}

bool hw_no_collision(void)
{
    for (uint8_t i = 0; i < N_VEHICLES; i++) {
        if (!hw.crossing[i]) {
            continue;
        }
        for (uint8_t j = (uint8_t)(i + 1U); j < N_VEHICLES; j++) {
            if (hw.crossing[j] && conflict_pair(i, j)) {
                return false;
            }
        }
    }
    return true;
}

uint8_t hw_conflicts_with_crossing_vec(void)
{
    uint8_t vec = 0U;
    for (uint8_t i = 0; i < N_VEHICLES; i++) {
        if (conflicts_with_crossing(i)) {
            vec |= (uint8_t)(1U << i);
        }
    }
    return vec;
}

bool hw_collision_alarm(void)
{
    return hw.alarm_latched;
}

void hw_clear_alarm(void)
{
    hw.alarm_latched = false;
}
