add_library(pico2 pico2.cpp)

pico_set_program_name(pico2 "pico2")
pico_set_program_version(pico2 "0.1")

target_link_libraries(pico2 pico_stdlib hardware_spi)