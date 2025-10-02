#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "st7789.h" // 你的显示驱动头文件

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[TFT_WIDTH * 40]; // 40 行缓冲区

static void lvgl_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    ESP_LOGW("LVGL", "Flush area: x[%d-%d] y[%d-%d]", area->x1, area->x2, area->y1, area->y2);
    ESP_LOGW("LVGL", "color_p[0]=0x%04X, color_p[1]=0x%04X", color_p[0].full, color_p[1].full);
    // 设置显示区域
    st7789_set_window(area->x1, area->x2, area->y1, area->y2);
    st7789_write_cmd(0x2C); // Memory Write
    int32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

    st7789_write_data((const uint8_t *)color_p, size * 2);

    lv_disp_flush_ready(disp_drv);
}



void lvgl_port_init(void)
{
    ESP_LOGI("LVGL", "lvgl_port_init start");
    lv_init();
    
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, TFT_WIDTH * 40);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0;
    
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
    
    // 初始化默认主题
    lv_disp_set_theme(disp, lv_theme_default_init(disp, 
        lv_palette_main(LV_PALETTE_BLUE), 
        lv_palette_main(LV_PALETTE_RED), 
        false, 
        LV_FONT_DEFAULT));
    
    ESP_LOGI("LVGL", "lvgl_port_init done");
}

static void lvgl_task(void *param)
{
    while (1) {
        // ESP_LOGI("LVGL", "lv_timer_handler running");
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void lvgl_port_start(void)
{
    xTaskCreate(lvgl_task, "lvgl", 16384, NULL, 5, NULL);
}