#include "example.h"
#include <lvgl.h>

namespace Lvgl::Example {

void createSimpleWidgetsExample()
{

        /*Create a container with COLUMN flex direction*/
        lv_obj_t * cont_col = lv_obj_create(lv_scr_act());
        lv_obj_set_size(cont_col, 200, 150);

        lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);

        uint32_t i;
        for(i = 0; i < 10; i++) {
            lv_obj_t * obj;
            lv_obj_t * label;

            /*Add items to the column*/
            obj = lv_btn_create(cont_col);
            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);

            label = lv_label_create(obj);
            lv_label_set_text_fmt(label, "Item: %d", i);
            lv_obj_center(label);
        }
}
}
