cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}cmakeArduinoToolchain.cmake)
set(PROJECT_NAME SmartDesk)
project(SmartDesk)

set(${CMAKE_PROJECT_NAME}_SKETCH SmartDesk.ino)

#### Uncomment below additional settings as needed.
# set(${CMAKE_PROJECT_NAME}_BOARD mega)
set(${CMAKE_PROJECT_NAME}_PORT com6)
set(mega.build.mcu mega)
# set(mega.upload.protocol wiring)
# set(mega.upload.speed 115200)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})