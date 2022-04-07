#include "tui.h"

static void tui_com_msgbox_container_cb(tui_obj_t *com_msgbox, tui_event_e event)
{
	tui_com_msgbox_attri_t *attri_me;

	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		if (attri_me->del_timer) {
			tui_timer_del(attri_me->del_timer);
			attri_me->del_timer = NULL;
		}
		free(attri_me);
	}
}

static void tui_com_msgbox_yes_button_cb(tui_obj_t *obj, tui_event_e event)
{
	tui_obj_t *com_msgbox;
	tui_com_msgbox_attri_t *attri_me;

	com_msgbox = tui_obj_get_parent((const tui_obj_t *)obj);
	if (com_msgbox == NULL) {
		printf("tui_com_msgbox_yes_button_cb L%d: faile\n", __LINE__);
		return;
	}
	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	if (event == TUI_EVENT_RELEASED) {
		tui_obj_set_click(tui_layer_top(), 0);
		if (attri_me->cb)
			attri_me->cb(com_msgbox, TUI_EVENT_DELETE, 1);

		tui_obj_del(com_msgbox);
	}
}

static void tui_com_msgbox_no_button_cb(tui_obj_t *obj, tui_event_e event)
{
	tui_obj_t *com_msgbox;
	tui_com_msgbox_attri_t *attri_me;

	com_msgbox = tui_obj_get_parent((const tui_obj_t *)obj);
	if (com_msgbox == NULL) {
		printf("tui_com_msgbox_no_button_cb L%d: faile\n", __LINE__);
		return;
	}
	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	if (event == TUI_EVENT_RELEASED) {
		tui_obj_set_click(tui_layer_top(), 0);
		if (attri_me->cb)
			attri_me->cb(com_msgbox, TUI_EVENT_DELETE, 0);

		tui_obj_del(com_msgbox);
	}
}

static void tui_com_msgbox_ok_button_cb(tui_obj_t *obj, tui_event_e event)
{
	tui_obj_t *com_msgbox;
	tui_com_msgbox_attri_t *attri_me;

	com_msgbox = tui_obj_get_parent((const tui_obj_t *)obj);
	if (com_msgbox == NULL) {
		printf("tui_com_msgbox_ok_button_cb L%d: faile\n", __LINE__);
		return;
	}
	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	if (event == TUI_EVENT_RELEASED) {
		tui_obj_set_click(tui_layer_top(), 0);
		if (attri_me->cb)
			attri_me->cb(com_msgbox, TUI_EVENT_DELETE, 2);

		tui_obj_del(com_msgbox);
	}
}

static void tui_com_msgbox_del_timer_cb(tui_timer_t * timer)
{
	tui_obj_t *com_msgbox;
	tui_com_msgbox_attri_t *attri_me;

	com_msgbox = tui_timer_get_user_data(timer);
	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	tui_obj_set_click(tui_layer_top(), 0);
	if (attri_me->cb)
		attri_me->cb(com_msgbox, TUI_EVENT_DELETE, 3);

	if (attri_me->del_timer) {
		tui_timer_del(attri_me->del_timer);
		attri_me->del_timer = NULL;
	}
	tui_obj_del(com_msgbox);
}

tui_obj_t * tui_com_msgbox_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_msgbox_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };

	attri_com = malloc(sizeof(tui_com_msgbox_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_msgbox_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_msgbox_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_msgbox_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0xFFEFEFEF;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_msgbox_container_cb;
	tui_container_set_attri(ret, &attri_root);
	
	tui_obj_set_border_radius(ret, 5);
	tui_obj_set_border_color(ret, 0xFF1F1FFF);
	tui_obj_set_border_width(ret, 1);

	return ret;
}

int tui_com_msgbox_set_attri(tui_obj_t *com_msgbox, tui_com_msgbox_attri_t *attri)
{
	tui_com_msgbox_attri_t *attri_me;
	tui_label_attri_t attri_txt = { 0 };
	tui_button_attri_t attri_btn = { 0 };
	int screen_hor_res;
	int screen_ver_res;

	if (com_msgbox == NULL) {
		printf("tui_com_msgbox_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	memcpy(attri_me, attri, sizeof(tui_com_msgbox_attri_t));
	
	tui_config_get_screen_resolution(&screen_hor_res, &screen_ver_res);
	attri_me->obj.pt.x = (screen_hor_res - attri_me->obj.size.width) / 2;
	attri_me->obj.pt.y = (screen_ver_res - attri_me->obj.size.height) / 2;

	tui_obj_set_x(com_msgbox, attri_me->obj.pt.x);
	tui_obj_set_y(com_msgbox, attri_me->obj.pt.y);
	tui_obj_set_width(com_msgbox, attri_me->obj.size.width);
	tui_obj_set_height(com_msgbox, attri_me->obj.size.height);
	tui_obj_set_hidden(com_msgbox, attri_me->obj.hidden);

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_msgbox, attri_me->obj.obj_id);

	attri_me->msg_txt = tui_label_create(com_msgbox);
	attri_txt.obj.pt.x = 0;
	attri_txt.obj.pt.y = 0;
	if (attri->btn_num == 0) {
		attri_txt.obj.size.width = attri_me->obj.size.width;
		attri_txt.obj.size.height = attri_me->obj.size.height;
	} else {
		attri_txt.obj.size.width = attri_me->obj.size.width;
		attri_txt.obj.size.height = attri_me->obj.size.height - 50;
	}
	attri_txt.txt = attri_me->msg_str;
	attri_txt.fnt_size = 32;
	attri_txt.fnt_color = 0xFF000000;
	attri_txt.mode = TUI_LABEL_LONG_DOT;
	attri_txt.align = TUI_LABEL_ALIGN_CENTER;
	tui_label_set_attri(attri_me->msg_txt, &attri_txt);
	tui_label_set_align_mid(attri_me->msg_txt, 1);

	if (attri->btn_num == 2) {
		attri_me->yes_bnt = tui_button_create(com_msgbox);
		attri_btn.obj.pt.x = attri_me->obj.size.width/2 - 80;
		attri_btn.obj.pt.y = attri_me->obj.size.height - 50;
		attri_btn.obj.size.width = 60;
		attri_btn.obj.size.height = 40;
		attri_btn.cb = tui_com_msgbox_yes_button_cb;
		attri_btn.border_color = 0xFF5F0000;
		attri_btn.border_width = 1;
		tui_button_set_attri(attri_me->yes_bnt, &attri_btn);

		attri_me->yes_txt = tui_label_create(com_msgbox);
		attri_txt.obj.pt.x = attri_btn.obj.pt.x;
		attri_txt.obj.pt.y = attri_btn.obj.pt.y;
		attri_txt.obj.size.width = attri_btn.obj.size.width;
		attri_txt.obj.size.height = attri_btn.obj.size.height;
		attri_txt.txt = attri_me->yes_str;
		attri_txt.fnt_size = 32;
		attri_txt.fnt_color = 0xFF000000;
		attri_txt.mode = TUI_LABEL_LONG_DOT;
		attri_txt.align = TUI_LABEL_ALIGN_CENTER;
		tui_label_set_attri(attri_me->yes_txt, &attri_txt);
		tui_label_set_align_mid(attri_me->yes_txt, 1);


		attri_me->no_bnt = tui_button_create(com_msgbox);
		attri_btn.obj.pt.x = attri_me->obj.size.width / 2 + 80 - attri_btn.obj.size.width;
		attri_btn.obj.pt.y = attri_me->obj.size.height - 50;
		attri_btn.obj.size.width = attri_btn.obj.size.width;
		attri_btn.obj.size.height = attri_btn.obj.size.height;
		attri_btn.cb = tui_com_msgbox_no_button_cb;
		attri_btn.border_color = 0xFF5F0000;
		attri_btn.border_width = 1;
		tui_button_set_attri(attri_me->no_bnt, &attri_btn);

		attri_me->no_txt = tui_label_create(com_msgbox);
		attri_txt.obj.pt.x = attri_btn.obj.pt.x;
		attri_txt.obj.pt.y = attri_btn.obj.pt.y;
		attri_txt.obj.size.width = attri_btn.obj.size.width;
		attri_txt.obj.size.height = attri_btn.obj.size.height;
		attri_txt.txt = attri_me->no_str;
		attri_txt.fnt_size = 32;
		attri_txt.fnt_color = 0xFF000000;
		attri_txt.mode = TUI_LABEL_LONG_DOT;
		attri_txt.align = TUI_LABEL_ALIGN_CENTER;
		tui_label_set_attri(attri_me->no_txt, &attri_txt);
		tui_label_set_align_mid(attri_me->no_txt, 1);
	} else if (attri->btn_num == 1) {
		attri_me->ok_bnt = tui_button_create(com_msgbox);
		attri_btn.obj.pt.x = attri_me->obj.size.width / 2 - 30;
		attri_btn.obj.pt.y = attri_me->obj.size.height - 50;
		attri_btn.obj.size.width = 60;
		attri_btn.obj.size.height = 40;
		attri_btn.cb = tui_com_msgbox_ok_button_cb;
		attri_btn.border_color = 0xFF5F0000;
		attri_btn.border_width = 1;
		tui_button_set_attri(attri_me->ok_bnt, &attri_btn);

		attri_me->ok_txt = tui_label_create(com_msgbox);
		attri_txt.obj.pt.x = attri_btn.obj.pt.x;
		attri_txt.obj.pt.y = attri_btn.obj.pt.y;
		attri_txt.obj.size.width = attri_btn.obj.size.width;
		attri_txt.obj.size.height = attri_btn.obj.size.height;
		attri_txt.txt = attri_me->yes_str;
		attri_txt.fnt_size = 32;
		attri_txt.fnt_color = 0xFF000000;
		attri_txt.mode = TUI_LABEL_LONG_DOT;
		attri_txt.align = TUI_LABEL_ALIGN_CENTER;
		tui_label_set_attri(attri_me->ok_txt, &attri_txt);
		tui_label_set_align_mid(attri_me->ok_txt, 1);
	} else {
		attri_me->del_timer = tui_timer_create(tui_com_msgbox_del_timer_cb, 5 * 1000, TUI_TIMER_PRIO_HIGH, com_msgbox);
	}
	
	tui_obj_set_click(tui_layer_top(), 1);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_msgbox, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_msgbox_get_attri(tui_obj_t *com_msgbox, tui_com_msgbox_attri_t *attri)
{
	tui_com_msgbox_attri_t *attri_me;

	if (com_msgbox == NULL) {
		printf("tui_com_msgbox_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_msgbox_attri_t *)tui_com_get_com_attri(com_msgbox);

	*attri = *attri_me;

	return 0;
}

tui_obj_t * tui_com_msgbox_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_msgbox_attri_t attri = { 0 };
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_msgbox_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_msgbox_create(par);
	if (ret == NULL) {
		printf("tui_com_msgbox_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_msgbox_create_json L%d: don't support child object\n", __LINE__, item->string);
		}
		else {
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
			}
			else if (strcmp(item->string, "size") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.obj.size.width = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.obj.size.height = array->valueint;
				}
			}
			else if (strcmp(item->string, "btn_num") == 0)
				attri.btn_num = item->valueint;
			else if (strcmp(item->string, "msg_str") == 0)
				attri.msg_str = item->valuestring;
			else if (strcmp(item->string, "yes_str") == 0)
				attri.yes_str = item->valuestring;
			else if (strcmp(item->string, "no_str") == 0)
				attri.no_str = item->valuestring;
			else if (strcmp(item->string, "ok_str") == 0)
				attri.ok_str = item->valuestring;
			else if (strcmp(item->string, "cb") == 0)
				;
			else
				printf("tui_com_msgbox_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_msgbox_set_attri(ret, &attri);

	return ret;
}
