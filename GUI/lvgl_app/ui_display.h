// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.3.3
// PROJECT: sample

#ifndef _UI_DISLPAY_H
#define _UI_DISLPAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "lv_font.h"
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * qc_module ;
extern lv_obj_t * ui_BG;
extern lv_obj_t * ui_LableWiFi;
//extern lv_obj_t * ui_LableWiFi3;
//extern lv_obj_t * ui_LableWiFi2;
//extern lv_obj_t * ui_LableWiFi1;
extern lv_obj_t * ui_LableTime;
extern lv_obj_t * ui_LableData;
extern lv_obj_t * ui_LableWeek;
extern lv_obj_t * ui_firstmodule1;
extern lv_obj_t * ui_loadingLogo;
extern lv_obj_t * ui_Task;
extern lv_obj_t * ui_IntervalTime;
extern lv_obj_t * ui_TaskTime;

extern lv_obj_t * ui_secondmodule;
extern lv_obj_t * ui_NextRunningTime;
extern lv_obj_t * ui_nextrunning;

extern lv_obj_t * ui_ThirdModule;
extern lv_obj_t * ui_PercentVal;
extern lv_obj_t * ui_Percent;
extern lv_obj_t * ui_Slider1;
extern lv_obj_t * ui_ThirdLabel;
extern lv_obj_t * ui_Percent1;
extern lv_obj_t * ui_FourthModule;
extern lv_obj_t * ui_duration;
extern lv_obj_t * ui_DurationTime;
extern lv_obj_t * ui_Panel2;
extern lv_obj_t * ui____initial_actions0;

extern lv_obj_t * ui_Screen2;

void Change_Task_Disp(void);
void NextRunning_Disp(void);

#define USER_WIFI1            "\xEE\x9B\xA7" /*59111, 0xE6E7*/
#define USER_WIFI2            "\xEE\x9B\xAB" /*59115, 0xE6EB*/
#define USER_WIFI3            "\xEE\x9B\xB9" /*59129, 0xE6F9*/
#define USER_PERCENT          "\xEE\x9D\x8E" /*59214, 0xE74E*/
#define USER_PERCENT1         "\xEE\x98\xAB" /*58923, 0xE62B*/
#define USER_LOADING          "\xEE\x98\x9B" /*58907, 0xE61B*/
#define USER_MistSpray        "\xEE\x98\xAC" /*58924, 0xE62C*/

void ui_init(void);
void qc_init(void);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
