#ifndef __TUI_COMPONENT_H__
#define __TUI_COMPONENT_H__

static void* tui_com_get_com_attri(tui_obj_t *com_dial)
{
	if (com_dial) {
		tui_container_attri_t attri_root = { 0 };

		tui_container_get_attri(com_dial, &attri_root);

		return  attri_root.attri_com;
	}
	else {
		return NULL;
	}
}

static void * tui_com_get_func(uint32_t obj_id, tui_map_cb_t map_cb[])
{
	int32_t i = 0;

	if (map_cb == NULL)
		return NULL;

	while (map_cb[i].func) {
		if (map_cb[i].id == obj_id)
			return map_cb[i].func;
		i++;
	}
	return NULL;
}

/*------------------------
 *  dial刻度表盘
 *------------------------*/
typedef void(*tui_com_dial_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *bg_img_obj;
	tui_obj_t *point_img_obj;
	tui_obj_t *txt_obj;
	/* 刻度表盘回调函数，返回当前值 */
	tui_com_dial_cb_t cb;

	char *bg_img_path;
	tui_point_t bg_img_pt;

	char *point_img_path;
	tui_point_t point_img_pt;
	tui_point_t point_img_rotate_pt;

	char txt[12];
	tui_point_t txt_pt;
	int32_t txt_fnt_size;
	uint32_t txt_fnt_color;
} tui_com_dial_attri_t;
tui_obj_t * tui_com_dial_create(tui_obj_t * par);
int tui_com_dial_set_attri(tui_obj_t *com_dial, tui_com_dial_attri_t *attri);
int tui_com_dial_get_attri(tui_obj_t *com_dial, tui_com_dial_attri_t *attri);
void tui_com_dial_set_point_angle(tui_obj_t *com_dial, int value/* 0~100 */);

/*------------------------
 *  clock时钟表盘
 *------------------------*/
typedef void(*tui_com_clock_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t sec);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *bg_img_obj;
	tui_obj_t *point_h_img_obj;
	tui_obj_t *point_m_img_obj;
	tui_obj_t *point_s_img_obj;
	tui_timer_t *clock_timer;
	/* 刻度表盘回调函数，返回当前值 */
	tui_com_clock_cb_t cb;

	char *bg_img_path;
	tui_point_t bg_img_pt;

	char *point_h_img_path;
	tui_point_t point_h_img_pt;
	tui_point_t point_h_img_rotate_pt;

	char *point_m_img_path;
	tui_point_t point_m_img_pt;
	tui_point_t point_m_img_rotate_pt;

	char *point_s_img_path;
	tui_point_t point_s_img_pt;
	tui_point_t point_s_img_rotate_pt;

} tui_com_clock_attri_t;
tui_obj_t * tui_com_clock_create(tui_obj_t * par);
int tui_com_clock_set_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri);
int tui_com_clock_get_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri);

/*------------------------
 *  chart图标曲线
 *------------------------*/
typedef void(*tui_com_chart_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *bg_chart_obj;
	tui_obj_t *chart_line_obj;
	tui_obj_t **chart_cursor_obj;
	tui_point_t *point_array;
	/* 刻度表盘回调函数，返回当前值 */
	tui_com_chart_cb_t cb;
	
	uint16_t point_num;

} tui_com_chart_attri_t;
tui_obj_t * tui_com_chart_create(tui_obj_t * par);
int tui_com_chart_set_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri);
int tui_com_chart_get_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri);
void tui_com_chart_set_point(tui_obj_t *com_chart, int32_t index, int32_t value);

/*------------------------
 *  calendar日历
 *------------------------*/
typedef void(*tui_com_calendar_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *pre_month_bnt;
	tui_obj_t *pre_month_txt;
	tui_obj_t *next_month_bnt;
	tui_obj_t *next_month_txt;
	tui_obj_t *year_text_obj;
	tui_obj_t *year_line_obj;
	tui_obj_t *week_text_obj[7];
	tui_obj_t *day_text_obj[42]; /* [6][7] */
	tui_time_t set_time;
	tui_timer_t *ref_timer;
	/* 刻度表盘回调函数，返回当前值 */
	tui_com_calendar_cb_t cb;

	char week_str[7][10];
	char pre_str[16];
	char next_str[16];
} tui_com_calendar_attri_t;
tui_obj_t * tui_com_calendar_create(tui_obj_t * par);
int tui_com_calendar_set_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri);
int tui_com_calendar_get_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri);
void tui_com_calendar_show_year_month(tui_obj_t *com_calendar, int year, int month);

/*------------------------
*  volti翻页动画
*------------------------*/
typedef void(*tui_com_volti_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *img_old_up;
	tui_obj_t *img_old_down;
	tui_obj_t *img_new_up;
	tui_obj_t *img_new_down;
	tui_coord_t height_d;
	/* 刻度表盘回调函数，返回当前值 */
	tui_com_volti_cb_t cb;
	
	uint8_t cur_num;
	char up_num_img_path[10][32];
	tui_point_t up_num_img_pt;
	char down_num_img_path[10][32];
	tui_point_t down_num_img_pt;
} tui_com_volti_attri_t;
tui_obj_t * tui_com_volti_create(tui_obj_t * par);
int tui_com_volti_set_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri);
int tui_com_volti_get_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri);
void tui_com_volti_set_num(tui_obj_t *com_volti, uint8_t num);

/*------------------------
 *  template模板组件
 *------------------------*/
typedef void(*tui_com_template_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	/* 刻度表盘回调函数，返回当前值 */
	tui_com_template_cb_t cb;
} tui_com_template_attri_t;
tui_obj_t * tui_com_template_create(tui_obj_t * par);
int tui_com_template_set_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri);
int tui_com_template_get_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri);

#endif /* __TUI_COMPONENT_H__ */
