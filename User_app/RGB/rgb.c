#include "rgb.h"
#include "main.h"
#include "tim.h"


//uint16_t static RGB_buffur[RESET_PULSE + WS2812_DATA_LEN] = { 0 };

uint16_t  RGB_buffur[RESET_PULSE + WS2812_DATA_LEN] = { 0 };

void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    //
    uint16_t* p = (RGB_buffur + RESET_PULSE) + (num * LED_DATA_LEN);
    
    for (uint16_t i = 0;i < 8;i++)
    {
        //
        p[i]      = (G << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
        p[i + 8]  = (R << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
        p[i + 16] = (B << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
    }
}
/*ws2812 初始化*/
void ws2812_init(uint8_t led_nums)
{
	uint16_t num_data;
	num_data = RESET_PULSE + led_nums * 24;
	for(uint8_t i = 0; i < led_nums; i++)
	{
		ws2812_set_RGB(0x00, 0x00, 0x00, i);
	}
	 HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_2,(uint32_t *)RGB_buffur,(num_data)); 
}


/*全蓝*/
void ws2812_blue(uint8_t led_nums)
{
	uint16_t num_data;
	num_data = RESET_PULSE + led_nums * 24;
	for(uint8_t i = 0; i < led_nums; i++)
	{
		ws2812_set_RGB(0x00, 0x00, 0x22, i);
	}
	 HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_2,(uint32_t *)RGB_buffur,(num_data));
}
/*全红*/
void ws2812_red(uint8_t led_nums)
{
	uint16_t num_data;
	num_data = RESET_PULSE + led_nums * 24;
	for(uint8_t i = 0; i < led_nums; i++)
	{
		ws2812_set_RGB(0x22, 0x00, 0x00, i);
	}
	 HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_2,(uint32_t *)RGB_buffur,(num_data));
}
/*全绿*/
void ws2812_green(uint8_t led_nums)
{
	uint16_t num_data;
	num_data = RESET_PULSE + led_nums * 24;
	for(uint8_t i = 0; i < led_nums; i++)
	{
		ws2812_set_RGB(0x00, 0x22, 0x00, i);
	}
	 HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_2,(uint32_t *)RGB_buffur,(num_data));
}

void ws2812_example(void)
{		
	ws2812_set_RGB(0x00, 0x00, 0x22, 0);
    ws2812_set_RGB(0x00, 0x00, 0x22, 1);
    ws2812_set_RGB(0x00, 0x00, 0x22, 2);
    ws2812_set_RGB(0x00, 0x00, 0x22, 3);
    ws2812_set_RGB(0x00, 0x00, 0x22, 6);
    ws2812_set_RGB(0x00, 0x00, 0x22, 7);
	ws2812_set_RGB(0x00, 0x00, 0x22, 8);
    ws2812_set_RGB(0x00, 0x00, 0x22, 10);
    HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_2,(uint32_t *)RGB_buffur,(RESET_PULSE + 10 * 24)); //344 = 80 + 24 * LED_NUMS(11)

//    HAL_Delay(500);
}

