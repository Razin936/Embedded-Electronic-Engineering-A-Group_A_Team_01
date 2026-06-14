/* =====================================================================
 * Embedded Electronic Engineering Lab A  (HSHL, SS 2026)
 * Cross-Traffic Intersection Management for Autonomous Vehicles
 * ---------------------------------------------------------------------
 * WEEK 3 - FIRST ABSTRACT IMPLEMENTATION
 *
 * This file is a well-defined, one-to-one mapping of the UPPAAL timed
 * automata model (RSU_MM1_MM2_priority_emergency_intersection) into
 * abstract C core logic. It is intentionally single-threaded and
 * deterministic. The RTOS tasking (FreeRTOS) and the HW partitioning
 * (ModelSim) are introduced later in Weeks 4 and 5.
 *
 *   Main street : EAST - WEST        Side street : NORTH - SOUTH
 *   RSU controls the intersection (V2I request / I2V grant).
 *   M/M/1 : exactly one vehicle crosses at a time.
 *   M/M/2 : two NON-conflicting vehicles may cross simultaneously.
 *   Priority : 1.Emergency  2.Main street  3.Side street
 *              4.Right turn before Left turn inside a conflict.
 *   Communication : V2I (req/emg_req), I2V (grant/yield), V2V (enter/leave).
 *   Continuous arrival : after leaving, a vehicle re-requests after its period.
 *
 * Build : gcc -std=c11 -Wall -O2 intersection_rsu.c -o intersection_rsu
 * Run   : ./intersection_rsu mm1      (M/M/1 capacity)
 *         ./intersection_rsu mm2      (M/M/2 capacity, default)
 * ===================================================================== */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* ====================================================================
 * 1. GLOBAL DECLARATIONS  (mapped from UPPAAL <declaration>)
 * ==================================================================== */

#define N 6                 /* number of vehicles                       */

/* M/M/1 vs M/M/2 server capacity (set at run time, see main) */
enum { MM1 = 1, MM2 = 2 };
static int MM_MODE = MM2;

/* Directions (UPPAAL constants) */
enum { EAST = 0, WEST = 1, NORTH = 2, SOUTH = 3 };

/* Turn types (UPPAAL constants) */
enum { RIGHT = 0, STRAIGHT = 1, LEFT = 2 };

/* Vehicle finite-state-machine locations (UPPAAL Vehicle template) */
typedef enum {
    LOC_IDLE = 0,   /* Idle           : waiting for its arrival period   */
    LOC_WAIT,       /* WaitingInQueue : request sent, waiting for grant  */
    LOC_ENTER,      /* V2V_Enter      : committed, announces entering    */
    LOC_CROSS,      /* Crossing       : occupying the intersection       */
    LOC_LEAVE       /* V2V_Leave      : committed, announces leaving      */
} loc_t;

/* Per-vehicle static configuration (UPPAAL template parameters) */
typedef struct {
    int  id;
    int  origin_cfg;   /* o        */
    int  turn_cfg;     /* t        */
    int  emergency_cfg;/* e        */
    int  period;       /* arrival period                                  */
    int  crossTime;    /* crossing duration                               */
} vehicle_cfg_t;

/* Queue and crossing state (UPPAAL global arrays) */
static bool waiting[N];
static bool crossing[N];

static int  origin[N];
static int  turn[N];
static int  emergency[N];

static int  crossingCount     = 0;
static bool emergencyNotified  = false;

/* Local clock x[i] and current FSM location loc[i] (UPPAAL clock/location) */
static loc_t loc[N];
static int   x[N];

/* ====================================================================
 * 2. HELPER FUNCTIONS  (verbatim mapping of UPPAAL helper functions)
 * ==================================================================== */

static bool isMainRoad(int i)
{
    return origin[i] == EAST || origin[i] == WEST;
}

static bool oppositeDirection(int a, int b)
{
    if (origin[a] == EAST  && origin[b] == WEST)  return true;
    if (origin[a] == WEST  && origin[b] == EAST)  return true;
    if (origin[a] == NORTH && origin[b] == SOUTH) return true;
    if (origin[a] == SOUTH && origin[b] == NORTH) return true;
    return false;
}

static int turnRank(int i)
{
    if (turn[i] == RIGHT)    return 0;
    if (turn[i] == STRAIGHT) return 1;
    return 2;
}

static int priorityValue(int i)
{
    /* Emergency has the highest priority. */
    if (emergency[i] == 1) return 0;

    /* Main street has higher priority than side street. */
    if (isMainRoad(i)) return 10 + turnRank(i);

    return 20 + turnRank(i);
}

static bool betterPriority(int a, int b)
{
    int pa = priorityValue(a);
    int pb = priorityValue(b);

    if (pa < pb) return true;
    if (pa > pb) return false;

    /* Tie breaker: lower ID first (keeps the order deterministic). */
    return a < b;
}

/* --------------------------------------------------------------------
 * Conflict checking  (UPPAAL conflict / conflictsWithCrossing / noCollision)
 * ------------------------------------------------------------------ */

static bool conflict(int a, int b)
{
    if (a == b) return false;

    /* Emergency vehicle gets exclusive crossing permission. */
    if (emergency[a] == 1 || emergency[b] == 1) return true;

    /* Same origin cannot enter together. */
    if (origin[a] == origin[b]) return true;

    /* Opposite-direction vehicles can sometimes pass together. */
    if (oppositeDirection(a, b)) {

        /* Both straight from opposite sides are allowed together. */
        if (turn[a] == STRAIGHT && turn[b] == STRAIGHT) return false;

        /* Both right turns from opposite sides are allowed together. */
        if (turn[a] == RIGHT && turn[b] == RIGHT) return false;

        /* Right versus Left from opposite sides is NOT allowed together. */
        if (turn[a] == RIGHT && turn[b] == LEFT) return true;
        if (turn[a] == LEFT  && turn[b] == RIGHT) return true;

        /* Any remaining left-turn combination is blocked for safety. */
        if (turn[a] == LEFT || turn[b] == LEFT) return true;

        return false;
    }

    /* Perpendicular movements are considered conflicting. */
    return true;
}

static bool conflictsWithCrossing(int i)
{
    for (int k = 0; k < N; k++)
        if (crossing[k] && conflict(i, k)) return true;
    return false;
}

static bool noCollision(void)
{
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
            if (crossing[i] && crossing[j] && conflict(i, j)) return false;
    return true;
}

/* --------------------------------------------------------------------
 * Queue and grant logic  (UPPAAL emergencyWaiting .. canGrant)
 * ------------------------------------------------------------------ */

static bool emergencyWaiting(void)
{
    for (int k = 0; k < N; k++)
        if (waiting[k] && emergency[k] == 1) return true;
    return false;
}

static bool capacityAvailableFor(int i)
{
    if (MM_MODE == MM1) return crossingCount == 0;

    /* M/M/2 mode */
    if (crossingCount >= 2) return false;
    return !conflictsWithCrossing(i);
}

static bool existsBetterGrantable(int i)
{
    for (int k = 0; k < N; k++)
        if (waiting[k] && k != i && betterPriority(k, i))
            if (capacityAvailableFor(k)) return true;
    return false;
}

static bool canGrant(int i)
{
    if (!waiting[i]) return false;

    /* If an emergency is waiting, no normal vehicle gets a new grant. */
    if (emergencyWaiting() && emergency[i] == 0) return false;

    if (!capacityAvailableFor(i))   return false;
    if (existsBetterGrantable(i))   return false;

    return true;
}

/* --------------------------------------------------------------------
 * State update functions  (UPPAAL requestVehicle / grantVehicle / finishVehicle)
 * ------------------------------------------------------------------ */

static void requestVehicle(int i, int o, int t, int e)
{
    origin[i]    = o;
    turn[i]      = t;
    emergency[i] = e;
    waiting[i]   = true;
}

static void grantVehicle(int i)
{
    waiting[i]    = false;
    crossing[i]   = true;
    crossingCount = crossingCount + 1;
    if (emergency[i] == 1) emergencyNotified = false;
}

static void finishVehicle(int i)
{
    crossing[i] = false;
    if (crossingCount > 0) crossingCount = crossingCount - 1;
}

/* ====================================================================
 * 3. SYSTEM CONFIGURATION  (mapped from UPPAAL <system>)
 * --------------------------------------------------------------------
 *  Vehicle(id, origin, turn, emergency, arrival_period, crossing_time)
 *
 *  V0 = emergency vehicle on main street
 *  V1 / V2 = main street straight vehicles (M/M/2 non-conflict test)
 *  V3 = side street straight
 *  V4 = North right turn   (critical pair)
 *  V5 = South left turn    (critical pair, must not cross with V4)
 * ================================================================== */

static const vehicle_cfg_t cfg[N] = {
    /* id, origin, turn,     emg, period, crossTime */
    {  0,  EAST,   STRAIGHT, 1,   30,     4 },   /* V0 emergency        */
    {  1,  EAST,   STRAIGHT, 0,    6,     5 },   /* V1 main street      */
    {  2,  WEST,   STRAIGHT, 0,    8,     5 },   /* V2 main opposite    */
    {  3,  NORTH,  STRAIGHT, 0,   10,     5 },   /* V3 side street      */
    {  4,  NORTH,  RIGHT,    0,   12,     5 },   /* V4 critical right   */
    {  5,  SOUTH,  LEFT,     0,   12,     5 },   /* V5 critical left    */
};

static const char *dir_str(int d)
{
    switch (d) { case EAST: return "EAST"; case WEST: return "WEST";
                 case NORTH: return "NORTH"; default: return "SOUTH"; }
}
static const char *turn_str(int t)
{
    switch (t) { case RIGHT: return "RIGHT"; case STRAIGHT: return "STRAIGHT";
                 default: return "LEFT"; }
}

/* ====================================================================
 * 4. RUNTIME SAFETY CHECKS  (mirror of the UPPAAL verification queries)
 * --------------------------------------------------------------------
 *  Q2  A[] noCollision()
 *  Q3  A[] crossingCount <= MM_MODE
 *  Q4  A[] not (crossing[4] && crossing[5])
 * ================================================================== */

static void safety_checks(int t)
{
    if (!noCollision()) {
        fprintf(stderr, "[t=%3d] SAFETY VIOLATION: collision detected!\n", t);
        assert(noCollision());
    }
    if (crossingCount > MM_MODE) {
        fprintf(stderr, "[t=%3d] SAFETY VIOLATION: capacity %d exceeds %d\n",
                t, crossingCount, MM_MODE);
        assert(crossingCount <= MM_MODE);
    }
    if (crossing[4] && crossing[5]) {
        fprintf(stderr, "[t=%3d] SAFETY VIOLATION: V4(N-right) and V5(S-left) "
                "cross together!\n", t);
        assert(!(crossing[4] && crossing[5]));
    }
}

/* ====================================================================
 * 5. ABSTRACT SCHEDULER / SIMULATION LOOP
 * --------------------------------------------------------------------
 * One discrete time unit corresponds to one tick of the UPPAAL clocks.
 * Committed locations (V2V_Enter, V2V_Leave) are processed within the
 * same tick (zero time), exactly as in the UPPAAL semantics.
 * ================================================================== */

#define T_MAX 80

/* Statistics */
static int  total_requests = 0;
static int  total_grants   = 0;
static int  max_concurrent = 0;
static int  emergency_grants = 0;
static int  pair_cross_events = 0;   /* ticks with 2 vehicles crossing */

static void rsu_grant_phase(int t)
{
    /* The RSU grants vehicles in strict priority order until no further
     * grantable vehicle remains. canGrant() already enforces capacity,
     * conflict and "no better candidate", so this loop reproduces the
     * non-deterministic UPPAAL select-and-grant deterministically.       */
    bool granted;
    do {
        granted  = false;
        int best = -1;
        for (int i = 0; i < N; i++)
            if (canGrant(i) && (best < 0 || betterPriority(i, best)))
                best = i;

        if (best >= 0) {
            grantVehicle(best);                 /* I2V grant + state update */
            total_grants++;
            if (emergency[best] == 1) emergency_grants++;
            printf("[t=%3d]  RSU GRANT   -> V%d  (%s %s%s)  cross=%d\n",
                   t, best, dir_str(origin[best]), turn_str(turn[best]),
                   emergency[best] ? " EMERGENCY" : "", crossingCount);

            /* Committed V2V_Enter : announce entering, move to Crossing. */
            printf("[t=%3d]    V%d V2V_ENTER (broadcast)\n", t, best);
            loc[best] = LOC_CROSS;
            x[best]   = 0;
            granted   = true;
        }
    } while (granted);
}

static void run_simulation(void)
{
    /* Initial reset (UPPAAL initial location = Idle, clocks = 0). */
    memset(waiting,   0, sizeof waiting);
    memset(crossing,  0, sizeof crossing);
    crossingCount    = 0;
    emergencyNotified = false;
    for (int i = 0; i < N; i++) {
        origin[i]    = cfg[i].origin_cfg;
        turn[i]      = cfg[i].turn_cfg;
        emergency[i] = cfg[i].emergency_cfg;
        loc[i]       = LOC_IDLE;
        x[i]         = 0;
    }

    printf("=====================================================\n");
    printf(" Abstract RSU intersection simulation  (mode = M/M/%d)\n", MM_MODE);
    printf("=====================================================\n");

    for (int t = 0; t <= T_MAX; t++) {

        /* ---- (a) Idle -> WaitingInQueue : continuous arrival ------- */
        for (int i = 0; i < N; i++) {
            if (loc[i] == LOC_IDLE && x[i] >= cfg[i].period) {
                requestVehicle(i, cfg[i].origin_cfg, cfg[i].turn_cfg,
                               cfg[i].emergency_cfg);
                if (cfg[i].emergency_cfg == 1) {
                    emergencyNotified = false;          /* emg_req path  */
                    printf("[t=%3d]  V%d EMG_REQ  (%s %s) -> RSU\n", t, i,
                           dir_str(origin[i]), turn_str(turn[i]));
                } else {
                    printf("[t=%3d]  V%d REQ      (%s %s) -> RSU\n", t, i,
                           dir_str(origin[i]), turn_str(turn[i]));
                }
                total_requests++;
                loc[i] = LOC_WAIT;
                x[i]   = 0;
            }
        }

        /* ---- (b) RSU : emergency I2V yield broadcast --------------- */
        if (emergencyWaiting() && !emergencyNotified) {
            emergencyNotified = true;
            printf("[t=%3d]  RSU I2V_YIELD broadcast (emergency approaching)\n", t);
        }

        /* ---- (c) RSU grant phase : WaitingInQueue -> Crossing ------ */
        rsu_grant_phase(t);

        /* ---- (d) Crossing -> V2V_Leave -> Idle --------------------- */
        for (int i = 0; i < N; i++) {
            if (loc[i] == LOC_CROSS && x[i] >= cfg[i].crossTime) {
                /* Committed V2V_Leave : announce leaving. */
                printf("[t=%3d]    V%d V2V_LEAVE (broadcast)\n", t, i);
                finishVehicle(i);                     /* leave + update  */
                printf("[t=%3d]  V%d DONE      -> Idle             cross=%d\n",
                       t, i, crossingCount);
                loc[i] = LOC_IDLE;
                x[i]   = 0;
            }
        }

        /* ---- (e) Safety checks + statistics ----------------------- */
        safety_checks(t);
        if (crossingCount > max_concurrent) max_concurrent = crossingCount;
        if (crossingCount == 2)             pair_cross_events++;

        /* ---- (f) Advance all local clocks by one tick ------------- */
        for (int i = 0; i < N; i++) x[i]++;
    }

    printf("-----------------------------------------------------\n");
    printf(" Summary (M/M/%d, horizon %d ticks)\n", MM_MODE, T_MAX);
    printf("   total requests        : %d\n", total_requests);
    printf("   total grants          : %d\n", total_grants);
    printf("   emergency grants      : %d\n", emergency_grants);
    printf("   max concurrent crossings : %d (capacity %d)\n",
           max_concurrent, MM_MODE);
    printf("   ticks with 2 crossing : %d\n", pair_cross_events);
    printf("   safety checks         : PASSED (no collision, capacity ok,\n");
    printf("                           V4/V5 never simultaneous)\n");
    printf("=====================================================\n");
}

/* ==================================================================== */

int main(int argc, char **argv)
{
    if (argc >= 2 && strcmp(argv[1], "mm1") == 0) MM_MODE = MM1;
    else if (argc >= 2 && strcmp(argv[1], "mm2") == 0) MM_MODE = MM2;

    run_simulation();
    return 0;
}
