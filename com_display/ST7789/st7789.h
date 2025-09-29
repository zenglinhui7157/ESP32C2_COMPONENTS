#ifndef _ST7789_H_
#define _ST7789_H_

#include "esp_log.h"
#include <stdint.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define COLOR_RGB(r,g,b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000

#define ST7789_INIT_TAG "st7789_init"

// ESP32C2 默认引脚定义（可根据实际硬件修改）
#define ST7789_PIN_NUM_MOSI  6
#define ST7789_PIN_NUM_CLK   7
#define ST7789_PIN_NUM_CS    10
#define ST7789_PIN_NUM_DC    8
#define ST7789_PIN_NUM_RST   9
#define ST7789_PIN_NUM_BLK   11

typedef enum {
	DISPLAY_ROTATION_0,
	DISPLAY_ROTATION_90,
	DISPLAY_ROTATION_180,
	DISPLAY_ROTATION_270,
} DisplayRotation;

esp_err_t st7789_init(void);
void st7789_fill_screen(uint16_t color);
void st7789_write_cmd(uint8_t cmd);
void st7789_write_data(const uint8_t *data, int len);
void st7789_reset(void);
void st7789_set_rotation(DisplayRotation rotation);
void st7789_set_window(uint16_t xStart, uint16_t xEnd, uint16_t yStart, uint16_t yEnd);
void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void st7789_fill_screen(uint16_t color);

#endif
