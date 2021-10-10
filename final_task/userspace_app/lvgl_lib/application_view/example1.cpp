#include "example.h"
#include <lvgl.h>

namespace Lvgl::Example {


static void scroll_event_cb(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        } else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

static void message_box_callback(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    //lv_obj_allocate_spec_attr()
    LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
}

static void create_messagbox_for_label(lv_obj_t* object)
{
    static const char * btns[] ={"OK",""};
    char* label_text = lv_label_get_text(object);
    lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Notification", label_text, btns, true);
    lv_obj_center(mbox1);
}
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        lv_obj_t* buttonLabel = lv_obj_get_child(btn,0);
        create_messagbox_for_label(buttonLabel);
    }
}

void createSimpleWidgetsExample()
{


    lv_obj_t * cont = lv_obj_create(lv_scr_act());
        lv_obj_set_size(cont, 240, 240);
        lv_obj_center(cont);
        lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
        lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL);
        lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_clip_corner(cont, true, 0);
        lv_obj_set_scroll_dir(cont, LV_DIR_VER);
        lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);
        lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

        uint32_t i;
        for(i = 0; i < 20; i++) {
            lv_obj_t * btn = lv_btn_create(cont);
            lv_obj_set_size(btn, LV_PCT(100), LV_SIZE_CONTENT);
            lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * label = lv_label_create(btn);
            lv_label_set_text_fmt(label, "Button %d", i);
        }

        /*Update the buttons position manually for first*/
        lv_event_send(cont, LV_EVENT_SCROLL, NULL);

        /*Be sure the fist button is in the middle*/
        lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF);


}
}
