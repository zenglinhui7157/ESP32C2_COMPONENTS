idf_component_register(
    SRCS
        com_display.c
        ST7789/st7789.c
    INCLUDE_DIRS "." "ST7789"
    REQUIRES driver
)
