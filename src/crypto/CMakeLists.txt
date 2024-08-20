file(GLOB C_SOURCES */*.c)
file(GLOB ASM_SOURCES cortex/*.s)

add_library(pico_wireguard_crypto
    ${C_SOURCES}
    ${ASM_SOURCES}
)
