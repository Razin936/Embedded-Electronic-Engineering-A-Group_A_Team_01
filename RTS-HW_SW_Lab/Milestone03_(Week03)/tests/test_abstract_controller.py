"""Unit tests for the Milestone 03 abstract cross-traffic controller."""

import sys
import unittest
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT_ROOT / "src"))

from abstract_cross_traffic_controller import CrossTrafficSimulation  # noqa: E402


class TestAbstractCrossTrafficController(unittest.TestCase):
    def test_mm1_completes_without_conflict(self) -> None:
        log, metrics = CrossTrafficSimulation(mode="mm1").run(max_steps=20)
        self.assertEqual(metrics.server_cap, 1)
        self.assertEqual(metrics.completed_vehicles, 4)
        self.assertEqual(metrics.conflict_violations, 0)
        self.assertEqual(metrics.simultaneous_crossing_events, 0)
        self.assertTrue(any("SERVER_CAP = 1" in line for line in log))

    def test_mm2_completes_with_compatible_parallel_crossing(self) -> None:
        log, metrics = CrossTrafficSimulation(mode="mm2").run(max_steps=20)
        self.assertEqual(metrics.server_cap, 2)
        self.assertEqual(metrics.completed_vehicles, 4)
        self.assertEqual(metrics.conflict_violations, 0)
        self.assertGreater(metrics.simultaneous_crossing_events, 0)
        self.assertTrue(any("Vehicle1" in line and "GRANT" in line for line in log))
        self.assertTrue(any("Vehicle2" in line and "GRANT" in line for line in log))

    def test_mm2_should_reduce_waiting_against_mm1(self) -> None:
        _log1, mm1 = CrossTrafficSimulation(mode="mm1").run(max_steps=20)
        _log2, mm2 = CrossTrafficSimulation(mode="mm2").run(max_steps=20)
        self.assertLessEqual(mm2.total_wait_time, mm1.total_wait_time)
        self.assertLessEqual(mm2.total_simulation_time, mm1.total_simulation_time)


if __name__ == "__main__":
    unittest.main()
