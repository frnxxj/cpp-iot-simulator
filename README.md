
# IoT Sensor Simulator

A multi-threaded C++20 application simulating an embedded sensor network.
The project demonstrates core systems programming concepts: thread safety, memory management without allocations on the hot path, and custom binary protocol serialization.

## Tech Stack

* **Language:** C++20
* **Build System:** CMake
* **Tools:** MinGW / GCC, CLion

## How to Build

1.  Clone the repository:
    ```bash
    git clone [https://github.com/frnxxj/cpp-iot-simulator.git](https://github.com/frnxxj/cpp-iot-simulator.git)
    cd cpp-iot-simulator
    ```

2.  Build using CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

3.  Run the simulator:
    ```bash
    ./qwe.exe
    ```
    *Check `logs.txt` for the output.*