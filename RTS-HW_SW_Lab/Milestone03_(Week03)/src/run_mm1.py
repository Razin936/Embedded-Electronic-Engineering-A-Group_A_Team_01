"""Convenience wrapper for the M/M/1 abstract implementation run."""

from abstract_cross_traffic_controller import CrossTrafficSimulation, write_log
from pathlib import Path

if __name__ == "__main__":
    log, _ = CrossTrafficSimulation(mode="mm1").run(max_steps=20)
    print("\n".join(log))
    write_log(Path("results/mm1_simulation_log.txt"), log)
