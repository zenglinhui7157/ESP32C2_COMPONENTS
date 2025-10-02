idf_component_register(
        SRCS
            com_display.c
            LVGL/lvgl_port.c
            ST7789/st7789.c
        INCLUDE_DIRS "." "FONTS" "ST7789" "LVGL"
            REQUIRES lvgl driver
)
