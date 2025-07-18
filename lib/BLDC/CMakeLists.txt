add_library(BLDC BLDC.cpp)

pico_set_program_name(BLDC "BLDC")
pico_set_program_version(BLDC "0.1")

target_link_libraries(BLDC pico_stdlib hardware_pwm)