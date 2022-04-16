#include "tui.h"

#define TUI_COM_CALENDAR_FIRST_ROW_H		70
#define TUI_COM_CALENDAR_SECOND_ROW_H		60

#define TUI_COM_CALENDAR_BTN_TXT_FNT_SIZE	32
#define TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE	32
#define TUI_COM_CALENDAR_WEEK_TXT_FNT_SIZE	32
#define TUI_COM_CALENDAR_DAY_TXT_FNT_SIZE	32

//#define TUI_COM_CALENDAR_WEEK_STARTS_MONDAY

#ifdef TUI_COM_CALENDAR_WEEK_STARTS_MONDAY
static const char * day_name[7] = { "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su" };
#else
static const char * day_name[7] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };
#endif

/***
 *                                     中国农历年计算
 *
 *-------------------------------------------------------------------------------------
 * bit： 6         5               4             3           2         1         0
 *       |     表示春节的公历月份  |                表示春节的公历日期           |
 *-------------------------------------------------------------------------------------
 * bit： 19     18    17    16    15    14    13    12    11    10    9     8    7
 *       1      2     3     4     5     6     7     8     9     10    11    12   13
 *       |农历 1 - 13 月大小。月份对应位为 1，农历月大（30天），为0表示小（29天）|
 *-------------------------------------------------------------------------------------
 * bit： 23                     22                     21                        20
 *       |              表示当年闰年月月份，值为 0 侧表示当年无闰月              |
 *-------------------------------------------------------------------------------------
 */
static int tui_com_calendar_calc_lunar_calendar(int year, int month, int day, unsigned int *lunar_calendar_day)
{
	int Spring_NY, Sun_NY, StaticDayCount;
	int index, flag;
	const int MonthAdd[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	const unsigned int LunarCalendarTable[199] = {
		0x04AE53, 0x0A5748, 0x5526BD, 0x0D2650, 0x0D9544, 0x46AAB9, 0x056A4D, 0x09AD42, 0x24AEB6, 0x04AE4A,/*1901-1910*/
		0x6A4DBE, 0x0A4D52, 0x0D2546, 0x5D52BA, 0x0B544E, 0x0D6A43, 0x296D37, 0x095B4B, 0x749BC1, 0x049754,/*1911-1920*/
		0x0A4B48, 0x5B25BC, 0x06A550, 0x06D445, 0x4ADAB8, 0x02B64D, 0x095742, 0x2497B7, 0x04974A, 0x664B3E,/*1921-1930*/
		0x0D4A51, 0x0EA546, 0x56D4BA, 0x05AD4E, 0x02B644, 0x393738, 0x092E4B, 0x7C96BF, 0x0C9553, 0x0D4A48,/*1931-1940*/
		0x6DA53B, 0x0B554F, 0x056A45, 0x4AADB9, 0x025D4D, 0x092D42, 0x2C95B6, 0x0A954A, 0x7B4ABD, 0x06CA51,/*1941-1950*/
		0x0B5546, 0x555ABB, 0x04DA4E, 0x0A5B43, 0x352BB8, 0x052B4C, 0x8A953F, 0x0E9552, 0x06AA48, 0x6AD53C,/*1951-1960*/
		0x0AB54F, 0x04B645, 0x4A5739, 0x0A574D, 0x052642, 0x3E9335, 0x0D9549, 0x75AABE, 0x056A51, 0x096D46,/*1961-1970*/
		0x54AEBB, 0x04AD4F, 0x0A4D43, 0x4D26B7, 0x0D254B, 0x8D52BF, 0x0B5452, 0x0B6A47, 0x696D3C, 0x095B50,/*1971-1980*/
		0x049B45, 0x4A4BB9, 0x0A4B4D, 0xAB25C2, 0x06A554, 0x06D449, 0x6ADA3D, 0x0AB651, 0x093746, 0x5497BB,/*1981-1990*/
		0x04974F, 0x064B44, 0x36A537, 0x0EA54A, 0x86B2BF, 0x05AC53, 0x0AB647, 0x5936BC, 0x092E50, 0x0C9645,/*1991-2000*/
		0x4D4AB8, 0x0D4A4C, 0x0DA541, 0x25AAB6, 0x056A49, 0x7AADBD, 0x025D52, 0x092D47, 0x5C95BA, 0x0A954E,/*2001-2010*/
		0x0B4A43, 0x4B5537, 0x0AD54A, 0x955ABF, 0x04BA53, 0x0A5B48, 0x652BBC, 0x052B50, 0x0A9345, 0x474AB9,/*2011-2020*/
		0x06AA4C, 0x0AD541, 0x24DAB6, 0x04B64A, 0x69573D, 0x0A4E51, 0x0D2646, 0x5E933A, 0x0D534D, 0x05AA43,/*2021-2030*/
		0x36B537, 0x096D4B, 0xB4AEBF, 0x04AD53, 0x0A4D48, 0x6D25BC, 0x0D254F, 0x0D5244, 0x5DAA38, 0x0B5A4C,/*2031-2040*/
		0x056D41, 0x24ADB6, 0x049B4A, 0x7A4BBE, 0x0A4B51, 0x0AA546, 0x5B52BA, 0x06D24E, 0x0ADA42, 0x355B37,/*2041-2050*/
		0x09374B, 0x8497C1, 0x049753, 0x064B48, 0x66A53C, 0x0EA54F, 0x06B244, 0x4AB638, 0x0AAE4C, 0x092E42,/*2051-2060*/
		0x3C9735, 0x0C9649, 0x7D4ABD, 0x0D4A51, 0x0DA545, 0x55AABA, 0x056A4E, 0x0A6D43, 0x452EB7, 0x052D4B,/*2061-2070*/
		0x8A95BF, 0x0A9553, 0x0B4A47, 0x6B553B, 0x0AD54F, 0x055A45, 0x4A5D38, 0x0A5B4C, 0x052B42, 0x3A93B6,/*2071-2080*/
		0x069349, 0x7729BD, 0x06AA51, 0x0AD546, 0x54DABA, 0x04B64E, 0x0A5743, 0x452738, 0x0D264A, 0x8E933E,/*2081-2090*/
		0x0D5252, 0x0DAA47, 0x66B53B, 0x056D4F, 0x04AE45, 0x4A4EB9, 0x0A4D4C, 0x0D1541, 0x2D92B5           /*2091-2099*/
	};

	//Spring_NY 记录春节离当年元旦的天数。
	//Sun_NY 记录阳历日离当年元旦的天数。
	if (((LunarCalendarTable[year - 1901] & 0x0060) >> 5) == 1)
		Spring_NY = (LunarCalendarTable[year - 1901] & 0x001F) - 1;
	else
		Spring_NY = (LunarCalendarTable[year - 1901] & 0x001F) - 1 + 31;

	Sun_NY = MonthAdd[month - 1] + day - 1;

	if ((!(year % 4)) && (month > 2))
		Sun_NY++;

	//StaticDayCount记录大小月的天数 29 或30
	//index 记录从哪个月开始来计算。
	//flag 是用来对闰月的特殊处理。
	//判断阳历日在春节前还是春节后
	if (Sun_NY >= Spring_NY) {//阳历日在春节后（含春节那天）
		Sun_NY -= Spring_NY;
		month = 1;
		index = 1;
		flag = 0;
		if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
			StaticDayCount = 29;
		else
			StaticDayCount = 30;

		while (Sun_NY >= StaticDayCount) {
			Sun_NY -= StaticDayCount;
			index++;
			if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20)) {
				flag = ~flag;
				if (flag == 0)
					month++;
			}
			else
				month++;

			if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
				StaticDayCount = 29;
			else
				StaticDayCount = 30;
		}
		day = Sun_NY + 1;
	}
	else { //阳历日在春节前 
		Spring_NY -= Sun_NY;
		year--;
		month = 12;
		if (((LunarCalendarTable[year - 1901] & 0xF00000) >> 20) == 0)
			index = 12;
		else
			index = 13;

		flag = 0;

		if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
			StaticDayCount = 29;
		else
			StaticDayCount = 30;

		while (Spring_NY > StaticDayCount) {
			Spring_NY -= StaticDayCount;
			index--;
			if (flag == 0)
				month--;
			if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20))
				flag = ~flag;

			if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
				StaticDayCount = 29;
			else
				StaticDayCount = 30;
		}
		day = StaticDayCount - Spring_NY + 1;
	}

	*lunar_calendar_day |= day;
	*lunar_calendar_day |= (month << 6);
	if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20))
		return 1;
	else
		return 0;
}

/**
 *                  计算打印出中国农历年
 */
static void tui_com_calendar_get_lunar_calendar(void)
{
	unsigned int lunar_calendar_day = 0;
	const char *ch_day[] = { "*", "初一", "初二", "初三", "初四", "初五",
		                     "初六", "初七", "初八", "初九", "初十",
		                     "十一", "十二", "十三", "十四", "十五",
		                     "十六", "十七", "十八", "十九", "二十",
		                     "廿一", "廿二", "廿三", "廿四", "廿五",
		                     "廿六", "廿七", "廿八", "廿九", "三十" };
	const char *ch_month[] = { "*", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "十一", "腊" };
	int year, month, day;
	char str[13] = "";

	year = 2022;
	month = 5;
	day = 1;

	printf("%d年%d月%d日\n", year, month, day);

	if (tui_com_calendar_calc_lunar_calendar(year, month, day, &lunar_calendar_day))
		strcat(str, "闰");

	strcat(str, ch_month[(lunar_calendar_day & 0x3C0) >> 6]);
	strcat(str, "月");
	strcat(str, ch_day[lunar_calendar_day & 0x3F]);
	printf("%s\n", str);
}

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
	char temp_str[20];
	tui_time_t cur_t = { 0 };

	com_calendar = tui_timer_get_user_data(timer);
	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	cur_t = tui_get_localtime();
	attri_me->set_time = cur_t;

	tui_com_calendar_show_year_month(com_calendar, cur_t.year, cur_t.mon + 1);
	sprintf(temp_str, "%d-%02d-%02d", cur_t.year, cur_t.mon + 1, cur_t.mday);
	tui_label_set_txt(attri_me->year_text_obj, temp_str);

	if (attri_me->ref_timer) {
		tui_timer_del(attri_me->ref_timer);
		attri_me->ref_timer = NULL;
	}
}

static void tui_com_calendar_pre_button_cb_t(tui_obj_t *obj, tui_event_e event)
{
	if (event == TUI_EVENT_RELEASED) {
		tui_obj_t *com_calendar;
		tui_com_calendar_attri_t *attri_me;
		char temp_str[20];
		tui_time_t cur_t = { 0 };

		cur_t = tui_get_localtime();

		com_calendar = tui_obj_get_parent((const tui_obj_t *)obj);
		if (com_calendar == NULL) {
			printf("tui_com_calendar_pre_button_cb_t L%d: faile\n", __LINE__);
			return;
		}
		attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);
		
		attri_me->set_time.mon--;
		if (attri_me->set_time.mon < 0) {
			attri_me->set_time.year--;
			attri_me->set_time.mon = 12 + attri_me->set_time.mon;
		}else if (attri_me->set_time.mon >= 12) {
			attri_me->set_time.year++;
			attri_me->set_time.mon -= 12;
		}
		tui_com_calendar_show_year_month(com_calendar, attri_me->set_time.year, attri_me->set_time.mon + 1);
		sprintf(temp_str, "%d-%02d-%02d", attri_me->set_time.year, attri_me->set_time.mon + 1, attri_me->set_time.mday);
		tui_label_set_txt(attri_me->year_text_obj, temp_str);

		if (cur_t.year != attri_me->set_time.year || cur_t.mon != attri_me->set_time.mon) {
			if (attri_me->ref_timer == NULL)
				attri_me->ref_timer = tui_timer_create(tui_com_calendar_ref_timer_cb, 10 * 1000, TUI_TIMER_PRIO_HIGH, com_calendar);
			else
				tui_timer_reset(attri_me->ref_timer);
		}
	}
}

static void tui_com_calendar_next_button_cb_t(tui_obj_t *obj, tui_event_e event)
{
	if (event == TUI_EVENT_RELEASED) {
		tui_obj_t *com_calendar;
		tui_com_calendar_attri_t *attri_me;
		char temp_str[20];
		tui_time_t cur_t = { 0 };

		cur_t = tui_get_localtime();
		
		com_calendar = tui_obj_get_parent((const tui_obj_t *)obj);
		if (com_calendar == NULL) {
			printf("tui_com_calendar_next_button_cb_t L%d: faile\n", __LINE__);
			return;
		}
		attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

		attri_me->set_time.mon++;
		if (attri_me->set_time.mon < 0) {
			attri_me->set_time.year--;
			attri_me->set_time.mon = 12 + attri_me->set_time.mon;
		}
		else if (attri_me->set_time.mon >= 12) {
			attri_me->set_time.year++;
			attri_me->set_time.mon -= 12;
		}
		tui_com_calendar_show_year_month(com_calendar, attri_me->set_time.year, attri_me->set_time.mon + 1);
		sprintf(temp_str, "%d-%02d-%02d", attri_me->set_time.year, attri_me->set_time.mon + 1, attri_me->set_time.mday);
		tui_label_set_txt(attri_me->year_text_obj, temp_str);

		if (cur_t.year != attri_me->set_time.year || cur_t.mon != attri_me->set_time.mon) {
			if (attri_me->ref_timer == NULL)
				attri_me->ref_timer = tui_timer_create(tui_com_calendar_ref_timer_cb, 10 * 1000, TUI_TIMER_PRIO_HIGH, com_calendar);
			else
				tui_timer_reset(attri_me->ref_timer);
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
	tui_button_attri_t attri_btn = { 0 };//注意先清空结构体，避免随机值
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
	unit_h = (attri_me->obj.size.height - TUI_COM_CALENDAR_FIRST_ROW_H - TUI_COM_CALENDAR_SECOND_ROW_H) / 7;

	attri_me->year_text_obj = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = 0;
	attri_txt.obj.pt.y = 0;
	attri_txt.obj.size.width = attri_me->obj.size.width;
	attri_txt.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H;
	attri_txt.txt = temp_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_YEAR_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFF000000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_CENTER;
	tui_label_set_attri(attri_me->year_text_obj, &attri_txt);
	tui_label_set_align_mid(attri_me->year_text_obj, 1);
	tui_obj_set_bg_color(attri_me->year_text_obj, 0xFFE0E0E0);

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
			attri_txt.obj.pt.x = i * unit_w;
			attri_txt.obj.pt.y = (TUI_COM_CALENDAR_FIRST_ROW_H + TUI_COM_CALENDAR_SECOND_ROW_H) + unit_h * j;
			attri_txt.obj.size.width = unit_w;
			attri_txt.obj.size.height = unit_h;
			attri_txt.txt = "**";
			attri_txt.fnt_size = TUI_COM_CALENDAR_DAY_TXT_FNT_SIZE;
			attri_txt.fnt_color = 0xFF000000;
			attri_txt.mode = TUI_LABEL_LONG_DOT;
			attri_txt.align = TUI_LABEL_ALIGN_CENTER;
			tui_label_set_attri(attri_me->day_text_obj[j * 7 + i], &attri_txt);
			tui_label_set_align_mid(attri_me->day_text_obj[j * 7 + i], 1);
		}
	}

	attri_me->pre_month_bnt = tui_button_create(com_calendar);
	attri_btn.obj.pt.x = 50;
	attri_btn.obj.pt.y = 0+5;
	attri_btn.obj.size.width = 100;
	attri_btn.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H - 10;
	attri_btn.cb = tui_com_calendar_pre_button_cb_t;
	attri_btn.border_color = 0xFF5F0000;
	attri_btn.border_width = 1;
	tui_button_set_attri(attri_me->pre_month_bnt, &attri_btn);

	attri_me->pre_month_txt = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = attri_btn.obj.pt.x;
	attri_txt.obj.pt.y = attri_btn.obj.pt.y;
	attri_txt.obj.size.width = attri_btn.obj.size.width;
	attri_txt.obj.size.height = attri_btn.obj.size.height;
	if (strlen(attri_me->pre_str) == 0)
		attri_txt.txt = "PRE";
	else
		attri_txt.txt = attri_me->pre_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_BTN_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFF000000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_CENTER;
	tui_label_set_attri(attri_me->pre_month_txt, &attri_txt);
	tui_label_set_align_mid(attri_me->pre_month_txt, 1);

	attri_me->next_month_bnt = tui_button_create(com_calendar);
	attri_btn.obj.pt.x = attri_me->obj.size.width - 50 - 100;
	attri_btn.obj.pt.y = 0+5;
	attri_btn.obj.size.width = 100;
	attri_btn.obj.size.height = TUI_COM_CALENDAR_FIRST_ROW_H - 10;
	attri_btn.cb = tui_com_calendar_next_button_cb_t;
	attri_btn.border_color = 0xFF5F0000;
	attri_btn.border_width = 1;
	tui_button_set_attri(attri_me->next_month_bnt, &attri_btn);

	attri_me->next_month_txt = tui_label_create(com_calendar);
	attri_txt.obj.pt.x = attri_btn.obj.pt.x;
	attri_txt.obj.pt.y = attri_btn.obj.pt.y;
	attri_txt.obj.size.width = attri_btn.obj.size.width;
	attri_txt.obj.size.height = attri_btn.obj.size.height;
	if (strlen(attri_me->next_str) == 0)
		attri_txt.txt = "NEXT";
	else
		attri_txt.txt = attri_me->next_str;
	attri_txt.fnt_size = TUI_COM_CALENDAR_BTN_TXT_FNT_SIZE;
	attri_txt.fnt_color = 0xFF000000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_CENTER;
	tui_label_set_attri(attri_me->next_month_txt, &attri_txt);
	tui_label_set_align_mid(attri_me->next_month_txt, 1);


	attri_me->year_line_obj = tui_line_create(com_calendar);
	attri_line.color = 0xFF1F1F1F;
	attri_line.width = 1;
	attri_line.pts[0].x = 0;
	attri_line.pts[0].y = TUI_COM_CALENDAR_FIRST_ROW_H + TUI_COM_CALENDAR_SECOND_ROW_H - 10;
	attri_line.pts[1].x = attri_me->obj.size.width;
	attri_line.pts[1].y = TUI_COM_CALENDAR_FIRST_ROW_H + TUI_COM_CALENDAR_SECOND_ROW_H - 10;
	tui_line_set_attri(attri_me->year_line_obj, &attri_line);
	
	tui_com_calendar_show_year_month(com_calendar, cur_t.year, cur_t.mon + 1);

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

void tui_com_calendar_show_year_month(tui_obj_t *com_calendar, int year, int month)
{
	tui_com_calendar_attri_t *attri_me;
	tui_time_t cur_t = { 0 };
	char temp_str[20];
	int i = 0, month_length = 0, week_start, day_cnt, day_text_obj_index_cnt = 0;

	if (com_calendar == NULL) {
		printf("tui_com_calendar_show_year_month L%d: faile\n", __LINE__);
		return;
	}

	attri_me = (tui_com_calendar_attri_t *)tui_com_get_com_attri(com_calendar);

	cur_t = tui_get_localtime();

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
		i++;
		day_text_obj_index_cnt++;
	}

	week_start = tui_com_calendar_get_day_of_week(year, month, 1);
	month_length = tui_com_calendar_get_month_length(year, month);
	day_cnt = 0;
	while (day_cnt < month_length) {
		if (month == cur_t.mon + 1 && day_cnt + 1 == cur_t.mday && year == cur_t.year) {
			tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0xFF3333FF);
		} else {
			tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0);
		}
		sprintf(temp_str, "%02d", (day_cnt + 1));
		tui_label_set_txt_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0xFF000000);
		tui_label_set_txt(attri_me->day_text_obj[day_text_obj_index_cnt], temp_str);
		day_cnt++;
		day_text_obj_index_cnt++;
	}

	i = 1;
	while (day_text_obj_index_cnt < 42) {
		sprintf(temp_str, "%02d", i++);
		tui_obj_set_bg_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0);
		tui_label_set_txt_color(attri_me->day_text_obj[day_text_obj_index_cnt], 0xFFAFAFAF);
		tui_label_set_txt(attri_me->day_text_obj[day_text_obj_index_cnt], temp_str);
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
			} else if (strcmp(item->string, "pre_str") == 0) {
				strcpy(attri.pre_str, item->valuestring);
			} else if (strcmp(item->string, "next_str") == 0) {
				strcpy(attri.next_str, item->valuestring);
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
