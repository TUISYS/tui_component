#ifndef __TUI_COMPONENT_H__
#define __TUI_COMPONENT_H__

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
 *  dial�̶ȱ���
 *------------------------*/
typedef void(*tui_com_dial_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* ͨ������ */
	tui_object_attri_t obj;
	tui_obj_t *bg_img_obj;
	tui_obj_t *point_img_obj;
	tui_obj_t *txt_obj;
	/* �̶ȱ��̻ص����������ص�ǰֵ */
	tui_com_dial_cb_t cb;

	char *bg_img_path;                      /* �̶ȱ��̱���ͼƬ·�� */
	tui_point_t bg_img_pt;                  /* �̶ȱ��̱���ͼƬ���ڵ�����x��y */

	char *point_img_path;                   /* �̶ȱ���ָ��ͼƬ·�� */
	tui_point_t point_img_pt;               /* �̶ȱ���ָ��ͼƬ���ڵ�����x��y */
	tui_point_t point_img_rotate_pt;        /* �̶ȱ���ָ��ͼƬ����ת�����x��y */

	char txt[12];                           /* �̶ȱ���ָ���ֵ0~100 */
	tui_point_t txt_pt;                     /* �̶ȱ���ָ��ֵ��labelλ�� */
	int32_t txt_fnt_size;                   /* �̶ȱ���ָ��ֵ��label�����С */
	uint32_t txt_fnt_color;                 /* �̶ȱ���ָ��ֵ��label������ɫ */
} tui_com_dial_attri_t;
tui_obj_t * tui_com_dial_create(tui_obj_t * par);
int tui_com_dial_set_attri(tui_obj_t *com_dial, tui_com_dial_attri_t *attri);
int tui_com_dial_get_attri(tui_obj_t *com_dial, tui_com_dial_attri_t *attri);
void tui_com_dial_set_point_angle(tui_obj_t *com_dial, int value/* 0~100 */);

/*------------------------
 *  clockʱ�ӱ���
 *------------------------*/
typedef void(*tui_com_clock_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t sec);
typedef struct {
	/* ͨ������ */
	tui_object_attri_t obj;
	tui_obj_t *bg_img_obj;
	tui_obj_t *point_h_img_obj;
	tui_obj_t *point_m_img_obj;
	tui_obj_t *point_s_img_obj;
	tui_timer_t *clock_timer;
	/* ʱ�ӱ��̻ص����������ص�ǰֵ */
	tui_com_clock_cb_t cb;

	char *bg_img_path;                      /* ʱ�ӱ��̱���ͼƬ·�� */
	tui_point_t bg_img_pt;                  /* ʱ�ӱ��̱���ͼƬ���ڵ�����x��y */

	char *point_h_img_path;                 /* ʱ�ӱ���`ʱ`ָ��ͼƬ·�� */
	tui_point_t point_h_img_pt;             /* ʱ�ӱ���`ʱ`ָ��ͼƬ���ڵ�����x��y */
	tui_point_t point_h_img_rotate_pt;      /* ʱ�ӱ���`ʱ`ָ��ͼƬ����ת�����x��y */

	char *point_m_img_path;                 /* ʱ�ӱ���`��`ָ��ͼƬ·�� */
	tui_point_t point_m_img_pt;             /* ʱ�ӱ���`��`ָ��ͼƬ���ڵ�����x��y */
	tui_point_t point_m_img_rotate_pt;      /* ʱ�ӱ���`��`ָ��ͼƬ����ת�����x��y */

	char *point_s_img_path;                 /* ʱ�ӱ���`��`ָ��ͼƬ·�� */
	tui_point_t point_s_img_pt;             /* ʱ�ӱ���`��`ָ��ͼƬ���ڵ�����x��y */
	tui_point_t point_s_img_rotate_pt;      /* ʱ�ӱ���`��`ָ��ͼƬ����ת�����x��y */

} tui_com_clock_attri_t;
tui_obj_t * tui_com_clock_create(tui_obj_t * par);
int tui_com_clock_set_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri);
int tui_com_clock_get_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri);

/*------------------------
 *  chartͼ������
 *------------------------*/
typedef void(*tui_com_chart_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* ͨ������ */
	tui_object_attri_t obj;
	tui_obj_t *bg_chart_obj;
	tui_obj_t *chart_line_obj;
	tui_obj_t **chart_cursor_obj;
	tui_point_t *point_array;
	/* ͼ�����߻ص����������ص�ǰֵ */
	tui_com_chart_cb_t cb;
	
	uint16_t point_num;                     /* ͼ��ĵ������ */
	bool is_bezier;                         /* ͼ�����ʾ��ʽ�����߻������� */
} tui_com_chart_attri_t;
tui_obj_t * tui_com_chart_create(tui_obj_t * par);
int tui_com_chart_set_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri);
int tui_com_chart_get_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri);
void tui_com_chart_set_point(tui_obj_t *com_chart, int32_t index, int32_t value);

/*------------------------
 *  calendar����
 *------------------------*/
typedef void(*tui_com_calendar_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* ͨ������ */
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
	/* �����ص����������ص�ǰֵ */
	tui_com_calendar_cb_t cb;

	char week_str[7][10];                  /* ���ڵ��ַ���һ�����������ġ��塢������ */
	char pre_str[16];                      /* ��һ�����ַ� */
	char next_str[16];                     /* ��һ�����ַ� */
} tui_com_calendar_attri_t;
tui_obj_t * tui_com_calendar_create(tui_obj_t * par);
int tui_com_calendar_set_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri);
int tui_com_calendar_get_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri);
void tui_com_calendar_show_year_month(tui_obj_t *com_calendar, int year, int month);

/*------------------------
*  volti��ҳ����
*------------------------*/
typedef void(*tui_com_volti_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* ͨ������ */
	tui_object_attri_t obj;
	tui_obj_t *img_old_up;
	tui_obj_t *img_old_down;
	tui_obj_t *img_new_up;
	tui_obj_t *img_new_down;
	tui_coord_t height_d;
	/* ��ҳ�����ص����������ص�ǰֵ */
	tui_com_volti_cb_t cb;
	
	uint8_t cur_num;                      /* ��ǰ����ֵ */
	char up_num_img_path[10][32];         /* ����ͼƬ���ϲ���ͼƬ·�� */
	tui_point_t up_num_img_pt;            /* ����ͼƬ���ϲ���ͼƬ����λ��x��y */
	char down_num_img_path[10][32];       /* ����ͼƬ���²���ͼƬ·�� */
	tui_point_t down_num_img_pt;          /* ����ͼƬ���²���ͼƬ����λ��x��y */
} tui_com_volti_attri_t;
tui_obj_t * tui_com_volti_create(tui_obj_t * par);
int tui_com_volti_set_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri);
int tui_com_volti_get_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri);
void tui_com_volti_set_num(tui_obj_t *com_volti, uint8_t num);

/*------------------------
*  msgbox��Ϣ��
*------------------------*/
typedef void(*tui_com_msgbox_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* ͨ������ */
	tui_object_attri_t obj;
	tui_obj_t * par;
	tui_obj_t *msg_txt;
	tui_obj_t *yes_bnt;
	tui_obj_t *yes_txt;
	tui_obj_t *no_bnt;
	tui_obj_t *no_txt;
	tui_obj_t *ok_bnt;
	tui_obj_t *ok_txt;
	tui_timer_t *del_timer;
	/* ��Ϣ��ص����������ص�ǰֵ */
	tui_com_msgbox_cb_t cb;
	
	uint8_t btn_num;/* 2 = yes no; 1 = ok; 0 = no btn*/

	char *msg_str;                        /* ��Ϣ���ȷ����ť���ַ� */

	char *yes_str;
	char *no_str;                         /* ��Ϣ���ȡ����ť���ַ� */

	char *ok_str;                         /* ��Ϣ�����ʾȷ����ť���ַ� */
} tui_com_msgbox_attri_t;
tui_obj_t * tui_com_msgbox_create(tui_obj_t * par);
int tui_com_msgbox_set_attri(tui_obj_t *com_msgbox, tui_com_msgbox_attri_t *attri);
int tui_com_msgbox_get_attri(tui_obj_t *com_msgbox, tui_com_msgbox_attri_t *attri);
void tui_com_msgbox_show_or_hide(tui_obj_t *com_msgbox, bool show_able);

/*------------------------
 *  templateģ�����
 *------------------------*/
typedef void(*tui_com_template_cb_t)(tui_obj_t *obj, tui_event_e event, int32_t value);
typedef struct {
	/* ͨ������ */
	tui_object_attri_t obj;
	/* ģ������ص����������ص�ǰֵ */
	tui_com_template_cb_t cb;
} tui_com_template_attri_t;
tui_obj_t * tui_com_template_create(tui_obj_t * par);
int tui_com_template_set_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri);
int tui_com_template_get_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri);

#endif /* __TUI_COMPONENT_H__ */
