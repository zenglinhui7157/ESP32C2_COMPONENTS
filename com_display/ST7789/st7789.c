#include "st7789.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static spi_device_handle_t st7789_spi;

static void st7789_gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << ST7789_PIN_NUM_DC) | (1ULL << ST7789_PIN_NUM_RST) | (1ULL << ST7789_PIN_NUM_BLK),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);
    gpio_set_level(ST7789_PIN_NUM_BLK, 1); // 背光默认打开
}

esp_err_t st7789_init(void)
{
    ESP_LOGI(ST7789_INIT_TAG, "st7789_init called");
    st7789_gpio_init();

    spi_bus_config_t buscfg = {
        .mosi_io_num = ST7789_PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = ST7789_PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = TFT_WIDTH * TFT_HEIGHT * 2 + 8
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 40 * 1000 * 1000,
        .mode = 2,
        .spics_io_num = ST7789_PIN_NUM_CS,
        .queue_size = 7,
        .pre_cb = NULL
    };
    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI2_HOST, &devcfg, &st7789_spi);

    st7789_reset();

    // ST7789初始化命令序列（参考Hardware/st7789.c）
    st7789_write_cmd(0x11); // Sleep Out
    vTaskDelay(pdMS_TO_TICKS(120));

    st7789_write_cmd(0xB2); // Frame rate control
    uint8_t porch_data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
    st7789_write_data(porch_data, sizeof(porch_data));

    st7789_write_cmd(0x20); // Display inversion off

    st7789_write_cmd(0xB7); // Gate control
    uint8_t gate = 0x56;
    st7789_write_data(&gate, 1);

    st7789_write_cmd(0xBB); // VCOM
    uint8_t vcom = 0x18;
    st7789_write_data(&vcom, 1);

    st7789_write_cmd(0xC0); // LCM control
    uint8_t lcm = 0x2C;
    st7789_write_data(&lcm, 1);

    st7789_write_cmd(0xC2); // VDV & VRH enable
    uint8_t vdv_vrh = 0x01;
    st7789_write_data(&vdv_vrh, 1);

    st7789_write_cmd(0xC3); // VRH set
    uint8_t vrh = 0x1F;
    st7789_write_data(&vrh, 1);

    st7789_write_cmd(0xC4); // VDV set
    uint8_t vdv = 0x20;
    st7789_write_data(&vdv, 1);

    st7789_write_cmd(0xC6); // FR Control 2
    uint8_t fr = 0x0F;
    st7789_write_data(&fr, 1);

    st7789_write_cmd(0xD0); // Power Control 1
    uint8_t power_data[] = {0xA6, 0xA1};
    st7789_write_data(power_data, 2);

    // Gamma正
    uint8_t gamma_positive[] = {0xD0,0x0D,0x14,0x0B,0x0B,0x07,0x3A,0x44,0x50,0x08,0x13,0x13,0x2D,0x32};
    st7789_write_cmd(0xE0);
    st7789_write_data(gamma_positive, sizeof(gamma_positive));

    // Gamma负
    uint8_t gamma_negative[] = {0xD0,0x0D,0x14,0x0B,0x0B,0x07,0x3A,0x44,0x50,0x08,0x13,0x13,0x2D,0x32};
    st7789_write_cmd(0xE1);
    st7789_write_data(gamma_negative, sizeof(gamma_negative));

    st7789_write_cmd(0x36); // MADCTL
    uint8_t madctl = 0x60; // 默认旋转
    st7789_write_data(&madctl, 1);

    st7789_write_cmd(0x3A); // COLMOD
    uint8_t colmod = 0x55; // 16-bit RGB565
    st7789_write_data(&colmod, 1);

    st7789_write_cmd(0xE7); // SPI双向模式
    uint8_t spi_mode = 0x00;
    st7789_write_data(&spi_mode, 1);

    st7789_write_cmd(0x21); // Display Inversion On
    st7789_write_cmd(0x29); // Display On

    st7789_set_rotation(DISPLAY_ROTATION_0); // 默认旋转
    vTaskDelay(pdMS_TO_TICKS(100));

    return ESP_OK;
}

void st7789_reset(void)
{
    ESP_LOGI(ST7789_INIT_TAG, "st7789_reset called");
    gpio_set_level(ST7789_PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(150));
    gpio_set_level(ST7789_PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(150));
}

void st7789_write_cmd(uint8_t cmd)
{
    gpio_set_level(ST7789_PIN_NUM_DC, 0);
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd
    };
    spi_device_transmit(st7789_spi, &t);
}

void st7789_write_data(const uint8_t *data, int len)
{
    gpio_set_level(ST7789_PIN_NUM_DC, 1);
    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = data
    };
    spi_device_transmit(st7789_spi, &t);
}

void st7789_set_rotation(DisplayRotation rotation)
{
    ESP_LOGI(ST7789_INIT_TAG, "set rotation: %d", rotation);
    // TODO: 发送 MADCTL 命令
    st7789_write_cmd(0x36);
    uint8_t madctl = 0x00;
    switch(rotation) {
        case DISPLAY_ROTATION_0: madctl = 0x00; break;
        case DISPLAY_ROTATION_90: madctl = 0x60; break;
        case DISPLAY_ROTATION_180: madctl = 0xC0; break;
        case DISPLAY_ROTATION_270: madctl = 0xA0; break;
    }
    st7789_write_data(&madctl, 1);
}

void st7789_set_window(uint16_t xStart, uint16_t xEnd, uint16_t yStart, uint16_t yEnd)
{
    ESP_LOGI(ST7789_INIT_TAG, "set window: x[%d-%d] y[%d-%d]", xStart, xEnd, yStart, yEnd);
    // TODO: 发送窗口设置命令
    st7789_write_cmd(0x2A); // Column Address Set
    uint8_t data_col[4] = {xStart >> 8, xStart & 0xFF, xEnd >> 8, xEnd & 0xFF};
    st7789_write_data(data_col, 4);
    st7789_write_cmd(0x2B); // Row Address Set
    uint8_t data_row[4] = {yStart >> 8, yStart & 0xFF, yEnd >> 8, yEnd & 0xFF};
    st7789_write_data(data_row, 4);
    st7789_write_cmd(0x2C); // Memory Write
}

void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    ESP_LOGI(ST7789_INIT_TAG, "Draw pixel at (%d, %d) color: 0x%04X", x, y, color);
    // TODO: 设置窗口并写入像素数据
    st7789_set_window(x, x, y, y);
    uint8_t data[2] = {color >> 8, color & 0xFF};
    st7789_write_data(data, 2);
}

void st7789_fill_screen(uint16_t color)
{
    ESP_LOGI(ST7789_INIT_TAG, "Fill screen with color: 0x%04X", color);
    // TODO: 填充整个屏幕
    st7789_set_window(0, TFT_WIDTH-1, 0, TFT_HEIGHT-1);
    uint8_t data[2] = {color >> 8, color & 0xFF};
    size_t size = TFT_WIDTH * TFT_HEIGHT;
    for (size_t i = 0; i < size; ++i) {
        st7789_write_data(data, 2);
    }
}
