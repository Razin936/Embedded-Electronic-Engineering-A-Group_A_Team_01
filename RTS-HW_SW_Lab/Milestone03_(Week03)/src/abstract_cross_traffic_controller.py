"""
Milestone 03 / Week 03 - First Abstract Implementation
V2V/V2I Cross-Traffic Management Controller

This executable Python prototype maps the verified Week 02 UPPAAL model into
an abstract software implementation. It supports both queueing modes:

    M/M/1 style: server_cap = 1  -> one vehicle may cross at a time
    M/M/2 style: server_cap = 2  -> two compatible vehicles may cross together

The model includes:
- V2V beacon/intention messages
- V2I request/grant/complete messages
- RSU-based phase control
- Capacity checking through server_cap
- Route-conflict checking
- Deterministic logs for report and Git evidence

Run examples:
    python src/abstract_cross_traffic_controller.py --mode mm1
    python src/abstract_cross_traffic_controller.py --mode mm2
    python src/abstract_cross_traffic_controller.py --mode mm2 --output results/mm2_simulation_log.txt
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Dict, List, Optional, Tuple


class Phase(str, Enum):
    RIGHT_1 = "RIGHT_1"
    STRAIGHT_NS = "STRAIGHT_NS"
    RIGHT_2 = "RIGHT_2"
    STRAIGHT_EW = "STRAIGHT_EW"


class VehicleState(str, Enum):
    APPROACHING = "APPROACHING"
    BEACONED = "BEACONED"
    REQUESTED = "REQUESTED"
    WAITING = "WAITING"
    CROSSING = "CROSSING"
    DONE = "DONE"


@dataclass(frozen=True)
class VehiclePlan:
    vehicle_id: int
    side: str
    route: str
    route_group: str
    arrival_time: int


@dataclass
class VehicleRuntime:
    plan: VehiclePlan
    state: VehicleState = VehicleState.APPROACHING
    remaining_crossing_time: int = 0
    wait_time: int = 0
    granted_time: Optional[int] = None
    completed_time: Optional[int] = None


@dataclass
class SimulationMetrics:
    mode: str
    server_cap: int
    total_vehicles: int
    completed_vehicles: int
    total_wait_time: int
    max_wait_time: int
    total_simulation_time: int
    simultaneous_crossing_events: int
    conflict_violations: int

    @property
    def average_wait_time(self) -> float:
        if self.completed_vehicles == 0:
            return 0.0
        return self.total_wait_time / self.completed_vehicles


class RSUController:
    """Abstract RSU controller for the cross-traffic intersection."""

    PHASE_ORDER: Tuple[Phase, ...] = (
        Phase.RIGHT_1,
        Phase.STRAIGHT_NS,
        Phase.RIGHT_2,
        Phase.STRAIGHT_EW,
    )

    def __init__(self, server_cap: int, crossing_time: int = 2, phase_time: int = 4) -> None:
        if server_cap not in (1, 2):
            raise ValueError("server_cap must be 1 for M/M/1 or 2 for M/M/2")
        self.server_cap = server_cap
        self.crossing_time = crossing_time
        self.phase_time = phase_time
        self.phase_index = 0
        self.active: Dict[int, VehicleRuntime] = {}
        self.waiting_queue: List[VehicleRuntime] = []
        self.log: List[str] = []
        self.simultaneous_crossing_events = 0
        self.conflict_violations = 0

    @property
    def phase(self) -> Phase:
        return self.PHASE_ORDER[self.phase_index]

    def update_phase(self, time_step: int) -> None:
        if time_step > 0 and time_step % self.phase_time == 0:
            self.phase_index = (self.phase_index + 1) % len(self.PHASE_ORDER)
            self.log.append(f"t={time_step:02d} | PHASE | switched to {self.phase.value}")

    def receive_request(self, vehicle: VehicleRuntime, time_step: int) -> None:
        vehicle.state = VehicleState.WAITING
        if vehicle not in self.waiting_queue:
            self.waiting_queue.append(vehicle)
        self.log.append(
            f"t={time_step:02d} | V2I   | Vehicle{vehicle.plan.vehicle_id} -> RSU: "
            f"REQUEST(route={vehicle.plan.route}, group={vehicle.plan.route_group})"
        )

    def is_phase_allowed(self, vehicle: VehicleRuntime) -> bool:
        group = vehicle.plan.route_group
        if self.phase in (Phase.RIGHT_1, Phase.RIGHT_2):
            return group == "RIGHT"
        if self.phase == Phase.STRAIGHT_NS:
            return group == "STRAIGHT_NS"
        if self.phase == Phase.STRAIGHT_EW:
            return group == "STRAIGHT_EW"
        return False

    def routes_are_compatible(self, a: VehicleRuntime, b: VehicleRuntime) -> bool:
        """Compatibility rule aligned with the Week 02 UPPAAL abstraction."""
        if self.server_cap == 1:
            return False
        # In the verified M/M/2 abstraction, compatible right-turn vehicles may cross together.
        if a.plan.route_group == "RIGHT" and b.plan.route_group == "RIGHT":
            return True
        # Straight movements are handled conservatively in this first implementation.
        return False

    def no_conflict_with_active(self, vehicle: VehicleRuntime) -> bool:
        for active_vehicle in self.active.values():
            if not self.routes_are_compatible(vehicle, active_vehicle):
                return False
        return True

    def can_grant(self, vehicle: VehicleRuntime) -> Tuple[bool, str]:
        if len(self.active) >= self.server_cap:
            return False, "server capacity full"
        if not self.is_phase_allowed(vehicle):
            return False, f"phase {self.phase.value} does not allow {vehicle.plan.route_group}"
        if not self.no_conflict_with_active(vehicle):
            return False, "route conflict with active vehicle"
        return True, "grant allowed"

    def process_waiting_queue(self, time_step: int) -> None:
        for vehicle in list(self.waiting_queue):
            allowed, reason = self.can_grant(vehicle)
            if allowed:
                self.waiting_queue.remove(vehicle)
                self.grant(vehicle, time_step)
            else:
                vehicle.wait_time += 1
                self.log.append(
                    f"t={time_step:02d} | WAIT  | Vehicle{vehicle.plan.vehicle_id} waits: {reason}"
                )

    def grant(self, vehicle: VehicleRuntime, time_step: int) -> None:
        vehicle.state = VehicleState.CROSSING
        vehicle.remaining_crossing_time = self.crossing_time
        vehicle.granted_time = time_step
        self.active[vehicle.plan.vehicle_id] = vehicle
        self.log.append(
            f"t={time_step:02d} | V2I   | RSU -> Vehicle{vehicle.plan.vehicle_id}: "
            f"GRANT(activeCount={len(self.active)}/{self.server_cap}, phase={self.phase.value})"
        )

    def update_active_vehicles(self, time_step: int) -> None:
        if len(self.active) > 1:
            self.simultaneous_crossing_events += 1

        # Safety monitor: count conflicts if incompatible routes are simultaneously active.
        active_list = list(self.active.values())
        for i, first in enumerate(active_list):
            for second in active_list[i + 1 :]:
                if not self.routes_are_compatible(first, second):
                    self.conflict_violations += 1
                    self.log.append(
                        f"t={time_step:02d} | ERROR | Conflict violation: "
                        f"Vehicle{first.plan.vehicle_id} and Vehicle{second.plan.vehicle_id}"
                    )

        for vehicle in list(self.active.values()):
            vehicle.remaining_crossing_time -= 1
            self.log.append(
                f"t={time_step:02d} | MOVE  | Vehicle{vehicle.plan.vehicle_id} crossing "
                f"route={vehicle.plan.route}, remaining={vehicle.remaining_crossing_time}"
            )
            if vehicle.remaining_crossing_time <= 0:
                self.complete(vehicle, time_step)

    def complete(self, vehicle: VehicleRuntime, time_step: int) -> None:
        vehicle.state = VehicleState.DONE
        vehicle.completed_time = time_step
        self.active.pop(vehicle.plan.vehicle_id, None)
        self.log.append(
            f"t={time_step:02d} | V2I   | Vehicle{vehicle.plan.vehicle_id} -> RSU: "
            f"COMPLETE(activeCount={len(self.active)}/{self.server_cap})"
        )


class CrossTrafficSimulation:
    """Executable abstract implementation for Milestone 03."""

    def __init__(self, mode: str) -> None:
        mode = mode.lower()
        if mode not in ("mm1", "mm2"):
            raise ValueError("mode must be 'mm1' or 'mm2'")
        self.mode = mode
        self.server_cap = 1 if mode == "mm1" else 2
        self.rsu = RSUController(server_cap=self.server_cap)
        self.vehicles: List[VehicleRuntime] = self._build_default_vehicle_set()

    @staticmethod
    def _build_default_vehicle_set() -> List[VehicleRuntime]:
        # Vehicle 1 and Vehicle 2 represent compatible right-turn routes.
        # Vehicle 3 and Vehicle 4 represent conservative straight/conflicting routes.
        plans = [
            VehiclePlan(1, "N", "N2E", "RIGHT", 0),
            VehiclePlan(2, "E", "E2S", "RIGHT", 0),
            VehiclePlan(3, "N", "N2S", "STRAIGHT_NS", 2),
            VehiclePlan(4, "E", "E2W", "STRAIGHT_EW", 6),
        ]
        return [VehicleRuntime(plan=p) for p in plans]

    def send_beacon_and_request(self, vehicle: VehicleRuntime, time_step: int) -> None:
        vehicle.state = VehicleState.BEACONED
        self.rsu.log.append(
            f"t={time_step:02d} | V2V   | Vehicle{vehicle.plan.vehicle_id}: "
            f"BEACON(route={vehicle.plan.route}, side={vehicle.plan.side})"
        )
        vehicle.state = VehicleState.REQUESTED
        self.rsu.receive_request(vehicle, time_step)

    def all_done(self) -> bool:
        return all(v.state == VehicleState.DONE for v in self.vehicles)

    def run(self, max_steps: int = 20) -> Tuple[List[str], SimulationMetrics]:
        self.rsu.log.append("=" * 72)
        self.rsu.log.append(f"Milestone 03 abstract implementation run: mode={self.mode.upper()}")
        self.rsu.log.append(f"SERVER_CAP = {self.server_cap}")
        self.rsu.log.append("V2V: beacon | V2I: request, grant, complete | Controller: RSU")
        self.rsu.log.append("=" * 72)

        for time_step in range(max_steps + 1):
            self.rsu.update_phase(time_step)

            for vehicle in self.vehicles:
                if vehicle.plan.arrival_time == time_step and vehicle.state == VehicleState.APPROACHING:
                    self.send_beacon_and_request(vehicle, time_step)

            self.rsu.process_waiting_queue(time_step)
            self.rsu.update_active_vehicles(time_step)

            if self.all_done() and not self.rsu.active and not self.rsu.waiting_queue:
                self.rsu.log.append(f"t={time_step:02d} | END   | All vehicles completed successfully.")
                break
        else:
            self.rsu.log.append("END | Maximum simulation steps reached before all vehicles completed.")

        completed = [v for v in self.vehicles if v.state == VehicleState.DONE]
        metrics = SimulationMetrics(
            mode=self.mode.upper(),
            server_cap=self.server_cap,
            total_vehicles=len(self.vehicles),
            completed_vehicles=len(completed),
            total_wait_time=sum(v.wait_time for v in self.vehicles),
            max_wait_time=max((v.wait_time for v in self.vehicles), default=0),
            total_simulation_time=max((v.completed_time or 0 for v in self.vehicles), default=0),
            simultaneous_crossing_events=self.rsu.simultaneous_crossing_events,
            conflict_violations=self.rsu.conflict_violations,
        )

        self.rsu.log.append("-" * 72)
        self.rsu.log.append("Summary metrics")
        self.rsu.log.append(f"completed_vehicles={metrics.completed_vehicles}/{metrics.total_vehicles}")
        self.rsu.log.append(f"total_wait_time={metrics.total_wait_time}")
        self.rsu.log.append(f"average_wait_time={metrics.average_wait_time:.2f}")
        self.rsu.log.append(f"max_wait_time={metrics.max_wait_time}")
        self.rsu.log.append(f"total_simulation_time={metrics.total_simulation_time}")
        self.rsu.log.append(f"simultaneous_crossing_events={metrics.simultaneous_crossing_events}")
        self.rsu.log.append(f"conflict_violations={metrics.conflict_violations}")
        self.rsu.log.append("-" * 72)
        return self.rsu.log, metrics


def write_log(path: Path, lines: List[str]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Milestone 03 abstract cross-traffic controller")
    parser.add_argument("--mode", choices=["mm1", "mm2"], required=True, help="Queueing abstraction mode")
    parser.add_argument("--steps", type=int, default=20, help="Maximum simulation steps")
    parser.add_argument("--output", type=Path, default=None, help="Optional file path for writing the log")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    simulation = CrossTrafficSimulation(mode=args.mode)
    log, _metrics = simulation.run(max_steps=args.steps)
    text = "\n".join(log)
    print(text)
    if args.output:
        write_log(args.output, log)


if __name__ == "__main__":
    main()
