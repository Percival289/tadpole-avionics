# Simulation mode

Simulation mode is isolated in this directory and is enabled by default in
this HIL checkout.

To explicitly enable it in another build, use the `TADPOLE_SIM_TEST=1`
compiler definition. The Arduino CLI equivalent is:

```text
arduino-cli compile --build-property compiler.cpp.extra_flags=-DTADPOLE_SIM_TEST=1
```

The current repository configuration identifies an ESP32 board, while the
intended HIL target is a Teensy 4.1. Before flashing a Teensy, select the
Teensy board/core in the build tool and verify that its USB `Serial` interface
is available. The simulation code uses only Arduino-compatible serial APIs.

When enabled, `sim_hil.cpp` replaces the physical MPU6050 read path, consumes
AviSim sensor/control frames, runs the hover PID controller, and sends actuator
commands back to AviSim. Normal builds retain the existing physical sensor
path.

The controller uses the current Leapfrog Phase 1 HIL baseline from
`sim_config.h`: 204.1166 kg maximum liftoff mass, 2224.11 N (500 lbf) maximum
engine thrust, and +/-10 degrees of TVC travel. The simulated engine thrust
axis is local +Y. Update `sim_config.h` together with AviSim's
`src/core/vehicle_config.h` when the released vehicle mass properties or engine
limits change.
