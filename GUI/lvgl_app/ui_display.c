// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.3.3
// PROJECT: sample
#include "stdio.h"
#include "ui_display.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_font.h"
#include  "ui.h"
#include "bc260y.h"
#include "lv_qrcode.h"
#include "cJSON_user.h"
#include "work.h"
///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * qc_module ;
lv_obj_t * ui_BG;
lv_obj_t * ui_LableWiFi;
//lv_obj_t * ui_LableWiFi3;
//lv_obj_t * ui_LableWiFi2;
//lv_obj_t * ui_LableWiFi1;
lv_obj_t * ui_LableTime;
lv_obj_t * ui_LableData;
lv_obj_t * ui_LableWeek;
lv_obj_t * ui_firstmodule1;
lv_obj_t * ui_loadingLogo;
lv_obj_t * ui_Task;
lv_obj_t * ui_IntervalTime;
lv_obj_t * ui_TaskTime;

lv_obj_t * ui_secondmodule;
lv_obj_t * ui_NextRunningTime;
lv_obj_t * ui_nextrunning;
lv_obj_t * ui_ThirdModule;
lv_obj_t * ui_PercentVal;
lv_obj_t * ui_Percent;
lv_obj_t * ui_Slider1;
lv_obj_t * ui_ThirdLabel;
lv_obj_t * ui_Percent1;
lv_obj_t * ui_FourthModule;
lv_obj_t * ui_duration;
lv_obj_t * ui_DurationTime;
lv_obj_t * ui_Panel2;
lv_obj_t * ui____initial_actions0;

lv_obj_t * ui_Screen2;
///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

LV_FONT_DECLARE(ui_font_confont16);
LV_FONT_DECLARE(ui_font_Font26);
LV_FONT_DECLARE(ui_font_Font14);
int num_val=0;

extern	RTC_DateTypeDef GetData;    //日期
extern	RTC_TimeTypeDef GetTime;    //时间

	RTC_DateTypeDef OldData;    //日期
	RTC_TimeTypeDef OldTime;    //时间
typedef struct _lv_clock
{
    lv_obj_t *time_label; // 时间标签
    lv_obj_t *date_label; // 日期标签
    lv_obj_t *weekday_label; // 星期标签
}lv_clock_t;

///////////////////// FUNCTIONS ////////////////////


static void clock_date_task_callback(lv_timer_t *timer)
{
    static const char *week_day[7] = { "星期日","星期一","星期二","星期三","星期四","星期五","星期六" };
    int year = GetData.Year;
    int month =GetData.Month;
    int day =GetData.Date;
    int weekday = GetData.WeekDay;
    int hour = GetTime.Hours;
    int minutes = GetTime.Minutes;
    int second =GetTime.Seconds;
 	
		if(OldData.Date!=day || OldData.Month!=month|| OldData.Year!=year)
		{
            if(weekday>6)
            {
                weekday=0;
            }
			 lv_label_set_text_fmt(ui_LableData, "20%d/%02d/%02d", year, month, day);
			 lv_label_set_text_fmt(ui_LableWeek, "%s", week_day[weekday]);
			OldData.Date=day ; 
			OldData.Month=month;
			OldData.Year=year;
		}
		if(OldTime.Hours!=hour ||OldTime.Minutes!=minutes ||OldTime.Seconds!=second)
		{
			 lv_label_set_text_fmt(ui_LableTime, "%02d:%02d:%02d", hour, minutes,second);
			OldTime.Hours=hour;
			OldTime.Minutes=minutes;
			OldTime.Seconds=second;
		}
			
}
uint8_t first_time_comming=1;
void Change_Task_Disp(void)
{
    if(work_time.work_time_flag)
    {
            lv_label_set_text_fmt(ui_Task,"任务%d",work_time.which_working_time+1);
            lv_label_set_text_fmt(ui_TaskTime,"%s~%s",Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].timestart,Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].timeend);
            lv_label_set_text_fmt(ui_IntervalTime,"间隔%dmin",Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].interval_time);
            lv_label_set_text_fmt(ui_DurationTime,"%dS",Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].worktime);
    }
    else
    {
        if(work_time.Taday_Task_Flag==0)
        {
            lv_label_set_text_fmt(ui_Task,"今日已无任务");
            lv_label_set_text_fmt(ui_TaskTime," ");
            lv_label_set_text_fmt(ui_IntervalTime," ");	
            lv_label_set_text_fmt(ui_NextRunningTime," ");
            lv_label_set_text_fmt(ui_DurationTime," ");
        }
        else{
            lv_label_set_text_fmt(ui_Task,"下一次任务为:任务%d",work_time.Next_which_working_time+1);
            lv_label_set_text_fmt(ui_TaskTime,"%s~%s",Cjson_Buf.Cjson_Buffer_Data[work_time.Next_which_working_time].timestart,Cjson_Buf.Cjson_Buffer_Data[work_time.Next_which_working_time].timeend);
            lv_label_set_text_fmt(ui_IntervalTime,"间隔%dmin",Cjson_Buf.Cjson_Buffer_Data[work_time.Next_which_working_time].interval_time);
            lv_label_set_text_fmt(ui_DurationTime,"%dS",Cjson_Buf.Cjson_Buffer_Data[work_time.Next_which_working_time].worktime);
            lv_label_set_text_fmt(ui_NextRunningTime,"%s",Cjson_Buf.Cjson_Buffer_Data[work_time.Next_which_working_time].timestart);
        }
    }
} 
void NextRunning_Disp(void)
{
    u8 hour_disp=0,min_disp=0;
     hour_disp=GetTime.Hours;
    if(work_time.working_flag==3){
        min_disp=GetTime.Minutes+Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].interval_time;      
        if(min_disp>=60)
        {
            min_disp-=60;
            hour_disp=GetTime.Hours+1;
        }
    }
    else 
    {
        if(first_time_comming==1)
        {
            first_time_comming=0;
            min_disp=GetTime.Minutes+1;
        }
        else{
            min_disp=GetTime.Minutes;
        }
    }
    lv_label_set_text_fmt(ui_NextRunningTime,"%02d:%02d",hour_disp,min_disp);

}
///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{
	/*static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);
    //Make a gradient
    lv_style_set_width(&style, 322);
    lv_style_set_height(&style,242);
		
    lv_style_set_x(&style, 0);
    lv_style_set_y(&style, 0);
	lv_style_set_bg_color(&style,lv_color_hex(0x594040));
    //ui_Screen1 = lv_obj_create(NULL);
	ui_Screen1 = lv_obj_create(lv_scr_act());
	 lv_obj_add_style(ui_Screen1, &style, 0);
*/
    ui_Screen1 = lv_obj_create(lv_scr_act());   
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_width(ui_Screen1, 320);   /// 1
    lv_obj_set_height(ui_Screen1, 240);    /// 1
    lv_obj_set_x(ui_Screen1, 0);
    lv_obj_set_y(ui_Screen1, 0);
    lv_obj_set_style_radius(ui_Screen1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x594040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LableWiFi = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableWiFi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableWiFi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableWiFi, 265);
    lv_obj_set_y(ui_LableWiFi, -10);
    lv_label_set_text(ui_LableWiFi, USER_WIFI3);//"	"
    lv_obj_set_style_text_font(ui_LableWiFi, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_LableWiFi,LableWifi_event_cb,LV_EVENT_COVER_CHECK,NULL);

/*
    ui_LableWiFi2 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableWiFi2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableWiFi2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableWiFi2, 280);
    lv_obj_set_y(ui_LableWiFi2, 4);
    lv_label_set_text(ui_LableWiFi2, "	");
    lv_obj_set_style_text_font(ui_LableWiFi2, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LableWiFi1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableWiFi1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableWiFi1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableWiFi1, 280);
    lv_obj_set_y(ui_LableWiFi1, 4);
    lv_label_set_text(ui_LableWiFi1, "");
    lv_obj_set_style_text_font(ui_LableWiFi1, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);
*/

    ui_LableTime = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableTime, -10);
    lv_obj_set_y(ui_LableTime, -10);
    lv_label_set_text(ui_LableTime, "00:00:00");
    lv_obj_set_style_text_font(ui_LableTime, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2 = lv_obj_create(ui_Screen1);      //长横线
    lv_obj_set_width(ui_Panel2, 310);
    lv_obj_set_height(ui_Panel2, 2);
    lv_obj_set_x(ui_Panel2, -10);
    lv_obj_set_y(ui_Panel2, 7);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel2, lv_color_hex(0xA8DFEF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2, 70, LV_PART_MAIN | LV_STATE_DEFAULT);


    ui_LableData = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableData, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableData, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableData, 70);
    lv_obj_set_y(ui_LableData, 15);
    lv_label_set_text(ui_LableData, "0000/00/00");
    lv_obj_set_style_text_font(ui_LableData, &ui_font_Font26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LableWeek = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableWeek, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableWeek, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableWeek, 105);
    lv_obj_set_y(ui_LableWeek, 45);
    lv_label_set_text(ui_LableWeek, "星期一");
    lv_obj_set_style_text_font(ui_LableWeek, &ui_font_Font26, LV_PART_MAIN | LV_STATE_DEFAULT);
						
    lv_timer_t* task_timer = lv_timer_create(clock_date_task_callback, 200, (void *)&ui_LableTime); // 创建定时任务，200ms刷新一次

		
    ui_firstmodule1 = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_firstmodule1, 170);
    lv_obj_set_height(ui_firstmodule1, 60);
    lv_obj_set_x(ui_firstmodule1, -10);
    lv_obj_set_y(ui_firstmodule1, 80);
    lv_obj_clear_flag(ui_firstmodule1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_firstmodule1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_firstmodule1, lv_color_hex(0x928A8A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_firstmodule1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_firstmodule1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_firstmodule1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_loadingLogo = lv_label_create(ui_firstmodule1);
    lv_obj_set_width(ui_loadingLogo, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_loadingLogo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_loadingLogo, -8);
    lv_obj_set_y(ui_loadingLogo, -11);
    lv_label_set_text(ui_loadingLogo, "	");
    lv_obj_set_style_text_font(ui_loadingLogo, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Task = lv_label_create(ui_firstmodule1);
    lv_obj_set_width(ui_Task, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Task, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Task, 14);
    lv_obj_set_y(ui_Task, -10);
    lv_label_set_text(ui_Task, "今日已无任务");
    lv_obj_set_style_text_font(ui_Task, &ui_font_Font14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IntervalTime = lv_label_create(ui_firstmodule1);
    lv_obj_set_width(ui_IntervalTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IntervalTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IntervalTime, 78);
    lv_obj_set_y(ui_IntervalTime, 16);
    lv_label_set_text(ui_IntervalTime, " ");
    lv_obj_set_style_text_font(ui_IntervalTime, &ui_font_Font14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TaskTime = lv_label_create(ui_firstmodule1);
    lv_obj_set_width(ui_TaskTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_TaskTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_TaskTime, -9);
    lv_obj_set_y(ui_TaskTime, 16);
    lv_label_set_text(ui_TaskTime, " ");
    lv_obj_set_style_text_font(ui_TaskTime, &ui_font_Font14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_secondmodule = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_secondmodule, 80);
    lv_obj_set_height(ui_secondmodule, 60);
    lv_obj_set_x(ui_secondmodule, -7);
    lv_obj_set_y(ui_secondmodule, 150);
    lv_obj_clear_flag(ui_secondmodule, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_secondmodule, lv_color_hex(0x928A8A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_secondmodule, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_secondmodule, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_secondmodule, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_NextRunningTime = lv_label_create(ui_secondmodule);
    lv_obj_set_width(ui_NextRunningTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_NextRunningTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_NextRunningTime, 0);
    lv_obj_set_y(ui_NextRunningTime, 9);
    lv_obj_set_align(ui_NextRunningTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_NextRunningTime, " ");

    ui_nextrunning = lv_label_create(ui_secondmodule);
    lv_obj_set_width(ui_nextrunning, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_nextrunning, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_nextrunning, 0);
    lv_obj_set_y(ui_nextrunning, -15);
    lv_obj_set_align(ui_nextrunning, LV_ALIGN_CENTER);
    lv_label_set_text(ui_nextrunning, "下次喷雾");
    lv_obj_set_style_text_font(ui_nextrunning, &ui_font_Font14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ThirdModule = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_ThirdModule, 120);
    lv_obj_set_height(ui_ThirdModule, 130);
    lv_obj_set_x(ui_ThirdModule, 170);
    lv_obj_set_y(ui_ThirdModule, 80);
    lv_obj_clear_flag(ui_ThirdModule, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ThirdModule, lv_color_hex(0x928A8A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ThirdModule, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ThirdModule, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ThirdModule, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ThirdLabel = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_ThirdLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ThirdLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ThirdLabel, 21);
    lv_obj_set_y(ui_ThirdLabel, -10);
    lv_label_set_text(ui_ThirdLabel, "剩余剂量");
    lv_obj_set_style_text_font(ui_ThirdLabel, &ui_font_Font14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Percent1 = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_Percent1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Percent1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Percent1, -37);
    lv_obj_set_y(ui_Percent1, -52);
    lv_obj_set_align(ui_Percent1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Percent1, "	");
    lv_obj_set_style_text_font(ui_Percent1, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider1 = lv_slider_create(ui_ThirdModule);
    lv_slider_set_value(ui_Slider1, 0, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider1) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider1, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider1, 80);
    lv_obj_set_height(ui_Slider1, 85);
    lv_obj_set_x(ui_Slider1, -1);
    lv_obj_set_y(ui_Slider1, 15);
    lv_obj_set_align(ui_Slider1, LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_Slider1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0xC4EDF8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Slider1, lv_color_hex(0xD4E6EC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Slider1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Slider1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Slider1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_Slider1, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0x589FF3), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider1, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Slider1, lv_color_hex(0xF51818), LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_Percent = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_Percent, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Percent, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Percent, 55);
    lv_obj_set_y(ui_Percent, 56);
    lv_label_set_text(ui_Percent, "	");
    lv_obj_set_style_text_color(ui_Percent, lv_color_hex(0x1172ED), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Percent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Percent, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PercentVal = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_PercentVal, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PercentVal, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_PercentVal, 22);
    lv_obj_set_y(ui_PercentVal, 47);
    lv_label_set_text(ui_PercentVal, "00");
    lv_obj_set_style_text_color(ui_PercentVal, lv_color_hex(0x1172ED), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_PercentVal, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_PercentVal, &ui_font_Font26, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_add_event_cb(ui_PercentVal, surplus_change_cb, LV_EVENT_VALUE_CHANGED , NULL); 

    ui_FourthModule = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_FourthModule, 80);
    lv_obj_set_height(ui_FourthModule, 60);
    lv_obj_set_x(ui_FourthModule, 77);
    lv_obj_set_y(ui_FourthModule, 150);
    lv_obj_clear_flag(ui_FourthModule, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_FourthModule, lv_color_hex(0x928A8A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_FourthModule, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_FourthModule, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_FourthModule, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_duration = lv_label_create(ui_FourthModule);
    lv_obj_set_width(ui_duration, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_duration, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_duration, 0);
    lv_obj_set_y(ui_duration, -15);
    lv_obj_set_align(ui_duration, LV_ALIGN_CENTER);
    lv_label_set_text(ui_duration, "时长");
    lv_obj_set_style_text_font(ui_duration, &ui_font_Font14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DurationTime = lv_label_create(ui_FourthModule);
    lv_obj_set_width(ui_DurationTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_DurationTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_DurationTime, 0);
    lv_obj_set_y(ui_DurationTime, 9);
    lv_obj_set_align(ui_DurationTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DurationTime, "");

   // lv_obj_add_event_cb(ui_Screen1, ui_event_Screen1, LV_EVENT_ALL, NULL);

}

void qc_init(void)
{

	qc_module = lv_obj_create(lv_scr_act());
	//qc_module = lv_obj_create(NULL);
    lv_obj_clear_flag(qc_module, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_width(qc_module, 320);
    lv_obj_set_height(qc_module, 240);
    lv_obj_set_x(qc_module,0);
    lv_obj_set_y(qc_module,0);
    lv_obj_set_style_radius(qc_module, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(qc_module, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(qc_module, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *qc_disp;
    qc_disp=lv_qrcode_create(qc_module,190,lv_color_make(0xff,0xff,0xff),lv_color_make(0x00,0x00,0x00));
	lv_qrcode_update(qc_disp,BC260_Massage.BC260_SN,strlen(BC260_Massage.BC260_SN));
    lv_obj_center(qc_disp);

    //lv_obj_add_event_cb(qc_module,ui_event_Screen2,LV_EVENT_ALL,NULL);

    lv_obj_add_event_cb(qc_module,qc_event_cb,LV_EVENT_CLICKED,NULL);
}

void ui_init(void)
{
//    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    //qc_init();
    //ui____initial_actions0 = lv_obj_create(lv_scr_act());
    //lv_disp_load_scr(ui_Screen1);
}
