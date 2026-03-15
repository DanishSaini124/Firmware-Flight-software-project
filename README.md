

This project is a real-time avionics suite built for high-power sounding rockets. It utilizes an ESP32 and an MPU6050 IMU to handle flight logic, sensor fusion, and safety protocols within a deterministic, interrupt-driven environment.
System Architecture

Unlike standard "Arduino-style" loops, this system utilizes a Hardware Timer Interrupt to maintain a strict 400Hz frequency (2.5ms intervals). This ensures deterministic execution of flight logic, which is critical for stability and control.
Key Technical Features:

    Interrupt-Driven Executive: Uses the ESP32 hardware timer (hw_timer_t) for high-precision loop timing.

    Finite State Machine (FSM): Manages flight phases including Launch, Boost, Coast, and Apogee.

    Digital Signal Processing: Implements an Exponential Moving Average (EMA) filter to mitigate engine vibration noise.

    Load Monitoring: Real-time telemetry tracks CPU load in microseconds (μs) to ensure timing margins.

🛠 Hardware Configuration
Component	Pin (ESP32)	Purpose
MPU6050	SDA (21), SCL (22)	6-DOF Inertial Measurement
RED LED	GPIO 2	Parachute Deployment / Tilt Alarm
Resistor	220 Ω	Current Limiting
Logic & Signal Processing
Digital Filtering

To handle the "noisy" environment of a rocket motor, the system processes raw Z-axis acceleration through a low-pass EMA filter:
y[n]=α⋅x[n]+(1−α)⋅y[n−1]

Where α=0.15 is used to prioritize signal stability over raw data spikes.
Mission State Machine

The flight software transitions through the following states based on detected physics:

    ON_PAD: Waiting for Z>15.0m/s2 (Liftoff).

    BOOST: Tracking peak G-force during active thrust.

    COAST: Detecting engine burnout and calculating the approach to Apogee.

    APOGEE: Detecting weightlessness (Z≈0m/s2) and triggering recovery systems.

    LANDED: Final state reached after impact detection.

📊 Performance

Based on simulation data, the executive maintains a significant timing margin:

    Interval Budget: 2,500μs

    Actual Execution Load: ≈1,980μs

    CPU Headroom: 20%

💻 How to Run

    Upload the .ino sketch to an ESP32.

    Open the Serial Monitor at 115200 baud.

    Observe the "Flight Executive Heartbeat" and real-time state transitions.

    Note: This project was developed as part of the Electrical Engineering curriculum at Santa Clara University (SCU), focusing on real-time systems and avionics safety.
    
