#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 1  // 对于ST7789通常需要交换字节

/*====================
   MEMORY SETTINGS
 *====================*/
#define LV_MEM_CUSTOM 1
#define LV_MEMCPY_MEMSET_STD 1

/*====================
   HAL SETTINGS
 *====================*/
#define LV_DISP_DEF_REFR_PERIOD 30
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "freertos/FreeRTOS.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (xTaskGetTickCount() * portTICK_PERIOD_MS)

/*====================
   DISPLAY SETTINGS
 *====================*/
#define LV_HOR_RES_MAX 240
#define LV_VER_RES_MAX 240

/*====================
   FONT SETTINGS
 *====================*/
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*====================
   FEATURE CONFIGURATION
 *====================*/
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

/* 必须启用的核心功能 */
#define LV_USE_OBJ 1
#define LV_USE_GROUP 1
#define LV_USE_ANIMATION 1

/* 显示和渲染相关 */
#define LV_USE_DISPLAY 1
#define LV_USE_REFR_DEBUG 1  // 启用刷新调试

/* 绘制和缓冲区 */
#define LV_USE_DRAW_BUF 1
#define LV_DRAW_BUF_ALIGN 4
#define LV_USE_GPU_STM32_DMA2D 0

/* 样式系统 - 这是关键！ */
#define LV_USE_STYLE 1
#define LV_USE_STYLE_DEBUG 0

/* 事件系统 - 这也是关键！ */
#define LV_USE_EVENT 1
#define LV_USE_OBSERVER 1

/* 刷新系统 - 必须启用 */
#define LV_USE_REFR 1

/* 对象类型支持 */
#define LV_USE_LABEL 1
#define LV_USE_BUTTON 1
#define LV_USE_BTN 1
#define LV_USE_IMG 1
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_SLIDER 1

/* 主题支持 */
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 0
#define LV_THEME_DEFAULT_GROW 1

/*====================
   OS INTEGRATION
 *====================*/
#define LV_USE_OS 1
#define LV_OS_INCLUDE "freertos/FreeRTOS.h"
#define LV_OS_PTHREAD 0

/*====================
   MEMORY ALLOCATOR
 *====================*/
#define LV_USE_MEM 1
#define LV_USE_MALLOC 1
#define LV_USE_STDLIB_MALLOC 1
#define LV_USE_STDLIB_REALLOC 1
#define LV_USE_STDLIB_FREE 1
#define LV_USE_STDLIB_CALLOC 1
#define LV_USE_STDLIB_MEMSET 1
#define LV_USE_STDLIB_MEMCPY 1
#define LV_USE_STDLIB_MEMMOVE 1

/*====================
   STRING FUNCTIONS
 *====================*/
#define LV_USE_STDLIB_STRING 1
#define LV_USE_STDLIB_STRLEN 1
#define LV_USE_STDLIB_STRCPY 1
#define LV_USE_STDLIB_STRCMP 1
#define LV_USE_STDLIB_STRNCMP 1
#define LV_USE_STDLIB_STRNCPY 1
#define LV_USE_STDLIB_SNPRINTF 1
#define LV_USE_STDLIB_VSNPRINTF 1
#define LV_USE_STDLIB_SPRINTF 1
#define LV_USE_STDLIB_VSPRINTF 1
#define LV_USE_STDLIB_STRSTR 1
#define LV_USE_STDLIB_STRTOK 1
#define LV_USE_STDLIB_STRTOL 1
#define LV_USE_STDLIB_STRTOUL 1
#define LV_USE_STDLIB_STRTOLL 1
#define LV_USE_STDLIB_STRTOULL 1
#define LV_USE_STDLIB_ATOI 1
#define LV_USE_STDLIB_ATOF 1
#define LV_USE_STDLIB_STRTOD 1
#define LV_USE_STDLIB_STRTOLD 1
#define LV_USE_STDLIB_STRTOK_R 1

/*====================
   ASSERTS
 *====================*/
#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MALLOC 1
#define LV_USE_ASSERT_STYLE 1
#define LV_USE_ASSERT_MEM_INTEGRITY 1
#define LV_USE_ASSERT_OBJ 1

#endif // LV_CONF_H