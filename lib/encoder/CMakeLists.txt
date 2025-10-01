add_library(encoder encoder.cpp)

pico_set_program_name(encoder "encoder")
pico_set_program_version(encoder "0.1")

target_link_libraries(encoder pico_stdlib hardware_uart)