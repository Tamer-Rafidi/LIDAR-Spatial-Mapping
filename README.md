#  Embedded Spatial Measurement System

Embedded Spatial Measurement System is a microcontroller-based project that integrates a time-of-flight sensor, stepper motor, and Python-based visualization to construct 3D spatial maps in real time. Built on the Texas Instruments MSP432E401Y microcontroller, the system demonstrates embedded hardware control, sensor integration, and PC-based data visualization.

##  Features

-  **Microcontroller (MSP432E401Y)**  
  ARM Cortex-M4F processor, 256KB SRAM, 12 MHz clock speed, programmed in C/C++ and Assembly.

-  **Stepper Motor Control**  
  ULN2003 driver with 512 steps per rotation, 5–12V operating range, and LED step indicators.

-  **Time-of-Flight Sensor**  
  VL53L1X sensor with up to 400 cm range, ±20 mm error margin, 50 Hz sampling rate, and I2C communication.

-  **Data Communication**  
  I2C protocol between sensor and microcontroller; UART/USB link between microcontroller and PC at 115200 bps.

-  **3D Visualization**  
  Python 3.8 with PySerial, NumPy, and Matplotlib to plot real-time distance data in x-y-z space.

-  **System Control**  
  Onboard GPIO button toggle to start/stop data capture.

##  Tech Stack

-  **Hardware:** MSP432E401Y microcontroller, ULN2003 stepper driver, VL53L1X ToF sensor, Analog Discovery 2  
-  **Programming Languages:** C, C++, Assembly, Python  
-  **Libraries:** PySerial, NumPy, Matplotlib  
-  **Tools:** KEIL uVision5, Quartus Programmer, IDLE/Python IDE  

##  Performance Metrics

-  **Stepper Motor:** 512 steps per rotation → 5.625° resolution (0.098 rad per step)
-  **Sensor Accuracy:** ±20 mm ranging error, 400 cm max range
-  **Communication:** 115200 bps UART, I2C at 100 kHz between sensor and microcontroller
-  **Visualization:** Real-time updates with Z-axis depth increment of 0.1 units per scan
-  **Limitations:** ToF sensor maxes out at 50 Hz; FPU is 32-bit (slowing 64-bit ops); PC UART capped at 128 kbps

##  Future Improvements

-  Increase visualization performance with GPU acceleration or optimized Python libraries
-  Improve ToF sampling speed beyond the 50 Hz limitation
-  Implement higher baud rate UART communication for faster data transfer
-  Enhance portability by integrating wireless communication (e.g., Bluetooth/Wi-Fi)
-  Add multi-sensor support for full 3D spatial scanning instead of planar measurements
