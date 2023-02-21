#include "ui.h"

/* Private includes ----------------------------------------------------------*/
#include "lvgl.h"
#include "lv_port_disp.h"
#include "ui_display.h"
/* Exported functions --------------------------------------------------------*/

void GUI_task(void *argument)
{
	lv_init();                  // lvgl初始化，如果这个没有初始化，那么下面的初始化会崩溃
	lv_port_disp_init();        // 显示器初始化
	ui_init();
	   int t=0 ,j=0;

	for(;;)
	{	
				t++;
		
		if(t>=100)
				t=0;
			
		if(t%10==0 && j==0)
		{
				j=1;
				DisplaySwitch(1);
				DisplayWifi(1);
				nextrunning_val(t);
		}
		else if(t%10==0 && j==1)
		{
				j=0;
				DisplaySwitch(0);
				DisplayWifi(0);
				nextrunning_val(t);
		}	

		lv_task_handler();
		osDelay(100);
	}
}

