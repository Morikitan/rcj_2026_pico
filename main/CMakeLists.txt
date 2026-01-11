# Add executable. Default name is the project name, version 0.1

add_executable(main main.cpp)

target_compile_definitions(main PRIVATE PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

# Add the standard include files to the build
target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_include_directories(main PRIVATE ../lib)

# Add the standard library to the build
target_link_libraries(main pico_stdlib hardware_pwm hardware_i2c hardware_gpio hardware_uart)
target_link_libraries(main BLDC rp2350 encoder gyro)

pico_add_extra_outputs(main)