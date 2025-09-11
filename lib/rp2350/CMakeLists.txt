add_library(rp2350 rp2350.cpp)

pico_set_program_name(rp2350 "rp2350")
pico_set_program_version(rp2350 "0.1")

pico_generate_pio_header(rp2350 ${CMAKE_CURRENT_LIST_DIR}/spi_slave.pio)

target_link_libraries(rp2350 pico_stdlib hardware_spi hardware_pio)
target_link_libraries(rp2350 hardware_clock)
