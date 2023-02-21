// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.3.3
// PROJECT: sample


#include "ui.h"
#include "ui_helpers.h"
#include "ui_comp.h"

uint32_t LV_EVENT_GET_COMP_CHILD;

typedef struct {
    uint32_t child_idx;
    lv_obj_t * child;
} ui_comp_get_child_t;

lv_obj_t * ui_comp_get_child(lv_obj_t * comp, uint32_t child_idx)
{
    ui_comp_get_child_t info;
    info.child = NULL;
    info.child_idx = child_idx;
    lv_event_send(comp, LV_EVENT_GET_COMP_CHILD, &info);
    return info.child;
}

void get_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    ui_comp_get_child_t * info = lv_event_get_param(e);
    info->child = c[info->child_idx];
}

void del_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    lv_free(c);
}


// COMPONENT Bottle

lv_obj_t * ui_Bottle_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Bottle;
    cui_Bottle = lv_label_create(comp_parent);
    lv_obj_set_width(cui_Bottle, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_Bottle, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(cui_Bottle, 200);
    lv_obj_set_y(cui_Bottle, 130);
    lv_obj_set_style_text_font(cui_Bottle, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t ** children = lv_malloc(sizeof(lv_obj_t *) * _UI_COMP_BOTTLE_NUM);
    children[UI_COMP_BOTTLE_BOTTLE] = cui_Bottle;
    lv_obj_add_event_cb(cui_Bottle, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Bottle, del_component_child_event_cb, LV_EVENT_DELETE, children);
    return cui_Bottle;
}

