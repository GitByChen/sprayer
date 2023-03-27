#include "ui.h"
#include "usart.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
#include "gpio.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lcd_init.h"
#include "lcd.h"
#include "iwdg.h"
#include "ui_display.h"
#include  "time.h"
#include "lv_qrcode.h"
#include "bc260y.h"
#include "work.h"
#include "cJSON_user.h"
#include "RC522S.h"
#include "HX711.h"
/* Exported functions --------------------------------------------------------*/
extern SemaphoreHandle_t MutexSemaphore;	//�����ź���

enum UI_EVENT ui_event_cb;
void qc_andisplay(uint8_t val)
{
   // lv_obj_del(obj);	ɾ������
   //	lv_obj_swap(ui_Screen1, obj);		//����λ��
	static uint8_t old_val=2;
	if(val!=old_val)
	{	
		if(val==0)
		{
			lv_obj_clear_flag(qc_module, LV_OBJ_FLAG_HIDDEN);		//�������
			//	lv_obj_add_flag(ui_Screen1, LV_OBJ_FLAG_HIDDEN);
			// lv_obj_move_foreground(obj);
		}
		else if(val==1)
		{
		//	lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_HIDDEN);
		  lv_obj_add_flag(qc_module, LV_OBJ_FLAG_HIDDEN);		//��������
		//	lv_obj_move_background(obj);						
		}
		old_val=val;
	}
}
void LableWifi_event_cb(lv_event_t * e)
{
	
	if(BC260_Massage.CSG_val<10)
	{
		lv_label_set_text(ui_LableWiFi, USER_WIFI1);
	}
	else if(BC260_Massage.CSG_val>=10 && BC260_Massage.CSG_val<20)
	{
		lv_label_set_text(ui_LableWiFi, USER_WIFI2);
	}
	else
	{
		lv_label_set_text(ui_LableWiFi, USER_WIFI3);
	}
}

void qc_event_cb(lv_event_t * e)
{
	   // lv_obj_del(obj);	ɾ������
   //	lv_obj_swap(ui_Screen1, obj);		//����λ��
	if(BC260_Massage.bindFlag==0)
	{
		lv_obj_clear_flag(qc_module, LV_OBJ_FLAG_HIDDEN);		//�������
		 lv_obj_add_flag(ui_Screen1, LV_OBJ_FLAG_HIDDEN);
	//	 lv_obj_move_foreground(qc_module);
	}
	else if(BC260_Massage.bindFlag==1)
	{
		lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(qc_module, LV_OBJ_FLAG_HIDDEN);		//��������
	//	lv_obj_move_background(qc_module);						
	}
}

void runningswitch_event_cb(lv_event_t * e)
{
	if(work_time.work_time_flag)
	{
		lv_obj_add_state(ui_runningswitch, LV_STATE_CHECKED);
	}
	else
	{
		lv_obj_clear_state(ui_runningswitch, LV_STATE_CHECKED);
		 lv_bar_set_value(ui_NextRunningBar, 0, LV_ANIM_OFF); //û������й�����������������
	}
}
void NextRunningBar_event_cb(lv_event_t * e)
{
	u8 val=0;
	val=work_time.working_interval_time*100/Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].interval_time;
	lv_bar_set_value(ui_NextRunningBar, val, LV_ANIM_OFF);
}
void surplus_change_cb(lv_event_t * e)
{
	u8 surplus=0;
	if(Pcd_Massage_Flag.Have_A_Card==PCD_OK)
	{
		surplus=HX711_Massage.Write_To_Card_Weight*100/1000;
		lv_slider_set_value(ui_Slider2, surplus, LV_ANIM_OFF);
		lv_label_set_text_fmt(ui_PercentVal,"%02d",surplus);
	}
	else
	{
		lv_slider_set_value(ui_Slider2, 0, LV_ANIM_OFF);
		lv_label_set_text_fmt(ui_PercentVal,"%02d",0);
	}
}
void GUI_task(void *argument)
{
	lv_init();                  // lvgl��ʼ����������û�г�ʼ������ô����ĳ�ʼ�������
	lv_port_disp_init();        // ��ʾ����ʼ��

	ui_init();
	while(strstr(BC260_Massage.BC260_SN,"MPN")==0)
	{
		lv_task_handler();
		osDelay(100);
	}
	qc_init();	
	lv_event_send(qc_module,LV_EVENT_COVER_CHECK,NULL);	//�����ж�һ�ΰ�״̬
	for(;;)
	{	
		switch (ui_event_cb)
		{
		case signal:
			/* code */
			lv_event_send(ui_LableWiFi,LV_EVENT_COVER_CHECK,NULL);	//�����ź�ͼ��ˢ���¼�
			ui_event_cb=normal;
			break;
		case bind:
			lv_event_send(qc_module,LV_EVENT_COVER_CHECK,NULL);	//����һ�ν����л��¼�
			ui_event_cb=normal;
			break;
		case run_static:
			lv_event_send(ui_runningswitch,LV_EVENT_COVER_CHECK,NULL);	//����һ��״̬�л��¼�
			ui_event_cb=normal;
			break;
		case NextRunning_Reflash:
			lv_event_send(ui_NextRunningBar,LV_EVENT_VALUE_CHANGED,NULL);	//����һ������������¼�
			ui_event_cb=normal;
			break;
		case surplus_change:
			lv_event_send(ui_PercentVal,LV_EVENT_VALUE_CHANGED,NULL);	//����һ��ʣ������ʾˢ���¼�
			ui_event_cb=normal;

			break;
		default:
			break;

		}
		lv_task_handler();
//		xSemaphoreGive(MutexSemaphore);					//�ͷ��ź��� 
		osDelay(100);
	}

}

