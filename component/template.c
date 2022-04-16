#include "tui.h"

static void tui_com_template_container_cb(tui_obj_t *com_template, tui_event_e event)
{
	tui_com_template_attri_t *attri_me;

	attri_me = (tui_com_template_attri_t *)tui_com_get_com_attri(com_template);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		free(attri_me);
	}
}

tui_obj_t * tui_com_template_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_template_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };//注意先清空结构体，避免随机值

	attri_com = malloc(sizeof(tui_com_template_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_template_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_template_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_template_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_template_container_cb;
	tui_container_set_attri(ret, &attri_root);

	return ret;
}

int tui_com_template_set_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri)
{
	tui_com_template_attri_t *attri_me;

	if (com_template == NULL) {
		printf("tui_com_template_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_template_attri_t *)tui_com_get_com_attri(com_template);

	memcpy(attri_me, attri, sizeof(tui_com_template_attri_t));
	tui_obj_set_x(com_template, attri_me->obj.pt.x);
	tui_obj_set_y(com_template, attri_me->obj.pt.y);
	tui_obj_set_width(com_template, attri_me->obj.size.width);
	tui_obj_set_height(com_template, attri_me->obj.size.height);
	tui_obj_set_hidden(com_template, attri_me->obj.hidden);

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_template, attri_me->obj.obj_id);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_template, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_template_get_attri(tui_obj_t *com_template, tui_com_template_attri_t *attri)
{
	tui_com_template_attri_t *attri_me;

	if (com_template == NULL) {
		printf("tui_com_template_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_template_attri_t *)tui_com_get_com_attri(com_template);

	*attri = *attri_me;

	return 0;
}

tui_obj_t * tui_com_template_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_template_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_template_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_template_create(par);
	if (ret == NULL) {
		printf("tui_com_template_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_template_create_json L%d: don't support child object\n", __LINE__, item->string);
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
			else if (strcmp(item->string, "bg_color") == 0)
				;// attri.bg_color = item->valueint;
			else if (strcmp(item->string, "fg_color") == 0)
				;// attri.fg_color = item->valueint;
			else if (strcmp(item->string, "com_template_chars") == 0)
				;// attri.com_template_chars = item->valuestring;
			else if (strcmp(item->string, "cb") == 0)
				;
			else
				printf("tui_com_template_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = (tui_com_template_cb_t)tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_template_set_attri(ret, &attri);

	return ret;
}
