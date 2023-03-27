#ifndef __UI_H
#define __UI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "lvgl.h"
#include "lv_font.h"

//extern const osThreadAttr guiTask_attributes;
enum UI_EVENT   //UI 事件
{
    normal=0,       //正常显示
    signal,          //信号图标刷新 
    bind,           //绑定状态改变
    run_static,      //运行状态
    NextRunning_Reflash,  //距离下一次喷雾时间更新
    surplus_change  //剩余量改变
    
    
};
extern enum UI_EVENT ui_event_cb;

/* Exported functions --------------------------------------------------------*/
void qc_andisplay(uint8_t val);
void qc_event_cb(lv_event_t * e);
void LableWifi_event_cb(lv_event_t * e);
void runningswitch_event_cb(lv_event_t * e);
void NextRunningBar_event_cb(lv_event_t * e);
void surplus_change_cb(lv_event_t * e);
#ifdef __cplusplus
}
#endif

#endif /* __UI_H */
