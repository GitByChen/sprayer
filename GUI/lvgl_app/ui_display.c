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

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * ui_BG;
lv_obj_t * ui_LableWiFi;
//lv_obj_t * ui_LableWiFi3;
//lv_obj_t * ui_LableWiFi2;
//lv_obj_t * ui_LableWiFi1;
lv_obj_t * ui_LableTime;
lv_obj_t * ui_LableData;
lv_obj_t * ui_LableWeek;
lv_obj_t * ui_firstmodule1;
lv_obj_t * ui_loading;
lv_obj_t * ui_runningstatic;
lv_obj_t * ui_runningswitch;
lv_obj_t * ui_secondmodule;
lv_obj_t * ui_nextrunning1;
lv_obj_t * ui_NextRunningBar;
lv_obj_t * ui_LableMistSpray;
lv_obj_t * ui_ThirdModule;
lv_obj_t * ui_PercentVal;
lv_obj_t * ui_Percent;
lv_obj_t * ui_Slider2;
lv_obj_t * ui_ThirdLabel;
lv_obj_t * ui_Percent1;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
LV_IMG_DECLARE(ui_img_background_png);    // assets\background.png
LV_IMG_DECLARE(ui_img_buttle2_png);    // assets\buttle2.png
LV_IMG_DECLARE(ui_img_buttle1_png);    // assets\buttle1.png


LV_FONT_DECLARE(ui_font_confont16);
LV_FONT_DECLARE(ui_font_Font16);
LV_FONT_DECLARE(ui_font_Font26);
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


void nextrunning_val(int val)
{
		lv_event_send(ui_PercentVal, LV_EVENT_VALUE_CHANGED, &val);
   num_val=val;
}

void DisplayWifi(int val)
{
    switch(val)
    {
        case 0:
           lv_label_set_text(ui_LableWiFi, USER_WIFI1);
        break;
        case 1:
            lv_label_set_text(ui_LableWiFi, USER_WIFI2);
        break;
        case 2:
            lv_label_set_text(ui_LableWiFi, USER_WIFI3);
        break;
    }
}

void DisplaySwitch(int val)
{
    if(val)

         lv_obj_add_state(ui_runningswitch, LV_STATE_CHECKED);
    else
        lv_obj_clear_state(ui_runningswitch, LV_STATE_CHECKED);
}

static void TimeReflash_cb(lv_event_t *e)
{
	//LV_UNUSED(e);
		  lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        //(e);
		 lv_bar_set_value(ui_NextRunningBar, num_val, LV_ANIM_OFF);
	   lv_slider_set_value(ui_Slider2, num_val, LV_ANIM_OFF);
		lv_label_set_text_fmt(ui_PercentVal,"%02d",num_val);
    }
}


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
 
///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_BG = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_BG, &ui_img_background_png);
    lv_obj_set_width(ui_BG, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BG, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BG, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BG, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_BG, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_angle(ui_BG, 2);
    lv_img_set_zoom(ui_BG, 2560);

    ui_LableWiFi = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableWiFi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableWiFi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableWiFi, 280);
    lv_obj_set_y(ui_LableWiFi, 4);
    lv_label_set_text(ui_LableWiFi, USER_WIFI3);//"	"
    lv_obj_set_style_text_font(ui_LableWiFi, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_obj_set_x(ui_LableTime, 4);
    lv_obj_set_y(ui_LableTime, 4);
    lv_label_set_text(ui_LableTime, "00:00:00");
    lv_obj_set_style_text_font(ui_LableTime, &ui_font_Font16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LableData = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableData, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableData, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableData, 85);
    lv_obj_set_y(ui_LableData, 30);
    lv_label_set_text(ui_LableData, "0000/00/00");
    lv_obj_set_style_text_font(ui_LableData, &ui_font_Font26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LableWeek = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LableWeek, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableWeek, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableWeek, 110);
    lv_obj_set_y(ui_LableWeek, 60);
    lv_label_set_text(ui_LableWeek, "星期一");
    lv_obj_set_style_text_font(ui_LableWeek, &ui_font_Font26, LV_PART_MAIN | LV_STATE_DEFAULT);
						
    lv_timer_t* task_timer = lv_timer_create(clock_date_task_callback, 1000, (void *)&ui_LableTime); // 创建定时任务，200ms刷新一次

		
    ui_firstmodule1 = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_firstmodule1, 120);
    lv_obj_set_height(ui_firstmodule1, 60);
    lv_obj_set_x(ui_firstmodule1, 10);
    lv_obj_set_y(ui_firstmodule1, 100);
    lv_obj_clear_flag(ui_firstmodule1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_firstmodule1, lv_color_hex(0x292C31), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_firstmodule1, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_firstmodule1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_firstmodule1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_loading = lv_label_create(ui_firstmodule1);
    lv_obj_set_width(ui_loading, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_loading, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_loading, -5);
    lv_obj_set_y(ui_loading, -10);
    lv_label_set_text(ui_loading,USER_LOADING);// "	"
    lv_obj_set_style_text_font(ui_loading, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_runningstatic = lv_label_create(ui_firstmodule1);
    lv_obj_set_width(ui_runningstatic, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_runningstatic, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_runningstatic, 19);
    lv_obj_set_y(ui_runningstatic, -8);
    lv_label_set_text(ui_runningstatic, "运行状态");
    lv_obj_set_style_text_font(ui_runningstatic, &ui_font_Font16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_runningswitch = lv_switch_create(ui_firstmodule1);
    lv_obj_set_width(ui_runningswitch, 50);
    lv_obj_set_height(ui_runningswitch, 20);
    lv_obj_set_x(ui_runningswitch, 19);
    lv_obj_set_y(ui_runningswitch, 16);
    lv_obj_set_style_bg_color(ui_runningswitch, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_runningswitch, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_secondmodule = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_secondmodule, 120);
    lv_obj_set_height(ui_secondmodule, 60);
    lv_obj_set_x(ui_secondmodule, 10);
    lv_obj_set_y(ui_secondmodule, 170);
    lv_obj_clear_flag(ui_secondmodule, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_secondmodule, lv_color_hex(0x292C31), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_secondmodule, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_secondmodule, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_secondmodule, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_nextrunning1 = lv_label_create(ui_secondmodule);
    lv_obj_set_width(ui_nextrunning1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_nextrunning1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_nextrunning1, 4);
    lv_obj_set_y(ui_nextrunning1, -15);
    lv_obj_set_align(ui_nextrunning1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_nextrunning1, "下次喷雾");
    lv_obj_set_style_text_font(ui_nextrunning1, &ui_font_Font16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_NextRunningBar = lv_bar_create(ui_secondmodule);
    lv_bar_set_value(ui_NextRunningBar, 25, LV_ANIM_OFF);
    lv_obj_set_width(ui_NextRunningBar, 100);
    lv_obj_set_height(ui_NextRunningBar, 10);
    lv_obj_set_x(ui_NextRunningBar, -6);
    lv_obj_set_y(ui_NextRunningBar, 21);
    lv_obj_set_style_bg_color(ui_NextRunningBar, lv_color_hex(0xD2D2D2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_NextRunningBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LableMistSpray = lv_label_create(ui_secondmodule);
    lv_obj_set_width(ui_LableMistSpray, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LableMistSpray, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LableMistSpray, -4);
    lv_obj_set_y(ui_LableMistSpray, -10);
    lv_label_set_text(ui_LableMistSpray,USER_MistSpray );//"	"
    lv_obj_set_style_text_font(ui_LableMistSpray, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ThirdModule = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_ThirdModule, 120);
    lv_obj_set_height(ui_ThirdModule, 130);
    lv_obj_set_x(ui_ThirdModule, 170);
    lv_obj_set_y(ui_ThirdModule, 100);
    lv_obj_clear_flag(ui_ThirdModule, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ThirdModule, lv_color_hex(0x292C31), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ThirdModule, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ThirdModule, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ThirdModule, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PercentVal = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_PercentVal, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PercentVal, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_PercentVal, 25);
    lv_obj_set_y(ui_PercentVal, 93);
    lv_label_set_text(ui_PercentVal, "1");
    lv_obj_set_style_text_font(ui_PercentVal, &ui_font_Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_add_event_cb(ui_PercentVal, TimeReflash_cb, LV_EVENT_VALUE_CHANGED , NULL); 
	
    ui_Percent = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_Percent, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Percent, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Percent, 45);
    lv_obj_set_y(ui_Percent, 95);
    lv_label_set_text(ui_Percent,USER_PERCENT );//"	"
    lv_obj_set_style_text_font(ui_Percent, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider2 = lv_slider_create(ui_ThirdModule);											//图片占 25K rom
    lv_slider_set_value(ui_Slider2, 50, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider2) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider2, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider2, 40);
    lv_obj_set_height(ui_Slider2, 80);
    lv_obj_set_x(ui_Slider2, 23);
    lv_obj_set_y(ui_Slider2, 10);
    lv_obj_set_style_radius(ui_Slider2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Slider2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_Slider2, &ui_img_buttle2_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_Slider2, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Slider2, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider2, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_Slider2, &ui_img_buttle1_png, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Slider2, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider2, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_ThirdLabel = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_ThirdLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ThirdLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ThirdLabel, 13);
    lv_obj_set_y(ui_ThirdLabel, -10);
    lv_label_set_text(ui_ThirdLabel, "剩余剂量");
    lv_obj_set_style_text_font(ui_ThirdLabel, &ui_font_Font16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Percent1 = lv_label_create(ui_ThirdModule);
    lv_obj_set_width(ui_Percent1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Percent1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Percent1, -43);
    lv_obj_set_y(ui_Percent1, -52);
    lv_obj_set_align(ui_Percent1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Percent1,USER_PERCENT1 );//"	"
    lv_obj_set_style_text_font(ui_Percent1, &ui_font_confont16, LV_PART_MAIN | LV_STATE_DEFAULT);


}

void ui_init(void)
{
//    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    lv_disp_load_scr(ui_Screen1);
}
