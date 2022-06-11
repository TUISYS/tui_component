#include "tui.h"

extern tui_obj_t * tui_com_calendar_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[]);
extern tui_obj_t * tui_com_clock_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[]);
extern tui_obj_t * tui_com_dial_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[]);
extern tui_obj_t * tui_com_volti_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[]);
extern tui_obj_t * tui_com_chart_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[]);
extern tui_obj_t * tui_com_msgbox_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[]);


char *component_reg_cb_map[] = {
	"calendar", (char*)tui_com_calendar_create_json,
	"clock", (char*)tui_com_clock_create_json,
	"dial", (char*)tui_com_dial_create_json,
	"volti", (char*)tui_com_volti_create_json,
	"chart", (char*)tui_com_chart_create_json,
	"msgbox", (char*)tui_com_msgbox_create_json,

	//add component reg cb

	"", ""/* end */
};



void tui_com_reg_func_map(void)
{
	tui_view_reg_func(component_reg_cb_map);
}


