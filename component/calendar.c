#include "tui.h"

#define TUI_COM_CALENDAR_FIRST_ROW_H		70
#define TUI_COM_CALENDAR_SECOND_ROW_H		60

#define TUI_COM_CALENDAR_BTN_TXT_FNT_SIZE	30
#define TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE	25
#define TUI_COM_CALENDAR_WEEK_TXT_FNT_SIZE	25
#define TUI_COM_CALENDAR_DAY_TXT_FNT_SIZE	25
#define TUI_COM_CALENDAR_NL_TXT_FNT_SIZE    16
#define TUI_COM_CALENDAR_DAY_TXT_LINE_SIZE	8

#define TUI_COM_CALENDAR_WEEK_STARTS_MONDAY

#ifdef TUI_COM_CALENDAR_WEEK_STARTS_MONDAY
static const char * day_name[7] = { "\xe4\xb8\x80", "\xe4\xba\x8c", "\xe4\xb8\x89", "\xe5\x9b\x9b", "\xe4\xba\x94", "\xe5\x85\xad", "\xe6\x97\xa5" };// { "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su" };
#else
static const char * day_name[7] = { "\xe6\x97\xa5", "\xe4\xb8\x80", "\xe4\xba\x8c", "\xe4\xb8\x89", "\xe5\x9b\x9b", "\xe4\xba\x94", "\xe5\x85\xad" };// { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };
#endif

/***
 *     农历
 *     年月日
 *     除夕春节元宵清明端午七夕中秋重阳
 *     元旦情人节妇女节植树节消费者权益日愚人节劳动节青年节母亲节儿童节父亲节建党节建军节国庆节万圣节感恩节国家公祭日平安夜圣诞节
 *     初一初二初三初四初五初六初七初八初九初十十一十二十三十四十五十六十七十八十九二十廿一廿二廿三廿四廿五廿六廿七廿八廿九三十
 *     正二三四五六七八九十十一腊
 *     立春雨水惊蛰春分清明谷雨立夏小满芒种夏至小暑大暑立秋处暑白露秋分寒露霜降立冬小雪大雪冬至小寒大寒
 *     鼠牛虎兔龙蛇马羊猴鸡狗猪
 */

/**
 * 闰年计算
 */
static uint8_t tui_com_calendar_is_leap_year(uint32_t year)
{
	return (year % 4) || ((year % 100 == 0) && (year % 400)) ? 0 : 1;
}

/**
 * 月的范围 [1..12]
 * 返回月month的天数
 */
static uint8_t tui_com_calendar_get_month_length(int32_t year, int32_t month)
{
	month--;
	if (month < 0) {
		year--;
		month = 12 + month;
	}
	if (month >= 12) {
		year++;
		month -= 12;
	}

	return (month == 1) ? (28 + tui_com_calendar_is_leap_year(year)) : 31 - month % 7 % 2;
}

/**
 * 月的范围 [1..12]
 * 返回的值是 [0..6]，代表 [Sun..Sat] 或者 [Mon..Sun] 取决于宏定义 TUI_COM_CALENDAR_WEEK_STARTS_MONDAY
 */
static uint8_t tui_com_calendar_get_day_of_week(uint32_t year, uint32_t month, uint32_t day)
{
	uint32_t a = month < 3 ? 1 : 0;
	uint32_t b = year - a;

#ifdef TUI_COM_CALENDAR_WEEK_STARTS_MONDAY
	uint32_t day_of_week = (day + (31 * (month - 2 + 12 * a) / 12) + b + (b / 4) - (b / 100) + (b / 400) - 1) % 7;
#else
	uint32_t day_of_week = (day + (31 * (month - 2 + 12 * a) / 12) + b + (b / 4) - (b / 100) + (b / 400)) % 7;
#endif

	return day_of_week;
}

static void tui_com_calendar_container_cb(tui_obj_t *com_calendar, tui_event_e event)
{
	tui_com_calendar_attri_t *attri_me;

	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		if (attri_me->ref_timer) {
			tui_timer_del(attri_me->ref_timer);
			attri_me->ref_timer = NULL;
		}
		free(attri_me);
	}
}

static void tui_com_calendar_ref_timer_cb(tui_timer_t * timer)
{
	tui_obj_t *com_calendar;
	tui_com_calendar_attri_t *attri_me;
	tui_time_t cur_t = { 0 };

	com_calendar = tui_timer_get_user_data(timer);
	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	cur_t = tui_get_localtime();
	attri_me->set_time = cur_t;

	tui_com_calendar_show_year_month(com_calendar, cur_t.year, cur_t.mon + 1, cur_t.mday);

	if (attri_me->ref_timer) {
		tui_timer_del(attri_me->ref_timer);
		attri_me->ref_timer = NULL;
	}
}

void tui_com_calendar_year_dropdown_cb(tui_obj_t *obj, tui_event_e event, int16_t index)
{
	if (TUI_EVENT_VALUE_CHANGED == event) {
		tui_obj_t *com_calendar;
		tui_com_calendar_attri_t *attri_me;
		tui_time_t cur_t = { 0 };

		cur_t = tui_get_localtime();

		com_calendar = tui_obj_get_parent((const tui_obj_t *)obj);
		if (com_calendar == NULL) {
			printf("tui_com_calendar_next_button_cb_t L%d: faile\n", __LINE__);
			return;
		}
		attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

		attri_me->set_time.year = 1900 + index;
		tui_com_calendar_show_year_month(com_calendar, attri_me->set_time.year, attri_me->set_time.mon + 1, attri_me->set_time.mday);

		if (cur_t.year != attri_me->set_time.year || cur_t.mon != attri_me->set_time.mon) {
			if (attri_me->ref_timer == NULL)
				attri_me->ref_timer = tui_timer_create(tui_com_calendar_ref_timer_cb, 10 * 1000, TUI_TIMER_PRIO_HIGH, com_calendar);
			else
				tui_timer_reset(attri_me->ref_timer);
		}
	}
}

void tui_com_calendar_month_dropdown_cb(tui_obj_t *obj, tui_event_e event, int16_t index)
{
	if (TUI_EVENT_VALUE_CHANGED == event) {
		tui_obj_t *com_calendar;
		tui_com_calendar_attri_t *attri_me;
		tui_time_t cur_t = { 0 };

		cur_t = tui_get_localtime();

		com_calendar = tui_obj_get_parent((const tui_obj_t *)obj);
		if (com_calendar == NULL) {
			printf("tui_com_calendar_next_button_cb_t L%d: faile\n", __LINE__);
			return;
		}
		attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

		attri_me->set_time.mon = index;
		tui_com_calendar_show_year_month(com_calendar, attri_me->set_time.year, attri_me->set_time.mon + 1, attri_me->set_time.mday);

		if (cur_t.year != attri_me->set_time.year || cur_t.mon != attri_me->set_time.mon) {
			if (attri_me->ref_timer == NULL)
				attri_me->ref_timer = tui_timer_create(tui_com_calendar_ref_timer_cb, 10 * 1000, TUI_TIMER_PRIO_HIGH, com_calendar);
			else
				tui_timer_reset(attri_me->ref_timer);
		}
	}
}

static void tui_com_calendar_day_cb_t(tui_obj_t *obj, tui_event_e event)
{
	if (TUI_EVENT_RELEASED == event) {
		tui_obj_t *com_calendar;
		tui_com_calendar_attri_t *attri_me;
		int i = 0;

		com_calendar = tui_obj_get_parent((const tui_obj_t *)obj);
		if (com_calendar == NULL) {
			printf("tui_com_calendar_day_cb_t L%d: faile\n", __LINE__);
			return;
		}
		attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

		for (i = 0; i < 42; i++) {
			if (obj == attri_me->nl_text_obj[i]) {
				tui_com_calendar_show_year_month(com_calendar, (attri_me->yymmdd[i] >> 16) & 0xFFFF, (attri_me->yymmdd[i] >> 8) & 0xFF, attri_me->yymmdd[i]& 0xFF);
				//printf("click:%4d-%02d-%02d\n", (attri_me->yymmdd[i] >> 16) & 0xFFFF, (attri_me->yymmdd[i] >> 8) & 0xFF, attri_me->yymmdd[i]& 0xFF);
				if (attri_me->ref_timer == NULL)
					attri_me->ref_timer = tui_timer_create(tui_com_calendar_ref_timer_cb, 10 * 1000, TUI_TIMER_PRIO_HIGH, com_calendar);
				else
					tui_timer_reset(attri_me->ref_timer);
				break;
			}
		}
	}
}

tui_obj_t * tui_com_calendar_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_calendar_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };//注意先清空结构体，避免随机值

	attri_com = malloc(sizeof(tui_com_calendar_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_calendar_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_calendar_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_calendar_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0xFFF8F8F8;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_calendar_container_cb;
	tui_container_set_attri(ret, &attri_root);

	return ret;
}

int tui_com_calendar_set_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri)
{
	char temp_str[20];
	tui_time_t cur_t = { 0 };
	tui_label_attri_t attri_txt = { 0 };//注意先清空结构体，避免随机值
	tui_dropdown_attri_t attri_dropdown = { 0 };//注意先清空结构体，避免随机值
	tui_line_attri_t attri_line = { 0 };//注意先清空结构体，避免随机值
	tui_com_calendar_attri_t *attri_me;
	int i = 0, j = 0;
	int unit_w, unit_h;

	if (com_calendar == NULL) {
		printf("tui_com_calendar_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	memcpy(attri_me, attri, sizeof(tui_com_calendar_attri_t));
	tui_obj_set_x(com_calendar, attri_me->obj.pt.x);
	tui_obj_set_y(com_calendar, attri_me->obj.pt.y);
	tui_obj_set_width(com_calendar, attri_me->obj.size.width);
	tui_obj_set_height(com_calendar, attri_me->obj.size.height);
	tui_obj_set_hidden(com_calendar, attri_me->obj.hidden);

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_calendar, attri_me->obj.obj_id);

	cur_t = tui_get_localtime();
	attri_me->set_time = cur_t;
	sprintf(temp_str, "%d-%02d-%02d", cur_t.year, cur_t.mon + 1, cur_t.mday);

	unit_w = attri_me->obj.size.width / 7;
	unit_h = ((attri_me->obj.size.height - TUI_COM_CALENDAR_FIRST_ROW_H - TUI_COM_CALENDAR_SECOND_ROW_H) / 6) - TUI_COM_CALENDAR_DAY_TXT_LINE_SIZE;

	attri_me->yl_year_text_obj = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = 0;
	attri_txt.obj.pt.y = 0;
	attri_txt.obj.size.width = attri_me->obj.size.width;
	attri_txt.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H;
	attri_txt.txt = temp_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFFEF0000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_RIGHT;
	tui_label_set_attri(attri_me->yl_year_text_obj, &attri_txt);
	tui_obj_set_bg_color(attri_me->yl_year_text_obj, 0xFFEFE8E0);

	attri_me->nl_year_text_obj = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = 0;
	attri_txt.obj.pt.y = 0;
	attri_txt.obj.size.width = attri_me->obj.size.width;
	attri_txt.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H;
	attri_txt.txt = temp_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFFEF0000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_RIGHT;
	tui_label_set_attri(attri_me->nl_year_text_obj, &attri_txt);
	tui_label_set_align_bottom(attri_me->nl_year_text_obj, 1);

	for (i = 0; i < 7; i++) {
		attri_me->week_text_obj[i] = tui_label_create(com_calendar);
		attri_txt.obj.pt.x = i * unit_w;
		attri_txt.obj.pt.y = TUI_COM_CALENDAR_FIRST_ROW_H;
		attri_txt.obj.size.width = unit_w;
		attri_txt.obj.size.height = TUI_COM_CALENDAR_SECOND_ROW_H;
		if (strlen(attri_me->week_str[i]) == 0)
			attri_txt.txt = (char *)day_name[i];
		else
			attri_txt.txt = (char *)attri_me->week_str[i];
		attri_txt.fnt_size = TUI_COM_CALENDAR_WEEK_TXT_FNT_SIZE;
		attri_txt.fnt_color = 0xFF000000;
		attri_txt.mode = TUI_LABEL_LONG_DOT;
		attri_txt.align = TUI_LABEL_ALIGN_CENTER;
		tui_label_set_attri(attri_me->week_text_obj[i], &attri_txt);
		tui_label_set_align_mid(attri_me->week_text_obj[i], 1);
	}

	for (j = 0; j < 6; j++) {
		for (i = 0; i < 7; i++) {
			attri_me->day_text_obj[j * 7 + i] = tui_label_create(com_calendar);
			attri_me->nl_text_obj[j * 7 + i] = tui_label_create(com_calendar);
			attri_txt.obj.pt.x = i * unit_w;
			attri_txt.obj.pt.y = (TUI_COM_CALENDAR_FIRST_ROW_H + TUI_COM_CALENDAR_SECOND_ROW_H) + (unit_h * j) + (j * TUI_COM_CALENDAR_DAY_TXT_LINE_SIZE);
			attri_txt.obj.size.width = unit_w;
			attri_txt.obj.size.height = unit_h;
			attri_txt.cb = NULL;
			attri_txt.txt = "**";
			attri_txt.fnt_size = TUI_COM_CALENDAR_DAY_TXT_FNT_SIZE;
			attri_txt.fnt_color = 0xFF000000;
			attri_txt.mode = TUI_LABEL_LONG_DOT;
			attri_txt.align = TUI_LABEL_ALIGN_CENTER;
			tui_label_set_attri(attri_me->day_text_obj[j * 7 + i], &attri_txt);

			attri_txt.cb = tui_com_calendar_day_cb_t;
			attri_txt.fnt_size = TUI_COM_CALENDAR_NL_TXT_FNT_SIZE;
			tui_label_set_attri(attri_me->nl_text_obj[j * 7 + i], &attri_txt);
			tui_label_set_align_bottom(attri_me->nl_text_obj[j * 7 + i], 1);
			tui_obj_set_click(attri_me->nl_text_obj[j * 7 + i], 1);
		}
	}

	attri_me->year_dropdown_bnt = tui_dropdown_create(com_calendar);
	/* 设置属性 */
	attri_dropdown.obj.pt.x = 30;
	attri_dropdown.obj.pt.y = (TUI_COM_CALENDAR_FIRST_ROW_H - 45) / 2;
	attri_dropdown.obj.size.width = 100;
	attri_dropdown.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H;
	attri_dropdown.cb = tui_com_calendar_year_dropdown_cb;
	if (cur_t.year >= 1900 && cur_t.year <= 2100)
		attri_dropdown.cur_index = cur_t.year - 1900;
	else
		attri_dropdown.cur_index = 0;
	attri_dropdown.options_num = 201;			/* 当前下拉菜单的索引值 */
	for (i = 0; i < 201; i++) {
		sprintf(temp_str, "%d", 1900+i);
		strcpy(attri_dropdown.options[i], temp_str);		/* 下拉菜单的文本设置 */
	}
	attri_dropdown.bar_color = 0xFFAF3F0F;		/* 下拉菜单bar的底色（0xFF112233  FF是透明度；11是R；22是G；33是B） */
	attri_dropdown.list_color = 0xFF9FAFAF;		/* 下拉菜单列表的底色（0xFF112233  FF是透明度；11是R；22是G；33是B） */
	attri_dropdown.text_color = 0xFF000000;		/* 下拉菜单字符的颜色（0xFF112233  FF是透明度；11是R；22是G；33是B） */
	attri_dropdown.text_font_size = TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE;		/* 下拉菜单字符字体大小 */
	tui_dropdown_set_attri(attri_me->year_dropdown_bnt, &attri_dropdown);
	tui_dropdown_set_symbol(attri_me->year_dropdown_bnt, 0);
	tui_dropdown_set_max_height(attri_me->year_dropdown_bnt, attri_me->obj.size.height - 60);
	tui_obj_set_border_radius(attri_me->year_dropdown_bnt, 10);
	
	attri_me->year_txt = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = attri_dropdown.obj.pt.x + attri_dropdown.obj.size.width;
	attri_txt.obj.pt.y = 0;
	attri_txt.obj.size.width = 80;
	attri_txt.obj.size.height = attri_dropdown.obj.size.height;
	if (strlen(attri_me->year_str) == 0)
		attri_txt.txt = "\xe5\xb9\xb4";
	else
		attri_txt.txt = attri_me->year_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_BTN_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFF000000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_LEFT;
	tui_label_set_attri(attri_me->year_txt, &attri_txt);
	tui_label_set_align_mid(attri_me->year_txt, 1);

	attri_me->month_dropdown_bnt = tui_dropdown_create(com_calendar);
	/* 设置属性 */
	attri_dropdown.obj.pt.x = 30 + 100 + 80;
	attri_dropdown.obj.pt.y = (TUI_COM_CALENDAR_FIRST_ROW_H - 45) / 2;
	attri_dropdown.obj.size.width = 100;
	attri_dropdown.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H;
	attri_dropdown.cb = tui_com_calendar_month_dropdown_cb;
	if (cur_t.mon >= 0 && cur_t.mon <= 11)
		attri_dropdown.cur_index = cur_t.mon;
	else
		attri_dropdown.cur_index = 0;
	attri_dropdown.options_num = 12;			/* 当前下拉菜单的索引值 */
	for (i = 0; i < 12; i++) {
		sprintf(temp_str, "%d", 1 + i);
		strcpy(attri_dropdown.options[i], temp_str);		/* 下拉菜单的文本设置 */
	}
	attri_dropdown.bar_color = 0xFFAF3F0F;		/* 下拉菜单bar的底色（0xFF112233  FF是透明度；11是R；22是G；33是B） */
	attri_dropdown.list_color = 0xFF9FAFAF;		/* 下拉菜单列表的底色（0xFF112233  FF是透明度；11是R；22是G；33是B） */
	attri_dropdown.text_color = 0xFF000000;		/* 下拉菜单字符的颜色（0xFF112233  FF是透明度；11是R；22是G；33是B） */
	attri_dropdown.text_font_size = TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE;		/* 下拉菜单字符字体大小 */
	tui_dropdown_set_attri(attri_me->month_dropdown_bnt, &attri_dropdown);
	tui_dropdown_set_symbol(attri_me->month_dropdown_bnt, 0);
	tui_dropdown_set_max_height(attri_me->month_dropdown_bnt, attri_me->obj.size.height - 60);
	tui_obj_set_border_radius(attri_me->month_dropdown_bnt, 10);

	attri_me->month_txt = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = attri_dropdown.obj.pt.x + attri_dropdown.obj.size.width;
	attri_txt.obj.pt.y = 0;
	attri_txt.obj.size.width = 80;
	attri_txt.obj.size.height = attri_dropdown.obj.size.height;
	if (strlen(attri_me->month_str) == 0)
		attri_txt.txt = "\xe6\x9c\x88";
	else
		attri_txt.txt = attri_me->month_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_BTN_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFF000000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_LEFT;
	tui_label_set_attri(attri_me->month_txt, &attri_txt);
	tui_label_set_align_mid(attri_me->month_txt, 1);


	attri_me->year_line_obj = tui_line_create(com_calendar);
	attri_line.color = 0xFF1F1F1F;
	attri_line.width = 1;
	attri_line.pts[0].x = 0;
	attri_line.pts[0].y = TUI_COM_CALENDAR_FIRST_ROW_H + TUI_COM_CALENDAR_SECOND_ROW_H - 10;
	attri_line.pts[1].x = attri_me->obj.size.width;
	attri_line.pts[1].y = TUI_COM_CALENDAR_FIRST_ROW_H + TUI_COM_CALENDAR_SECOND_ROW_H - 10;
	tui_line_set_attri(attri_me->year_line_obj, &attri_line);
	
	tui_com_calendar_show_year_month(com_calendar, cur_t.year, cur_t.mon + 1, cur_t.mday);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_calendar, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_calendar_get_attri(tui_obj_t *com_calendar, tui_com_calendar_attri_t *attri)
{
	tui_com_calendar_attri_t *attri_me;

	if (com_calendar == NULL) {
		printf("tui_com_calendar_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	*attri = *attri_me;

	return 0;
}

void tui_com_calendar_show_year_month(tui_obj_t *com_calendar, int year, int month, int day)
{
	tui_com_calendar_attri_t *attri_me;
	char temp_str[128] = { 0 };
	int i = 0, month_length = 0, week_start, day_cnt, day_text_obj_index_cnt = 0, is_jieri = 0;

	if (com_calendar == NULL) {
		printf("tui_com_calendar_show_year_month L%d: faile\n", __LINE__);
		return;
	}

	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	if (year < 1900 || year > 2100)
		return;

	sprintf(temp_str, "%d-%02d-%02d", year, month, day);
	tui_label_set_txt(attri_me->yl_year_text_obj, temp_str);
	tui_com_calendar_get_lunar_calendar(year, month, day, &is_jieri, temp_str);
	tui_label_set_txt(attri_me->nl_year_text_obj, temp_str);
	if (year >= 1900 && year <= 2100)
		tui_dropdown_set_selected_index(attri_me->year_dropdown_bnt, year - 1900);
	if (month >= 1 && month <= 12)
		tui_dropdown_set_selected_index(attri_me->month_dropdown_bnt, month-1);

	day_text_obj_index_cnt = 0;
	week_start = tui_com_calendar_get_day_of_week(year, month, 1);
	if (week_start == 0)
		week_start = 7;
	i = 0;
	month_length = tui_com_calendar_get_month_length(year, month - 1);
	while (i < week_start) {
		sprintf(temp_str, "%02d", month_length - i);
		tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0);
		tui_label_set_txt_color(attri_me->day_text_obj[week_start - i - 1], 0xFFAFAFAF);
		tui_label_set_txt(attri_me->day_text_obj[week_start - i - 1], temp_str);

		if (month - 1 < 1)
			tui_label_set_txt(attri_me->nl_text_obj[week_start - i - 1], tui_com_calendar_get_lunar_calendar(year - 1, 12, month_length - i, &is_jieri, NULL));
		else
			tui_label_set_txt(attri_me->nl_text_obj[week_start - i - 1], tui_com_calendar_get_lunar_calendar(year, month - 1, month_length - i, &is_jieri, NULL));
		if (is_jieri)
			tui_label_set_txt_color(attri_me->nl_text_obj[week_start - i - 1], 0xFFAFAFFF);
		else
			tui_label_set_txt_color(attri_me->nl_text_obj[week_start - i - 1], 0xFFAFAFAF);
		attri_me->yymmdd[week_start - i - 1] = (year << 16) | ((month - 1) << 8) | (month_length - i);

		i++;
		day_text_obj_index_cnt++;
	}

	week_start = tui_com_calendar_get_day_of_week(year, month, 1);
	month_length = tui_com_calendar_get_month_length(year, month);
	day_cnt = 0;
	while (day_cnt < month_length) {
		if (month == month && day_cnt + 1 == day && year == year) {
			tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0xFFE333FF);
		} else {
			tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0);
		}
		sprintf(temp_str, "%02d", (day_cnt + 1));
		tui_label_set_txt_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0xFF000000);
		tui_label_set_txt(attri_me->day_text_obj[day_text_obj_index_cnt], temp_str);

		tui_label_set_txt(attri_me->nl_text_obj[day_text_obj_index_cnt], tui_com_calendar_get_lunar_calendar(year, month, day_cnt + 1, &is_jieri, NULL));
		if (is_jieri)
			tui_label_set_txt_color(attri_me->nl_text_obj[day_text_obj_index_cnt], 0xFF0000FF);
		else
			tui_label_set_txt_color(attri_me->nl_text_obj[day_text_obj_index_cnt], 0xFF000000);
		attri_me->yymmdd[day_text_obj_index_cnt] = (year << 16) | ((month) << 8) | (day_cnt + 1);

		day_cnt++;
		day_text_obj_index_cnt++;
	}

	i = 1;
	while (day_text_obj_index_cnt < 42) {
		sprintf(temp_str, "%02d", i++);
		tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0);
		tui_label_set_txt_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0xFFAFAFAF);
		tui_label_set_txt(attri_me->day_text_obj[day_text_obj_index_cnt], temp_str);

		if (month + 1 > 12)
			tui_label_set_txt(attri_me->nl_text_obj[day_text_obj_index_cnt], tui_com_calendar_get_lunar_calendar(year + 1, 1, i - 1, &is_jieri, NULL));
		else
			tui_label_set_txt(attri_me->nl_text_obj[day_text_obj_index_cnt], tui_com_calendar_get_lunar_calendar(year, month + 1, i - 1, &is_jieri, NULL));
		if (is_jieri)
			tui_label_set_txt_color(attri_me->nl_text_obj[day_text_obj_index_cnt], 0xFFAFAFFF);
		else
			tui_label_set_txt_color(attri_me->nl_text_obj[day_text_obj_index_cnt], 0xFFAFAFAF);
		attri_me->yymmdd[day_text_obj_index_cnt] = (year << 16) | ((month + 1) << 8) | (i - 1);

		day_text_obj_index_cnt++;
	}
}

tui_obj_t * tui_com_calendar_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_calendar_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_calendar_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_calendar_create(par);
	if (ret == NULL) {
		printf("tui_com_calendar_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_calendar_create_json L%d: don't support child object\n", __LINE__, item->string);
		} else {
			if (strcmp(item->string, "type_name") == 0)
				strcpy(attri.obj.type_name, item->valuestring);
			else if (strcmp(item->string, "obj_id") == 0)
				attri.obj.obj_id = item->valueint;
			else if (strcmp(item->string, "hidden") == 0)
				attri.obj.hidden = item->valueint;
			else if (strcmp(item->string, "pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.obj.pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.obj.pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "size") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.obj.size.width = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.obj.size.height = array->valueint;
				}
			} else if (strcmp(item->string, "week_str") == 0) {
				if (tJSON_GetArraySize(item) == 7) {
					array = tJSON_GetArrayItem(item, 0);
					strcpy(attri.week_str[0], array->valuestring);
					array = tJSON_GetArrayItem(item, 1);
					strcpy(attri.week_str[1], array->valuestring);
					array = tJSON_GetArrayItem(item, 2);
					strcpy(attri.week_str[2], array->valuestring);
					array = tJSON_GetArrayItem(item, 3);
					strcpy(attri.week_str[3], array->valuestring);
					array = tJSON_GetArrayItem(item, 4);
					strcpy(attri.week_str[4], array->valuestring);
					array = tJSON_GetArrayItem(item, 5);
					strcpy(attri.week_str[5], array->valuestring);
					array = tJSON_GetArrayItem(item, 6);
					strcpy(attri.week_str[6], array->valuestring);
				}
			} else if (strcmp(item->string, "year_str") == 0) {
				strcpy(attri.year_str, item->valuestring);
			} else if (strcmp(item->string, "month_str") == 0) {
				strcpy(attri.month_str, item->valuestring);
			} else if (strcmp(item->string, "cb") == 0)
				;
			else
				printf("tui_com_calendar_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = (tui_com_calendar_cb_t)tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_calendar_set_attri(ret, &attri);

	return ret;
}
