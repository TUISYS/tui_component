#ifndef __TUI_COMPONENT_H__
#define __TUI_COMPONENT_H__

#define TUI_COMPONENT_ENABEL

static void* tui_com_get_com_attri(tui_obj_t *com_dial)
{
	if (com_dial) {
		tui_container_attri_t attri_root = { 0 };

		tui_container_get_attri(com_dial, &attri_root);

		return  attri_root.attri_com;
	} else {
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
 *  为了让UIStudio知道，需要注册组件
 *------------------------*/
void tui_com_reg_func_map(void);

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

	char *bg_img_path;                      /* 刻度表盘背景图片路径 */
	tui_point_t bg_img_pt;                  /* 刻度表盘背景图片所在的坐标x，y */

	char *point_img_path;                   /* 刻度表盘指针图片路径 */
	tui_point_t point_img_pt;               /* 刻度表盘指针图片所在的坐标x，y */
	tui_point_t point_img_rotate_pt;        /* 刻度表盘指针图片的旋转坐标点x，y */

	tui_point_t txt_pt;                     /* 刻度表盘指针值的label位置 */
	int32_t txt_fnt_size;                   /* 刻度表盘指针值的label字体大小 */
	uint32_t txt_fnt_color;                 /* 刻度表盘指针值的label字体颜色 */
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
	/* 时钟表盘回调函数，返回当前值 */
	tui_com_clock_cb_t cb;

	char *bg_img_path;                      /* 时钟表盘背景图片路径 */
	tui_point_t bg_img_pt;                  /* 时钟表盘背景图片所在的坐标x，y */

	char *point_h_img_path;                 /* 时钟表盘`时`指针图片路径 */
	tui_point_t point_h_img_pt;             /* 时钟表盘`时`指针图片所在的坐标x，y */
	tui_point_t point_h_img_rotate_pt;      /* 时钟表盘`时`指针图片的旋转坐标点x，y */

	char *point_m_img_path;                 /* 时钟表盘`分`指针图片路径 */
	tui_point_t point_m_img_pt;             /* 时钟表盘`分`指针图片所在的坐标x，y */
	tui_point_t point_m_img_rotate_pt;      /* 时钟表盘`分`指针图片的旋转坐标点x，y */

	char *point_s_img_path;                 /* 时钟表盘`秒`指针图片路径 */
	tui_point_t point_s_img_pt;             /* 时钟表盘`秒`指针图片所在的坐标x，y */
	tui_point_t point_s_img_rotate_pt;      /* 时钟表盘`秒`指针图片的旋转坐标点x，y */

} tui_com_clock_attri_t;
tui_obj_t * tui_com_clock_create(tui_obj_t * par);
int tui_com_clock_set_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri);
int tui_com_clock_get_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri);

/*------------------------
 *  chart图表折线
 *------------------------*/
typedef void(*tui_com_chart_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *bg_chart_obj;
	tui_obj_t *chart_line_obj;
	tui_obj_t **chart_cursor_obj;
	tui_point_t *point_array;
	/* 图表折线回调函数，返回当前值 */
	tui_com_chart_cb_t cb;
	
	uint16_t point_num;                     /* 图表的点的数量 */
	bool is_bezier;                         /* 图表的显示方式，折线或者曲线 */
	uint16_t mode;                          /* 0：线、1：柱形*/
	bool is_anim;                           /* 是否动画显示 */
} tui_com_chart_attri_t;
tui_obj_t * tui_com_chart_create(tui_obj_t * par);
int tui_com_chart_set_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri);
int tui_com_chart_get_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri);
void tui_com_chart_set_bezier(tui_obj_t *com_chart, bool is_bezier);
void tui_com_chart_set_point(tui_obj_t *com_chart, int32_t index, int32_t value);/* 0~1000 */

/*------------------------
 *  calendar日历
 *------------------------*/
extern const char * tui_com_calendar_get_lunar_calendar(int year, int month, int day, int *is_jieri, char *get_nl_string);
typedef void(*tui_com_calendar_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t *year_dropdown_bnt;
	tui_obj_t *year_txt;
	tui_obj_t *month_dropdown_bnt;
	tui_obj_t *month_txt;
	tui_obj_t *yl_year_text_obj;
	tui_obj_t *nl_year_text_obj;
	tui_obj_t *year_line_obj;
	tui_obj_t *week_text_obj[7];
	tui_obj_t *day_text_obj[42]; /* [6][7] */
	tui_obj_t *nl_text_obj[42]; /* [6][7] */
	uint32_t   yymmdd[42]; /* [6][7] */
	tui_time_t set_time;
	tui_timer_t *ref_timer;
	/* 日历回调函数，返回当前值 */
	tui_com_calendar_cb_t cb;

	char week_str[7][10];                  /* 星期的字符：一、二、三、四、五、六、日 */
	char year_str[16];                      /* 上一个的字符 */
	char month_str[16];                     /* 下一个的字符 */
} tui_com_calendar_attri_t;
tui_obj_t * tui_com_calendar_create(tui_obj_t * par);
int tui_com_calendar_set_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri);
int tui_com_calendar_get_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri);
void tui_com_calendar_show_year_month(tui_obj_t *com_calendar, int year, int month, int day);

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
	/* 翻页动画回调函数，返回当前值 */
	tui_com_volti_cb_t cb;
	
	uint8_t cur_num;                      /* 当前的数值 */
	char up_num_img_path[10][32];         /* 数字图片的上部分图片路径 */
	tui_point_t up_num_img_pt;            /* 数字图片的上部分图片坐标位置x，y */
	char down_num_img_path[10][32];       /* 数字图片的下部分图片路径 */
	tui_point_t down_num_img_pt;          /* 数字图片的下部分图片坐标位置x，y */
} tui_com_volti_attri_t;
tui_obj_t * tui_com_volti_create(tui_obj_t * par);
int tui_com_volti_set_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri);
int tui_com_volti_get_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri);
void tui_com_volti_set_num(tui_obj_t *com_volti, uint8_t num);

/*------------------------
*  msgbox消息框
*------------------------*/
typedef void(*tui_com_msgbox_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	tui_obj_t * par;
	tui_obj_t * top_cont;
	tui_obj_t *msg_txt;
	tui_obj_t *yes_bnt;
	tui_obj_t *yes_txt;
	tui_obj_t *no_bnt;
	tui_obj_t *no_txt;
	tui_obj_t *ok_bnt;
	tui_obj_t *ok_txt;
	tui_timer_t *del_timer;
	/* 消息框回调函数，返回当前值 */
	tui_com_msgbox_cb_t cb;
	
	uint8_t btn_num;                      /* 2 = yes no; 1 = ok; 0 = no btn*/

	char *msg_str;                        /* 消息框的内容字符 */

	char *yes_str;                        /* 消息框的确定按钮的字符 */
	char *no_str;                         /* 消息框的取消按钮的字符 */

	char *ok_str;                         /* 消息框的提示确定按钮的字符 */
} tui_com_msgbox_attri_t;
tui_obj_t * tui_com_msgbox_create(tui_obj_t * par);
int tui_com_msgbox_set_attri(tui_obj_t *com_msgbox, tui_com_msgbox_attri_t *attri);
int tui_com_msgbox_get_attri(tui_obj_t *com_msgbox, tui_com_msgbox_attri_t *attri);
void tui_com_msgbox_show_or_hide(tui_obj_t *com_msgbox, bool show_able);

/*------------------------
 *  template模板组件
 *------------------------*/
typedef void(*tui_com_template_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* 通用属性 */
	tui_object_attri_t obj;
	/* 模板组件回调函数，返回当前值 */
	tui_com_template_cb_t cb;
} tui_com_template_attri_t;
tui_obj_t * tui_com_template_create(tui_obj_t * par);
int tui_com_template_set_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri);
int tui_com_template_get_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri);

#endif /* __TUI_COMPONENT_H__ */
