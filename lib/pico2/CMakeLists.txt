add_library(pico2 pico2.cpp)

pico_set_program_name(pico2 "pico2")
pico_set_program_version(pico2 "0.1")

pico_generate_pio_header(pico2 ${CMAKE_CURRENT_LIST_DIR}/spi.pio)

target_link_libraries(pico2 pico_stdlib hardware_spi hardware_pio)

target_sources(pico2 PRIVATE
        pio_spi.c
        pio_spi.h
        )