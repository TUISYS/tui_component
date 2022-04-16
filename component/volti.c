#include "tui.h"

static void tui_com_volti_container_cb(tui_obj_t *com_volti, tui_event_e event)
{
	tui_com_volti_attri_t *attri_me;

	attri_me = (tui_com_volti_attri_t *)tui_com_get_com_attri(com_volti);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		free(attri_me);
	}
}

tui_obj_t * tui_com_volti_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_volti_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };//注意先清空结构体，避免随机值

	attri_com = malloc(sizeof(tui_com_volti_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_volti_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_volti_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_volti_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_volti_container_cb;
	tui_container_set_attri(ret, &attri_root);

	return ret;
}

int tui_com_volti_set_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri)
{
	tui_com_volti_attri_t *attri_me;
	tui_image_attri_t attri_img = { 0 };//注意先清空结构体，避免随机值

	if (com_volti == NULL) {
		printf("tui_com_volti_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_volti_attri_t *)tui_com_get_com_attri(com_volti);

	memcpy(attri_me, attri, sizeof(tui_com_volti_attri_t));
	tui_obj_set_x(com_volti, attri_me->obj.pt.x);
	tui_obj_set_y(com_volti, attri_me->obj.pt.y);
	tui_obj_set_width(com_volti, attri_me->obj.size.width);
	tui_obj_set_height(com_volti, attri_me->obj.size.height);
	tui_obj_set_hidden(com_volti, attri_me->obj.hidden);

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_volti, attri_me->obj.obj_id);


	/* 创建对象 */
	attri_me->img_new_up = tui_image_create(com_volti);
	/* 设置属性 */
	attri_img.obj.pt.x = attri_me->up_num_img_pt.x;
	attri_img.obj.pt.y = attri_me->up_num_img_pt.y;
	tui_image_set_attri(attri_me->img_new_up, &attri_img);
	tui_image_set_image_src(attri_me->img_new_up, 0, attri_me->up_num_img_path[attri_me->cur_num]);
	tui_image_set_cur_img_index(attri_me->img_new_up, 0);

	/* 创建对象 */
	attri_me->img_old_up = tui_image_create(com_volti);
	/* 设置属性 */
	attri_img.obj.pt.x = attri_me->up_num_img_pt.x;
	attri_img.obj.pt.y = attri_me->up_num_img_pt.y;
	tui_image_set_attri(attri_me->img_old_up, &attri_img);
	tui_image_set_image_src(attri_me->img_old_up, 0, attri_me->up_num_img_path[attri_me->cur_num]);
	tui_image_set_cur_img_index(attri_me->img_old_up, 0);

	/* 创建对象 */
	attri_me->img_old_down = tui_image_create(com_volti);
	/* 设置属性 */
	attri_img.obj.pt.x = attri_me->down_num_img_pt.x;
	attri_img.obj.pt.y = attri_me->down_num_img_pt.y;
	tui_image_set_attri(attri_me->img_old_down, &attri_img);
	tui_image_set_image_src(attri_me->img_old_down, 0, attri_me->down_num_img_path[attri_me->cur_num]);
	tui_image_set_cur_img_index(attri_me->img_old_down, 0);

	/* 创建对象 */
	attri_me->img_new_down = tui_image_create(com_volti);
	/* 设置属性 */
	attri_img.obj.pt.x = attri_me->down_num_img_pt.x;
	attri_img.obj.pt.y = attri_me->down_num_img_pt.y;
	tui_image_set_attri(attri_me->img_new_down, &attri_img);
	tui_image_set_image_src(attri_me->img_new_down, 0, attri_me->down_num_img_path[attri_me->cur_num]);
	tui_image_set_cur_img_index(attri_me->img_new_down, 0);
	attri_me->height_d = tui_obj_get_height(attri_me->img_new_down);
	tui_image_set_size(attri_me->img_new_down, tui_obj_get_width(attri_me->img_new_down), 0);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_volti, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_volti_get_attri(tui_obj_t *com_volti, tui_com_volti_attri_t *attri)
{
	tui_com_volti_attri_t *attri_me;

	if (com_volti == NULL) {
		printf("tui_com_volti_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_volti_attri_t *)tui_com_get_com_attri(com_volti);

	*attri = *attri_me;

	return 0;
}

static void tui_com_volti_anim1_end_cb(tui_obj_t * obj)
{
	tui_obj_t *com_volti;
	tui_com_volti_attri_t *attri_me;

	com_volti = tui_obj_get_parent((const tui_obj_t *)obj);
	if (com_volti == NULL) {
		printf("tui_com_volti_anim1_cb L%d: faile\n", __LINE__);
		return;
	}
	attri_me = (tui_com_volti_attri_t *)tui_com_get_com_attri(com_volti);

	tui_image_set_image_src(attri_me->img_new_up, 0, attri_me->up_num_img_path[attri_me->cur_num]);
	tui_image_set_image_src(attri_me->img_old_up, 0, attri_me->up_num_img_path[attri_me->cur_num]);

	tui_image_set_image_src(attri_me->img_old_down, 0, attri_me->down_num_img_path[attri_me->cur_num]);
	tui_image_set_image_src(attri_me->img_new_down, 0, attri_me->down_num_img_path[attri_me->cur_num]);
	tui_image_set_size(attri_me->img_new_down, tui_obj_get_width(attri_me->img_new_down), 0);

	if (attri_me->cb) {
		attri_me->cb(com_volti, TUI_EVENT_VALUE_CHANGED, attri_me->cur_num);
	}
}

static void tui_com_volti_anim0_end_cb(tui_obj_t * obj)
{
	tui_obj_t *com_volti;
	tui_com_volti_attri_t *attri_me;

	com_volti = tui_obj_get_parent((const tui_obj_t *)obj);
	if (com_volti == NULL) {
		printf("tui_com_volti_anim1_cb L%d: faile\n", __LINE__);
		return;
	}
	attri_me = (tui_com_volti_attri_t *)tui_com_get_com_attri(com_volti);

	tui_image_anim_height(attri_me->img_new_down, 800, 0, attri_me->height_d, TUI_ANIM_PATH_LINEAR, tui_com_volti_anim1_end_cb);
}

void tui_com_volti_set_num(tui_obj_t *com_volti, uint8_t num)
{
	tui_com_volti_attri_t *attri_me;

	if (com_volti == NULL) {
		printf("tui_com_volti_set_num L%d: faile\n", __LINE__);
		return;
	}

	attri_me = (tui_com_volti_attri_t *)tui_com_get_com_attri(com_volti);

	if (attri_me->cur_num != num) {
		tui_image_set_image_src(attri_me->img_new_up, 0, attri_me->up_num_img_path[num]);
		tui_image_set_cur_img_index(attri_me->img_new_up, 0);
		tui_obj_set_x(attri_me->img_old_up, attri_me->up_num_img_pt.x);
		tui_obj_set_y(attri_me->img_old_up, attri_me->up_num_img_pt.y);
		tui_image_set_image_src(attri_me->img_old_up, 0, attri_me->up_num_img_path[attri_me->cur_num]);
		tui_image_set_cur_img_index(attri_me->img_old_up, 0);

		tui_image_set_image_src(attri_me->img_old_down, 0, attri_me->down_num_img_path[attri_me->cur_num]);
		tui_image_set_cur_img_index(attri_me->img_old_down, 0);
		tui_image_set_image_src(attri_me->img_new_down, 0, attri_me->down_num_img_path[num]);
		tui_image_set_cur_img_index(attri_me->img_new_down, 0);
		tui_image_set_size(attri_me->img_new_down, tui_obj_get_width(attri_me->img_new_down), 0);

		attri_me->cur_num = num;

		tui_image_anim_height(attri_me->img_old_up, 800, tui_obj_get_height(attri_me->img_old_up), 0, TUI_ANIM_PATH_LINEAR, tui_com_volti_anim0_end_cb);
		tui_obj_anim_mov_y(attri_me->img_old_up, 800, tui_obj_get_y(attri_me->img_old_up), tui_obj_get_y(attri_me->img_old_up) + tui_obj_get_height(attri_me->img_old_up), TUI_ANIM_PATH_LINEAR, NULL);
	}
}

tui_obj_t * tui_com_volti_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_volti_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_volti_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_volti_create(par);
	if (ret == NULL) {
		printf("tui_com_volti_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_volti_create_json L%d: don't support child object\n", __LINE__, item->string);
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
			} else if (strcmp(item->string, "up_num_img_path") == 0) {
				if (tJSON_GetArraySize(item) == 10) {
					array = tJSON_GetArrayItem(item, 0);
					strcpy(attri.up_num_img_path[0], array->valuestring);
					array = tJSON_GetArrayItem(item, 1);
					strcpy(attri.up_num_img_path[1], array->valuestring);
					array = tJSON_GetArrayItem(item, 2);
					strcpy(attri.up_num_img_path[2], array->valuestring);
					array = tJSON_GetArrayItem(item, 3);
					strcpy(attri.up_num_img_path[3], array->valuestring);
					array = tJSON_GetArrayItem(item, 4);
					strcpy(attri.up_num_img_path[4], array->valuestring);
					array = tJSON_GetArrayItem(item, 5);
					strcpy(attri.up_num_img_path[5], array->valuestring);
					array = tJSON_GetArrayItem(item, 6);
					strcpy(attri.up_num_img_path[6], array->valuestring);
					array = tJSON_GetArrayItem(item, 7);
					strcpy(attri.up_num_img_path[7], array->valuestring);
					array = tJSON_GetArrayItem(item, 8);
					strcpy(attri.up_num_img_path[8], array->valuestring);
					array = tJSON_GetArrayItem(item, 9);
					strcpy(attri.up_num_img_path[9], array->valuestring);
				}
			} else if (strcmp(item->string, "down_num_img_path") == 0) {
				if (tJSON_GetArraySize(item) == 10) {
					array = tJSON_GetArrayItem(item, 0);
					strcpy(attri.down_num_img_path[0], array->valuestring);
					array = tJSON_GetArrayItem(item, 1);
					strcpy(attri.down_num_img_path[1], array->valuestring);
					array = tJSON_GetArrayItem(item, 2);
					strcpy(attri.down_num_img_path[2], array->valuestring);
					array = tJSON_GetArrayItem(item, 3);
					strcpy(attri.down_num_img_path[3], array->valuestring);
					array = tJSON_GetArrayItem(item, 4);
					strcpy(attri.down_num_img_path[4], array->valuestring);
					array = tJSON_GetArrayItem(item, 5);
					strcpy(attri.down_num_img_path[5], array->valuestring);
					array = tJSON_GetArrayItem(item, 6);
					strcpy(attri.down_num_img_path[6], array->valuestring);
					array = tJSON_GetArrayItem(item, 7);
					strcpy(attri.down_num_img_path[7], array->valuestring);
					array = tJSON_GetArrayItem(item, 8);
					strcpy(attri.down_num_img_path[8], array->valuestring);
					array = tJSON_GetArrayItem(item, 9);
					strcpy(attri.down_num_img_path[9], array->valuestring);
				}
			} else if (strcmp(item->string, "up_num_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.up_num_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.up_num_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "down_num_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.down_num_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.down_num_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "cur_num") == 0) {
				attri.cur_num = item->valueint;
			} else
				printf("tui_com_volti_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = (tui_com_volti_cb_t)tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_volti_set_attri(ret, &attri);

	return ret;
}
