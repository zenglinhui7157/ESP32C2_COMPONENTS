#ifndef _ST7789_H_
#define _ST7789_H_

#include "esp_log.h"
#include <stdint.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define ST7789_SPI_MODE 0
#define ST7789_SPI_CLOCK_HZ 40000000 // 40 MHz

#define COLOR_RGB(r,g,b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000

#define ST7789_INIT_TAG "st7789_init"

// ESP32C2 默认引脚定义（可根据实际硬件修改）
#define ST7789_PIN_NUM_MOSI  7
#define ST7789_PIN_NUM_CLK   6
#define ST7789_PIN_NUM_CS    3
#define ST7789_PIN_NUM_DC    10
#define ST7789_PIN_NUM_RST   4
#define ST7789_PIN_NUM_BLK   18


typedef enum {
	DISPLAY_ROTATION_0,
	DISPLAY_ROTATION_90,
	DISPLAY_ROTATION_180,
	DISPLAY_ROTATION_270,
} DisplayRotation;

esp_err_t st7789_init(void);
esp_err_t st7789_fill_screen(uint16_t color);
esp_err_t st7789_write_cmd(uint8_t cmd);
esp_err_t st7789_write_data(const uint8_t *data, int len);
esp_err_t st7789_reset(void);
esp_err_t st7789_set_rotation(DisplayRotation rotation);
esp_err_t st7789_set_window(uint16_t xStart, uint16_t xEnd, uint16_t yStart, uint16_t yEnd);
esp_err_t st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
esp_err_t st7789_fill_screen(uint16_t color);

#endif
