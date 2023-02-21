#include "ui.h"

/* Private includes ----------------------------------------------------------*/
#include "lvgl.h"
#include "lv_port_disp.h"
#include "ui_display.h"
/* Exported functions --------------------------------------------------------*/

void GUI_task(void *argument)
{
	lv_init();                  // lvgl��ʼ����������û�г�ʼ������ô����ĳ�ʼ�������
	lv_port_disp_init();        // ��ʾ����ʼ��
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

