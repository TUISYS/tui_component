#include "tui.h"

static void tui_com_dial_container_cb(tui_obj_t *com_dial, tui_event_e event)
{
	tui_com_dial_attri_t *attri_me;

	attri_me = (tui_com_dial_attri_t *)tui_com_get_com_attri(com_dial);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		free(attri_me);
	}
}

static void tui_com_dial_point_img_cb(tui_obj_t *obj, tui_event_e event, int16_t angle)
{
	char value_str[8];
	int value;

	if (event == TUI_EVENT_VALUE_CHANGED) {
		tui_obj_t *com_dial;
		tui_com_dial_attri_t *attri_me;

		com_dial = tui_obj_get_parent((const tui_obj_t *)obj);
		if (com_dial == NULL) {
			printf("tui_com_dial_point_img_cb L%d: faile\n", __LINE__);
			return;
		}

		attri_me = (tui_com_dial_attri_t *)tui_com_get_com_attri(com_dial);

		if (angle < 240)
			angle += 120;
		else
			angle -= 240;

		value = angle * 100 / 240;
		sprintf(value_str, "%d", value);
		if (attri_me->txt_obj)
			tui_label_set_txt(attri_me->txt_obj, value_str);
		if (attri_me->cb) {
			attri_me->cb(com_dial, TUI_EVENT_VALUE_CHANGED, value);
		}
	}
}

tui_obj_t * tui_com_dial_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_dial_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };//注意先清空结构体，避免随机值

	attri_com = malloc(sizeof(tui_com_dial_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_dial_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_dial_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_dial_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_dial_container_cb;
	tui_container_set_attri(ret, &attri_root);

	return ret;
}

int tui_com_dial_set_attri(tui_obj_t *com_dial, tui_com_dial_attri_t *attri)
{
	tui_com_dial_attri_t *attri_me;
	tui_image_attri_t attri_pt = { 0 };//注意先清空结构体，避免随机值
	tui_label_attri_t attri_txt = { 0 };//注意先清空结构体，避免随机值

	if (com_dial == NULL || attri == NULL) {
		printf("tui_com_dial_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_dial_attri_t *)tui_com_get_com_attri(com_dial);

	memcpy(attri_me, attri, sizeof(tui_com_dial_attri_t));
	tui_obj_set_x(com_dial, attri_me->obj.pt.x);
	tui_obj_set_y(com_dial, attri_me->obj.pt.y);
	tui_obj_set_width(com_dial, attri_me->obj.size.width);
	tui_obj_set_height(com_dial, attri_me->obj.size.height);
	tui_obj_set_hidden(com_dial, attri_me->obj.hidden);

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_dial, attri_me->obj.obj_id);
	
	/* 创建对象 */
	attri_me->bg_img_obj = tui_image_create(com_dial);
	/* 设置属性 */
	attri_pt.obj.pt.x = attri_me->bg_img_pt.x;
	attri_pt.obj.pt.y = attri_me->bg_img_pt.y;
	tui_image_set_attri(attri_me->bg_img_obj, &attri_pt);	
	tui_image_set_image_src(attri_me->bg_img_obj, 0, attri->bg_img_path);
	tui_image_set_cur_img_index(attri_me->bg_img_obj, 0);

	/* 创建对象 */
	attri_me->point_img_obj = tui_image_create(com_dial);
	/* 设置属性 */
	attri_pt.obj.pt.x = attri_me->point_img_pt.x;
	attri_pt.obj.pt.y = attri_me->point_img_pt.y;
	attri_pt.rotate_pt.x = attri_me->point_img_rotate_pt.x;
	attri_pt.rotate_pt.y = attri_me->point_img_rotate_pt.y;
	attri_pt.cb = tui_com_dial_point_img_cb;
	tui_image_set_attri(attri_me->point_img_obj, &attri_pt);
	tui_image_set_image_src(attri_me->point_img_obj, 0, attri->point_img_path);
	tui_image_set_cur_img_index(attri_me->point_img_obj, 0);
	tui_image_set_rotation_center_coor(attri_me->point_img_obj, attri_pt.rotate_pt.x, attri_pt.rotate_pt.y);	/* 设置图片旋转的参考原点坐标 */
	tui_image_set_angle(attri_me->point_img_obj, 240, 0);															/* 默认旋转240度， 最后一个参数0是没有动画过程，1是有动画过程 */
	
	/* 创建对象 */
	attri_me->txt_obj = tui_label_create(com_dial);
	/* 设置属性 */
	attri_txt.obj.pt.x = attri_me->txt_pt.x;
	attri_txt.obj.pt.y = attri_me->txt_pt.y;
	attri_txt.fnt_size = attri_me->txt_fnt_size;
	attri_txt.txt = attri_me->txt;
	attri_txt.fnt_color = attri_me->txt_fnt_color;
	attri_txt.mode = TUI_LABEL_LONG_EXPAND;
	attri_txt.align = TUI_LABEL_ALIGN_CENTER;
	tui_label_set_attri(attri_me->txt_obj, &attri_txt);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_dial, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_dial_get_attri(tui_obj_t *com_dial, tui_com_dial_attri_t *attri)
{
	tui_com_dial_attri_t *attri_me;

	if (com_dial == NULL) {
		printf("tui_com_dial_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_dial_attri_t *)tui_com_get_com_attri(com_dial);

	*attri = *attri_me;

	return 0;
}

void tui_com_dial_set_point_angle(tui_obj_t *com_dial, int value/* 0~100 */)
{
	int value_angle;
	int cur_point_angle;
	tui_com_dial_attri_t *attri_me;

	if (com_dial == NULL) {
		printf("tui_com_dial_set_point_angle L%d: faile\n", __LINE__);
		return;
	}

	attri_me = (tui_com_dial_attri_t *)tui_com_get_com_attri(com_dial);

	if (value > 100)
		value = 100;
	if (value < 0)
		value = 0;

	value_angle = (240 * value) / 100;

	if (value_angle >= 120)
		cur_point_angle = value_angle - 120;
	else
		cur_point_angle = 240 + value_angle;

	/*        .0
	 *
	 *
	 * .270		    .90
	 *
	 *
	 *	      .180
	 */
	tui_image_set_angle(attri_me->point_img_obj, cur_point_angle, 1);/* 最后一个参数0是没有动画过程，1是有动画过程 */
}

tui_obj_t * tui_com_dial_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_dial_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_dial_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_dial_create(par);
	if (ret == NULL) {
		printf("tui_com_dial_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_dial_create_json L%d: don't support child object\n", __LINE__, item->string);
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
			} else if (strcmp(item->string, "bg_img_path") == 0) {
				attri.bg_img_path = item->valuestring;
			} else if (strcmp(item->string, "bg_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.bg_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.bg_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_img_path") == 0) {
				attri.point_img_path = item->valuestring;
			} else if (strcmp(item->string, "point_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_img_rotate_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_img_rotate_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_img_rotate_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "txt_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.txt_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.txt_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "txt") == 0) {
				strcpy(attri.txt, item->valuestring);
			} else if (strcmp(item->string, "txt_fnt_size") == 0) {
				attri.txt_fnt_size = item->valueint;
			} else if (strcmp(item->string, "txt_fnt_color") == 0) {
				attri.txt_fnt_color = item->valueint;
			} else if (strcmp(item->string, "cb") == 0)
				;
			else
				printf("tui_com_dial_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = (tui_com_dial_cb_t)tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_dial_set_attri(ret, &attri);

	return ret;
}
