#ifndef _RGB_H_
#define _RGB_H_

#include "main.h"
#include "stdint.h"
//#include "type.h"
#define ONE_PULSE        (67)                           //1 码
#define ZERO_PULSE       (32)                           //0 码
#define RESET_PULSE      (80)                           //80 ，复位信号
#define LED_NUMS         (10)                            //led 数量
#define LED_DATA_LEN     (24)                           //led 长度，单个需要24bits
#define WS2812_DATA_LEN  (LED_NUMS*LED_DATA_LEN)        //ws2812灯条需要的数组长度

extern uint16_t  RGB_buffur[RESET_PULSE + WS2812_DATA_LEN];

void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num);

void ws2812_example(void);
extern void ws2812_blue(uint8_t led_nums);
extern void ws2812_red(uint8_t led_nums);
extern void ws2812_green(uint8_t led_nums);
void ws2812_init(uint8_t led_nums);
#endif

