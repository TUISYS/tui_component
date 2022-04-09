#include "tui.h"

static void tui_com_clock_container_cb(tui_obj_t *com_clock, tui_event_e event)
{
	tui_com_clock_attri_t *attri_me;

	attri_me = (tui_com_clock_attri_t *)tui_com_get_com_attri(com_clock);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		if (attri_me->clock_timer) {
			tui_timer_del(attri_me->clock_timer);
			attri_me->clock_timer = NULL;
		}
		free(attri_me);
	}
}

static void tui_com_clock_timer_cb(tui_timer_t * timer)
{
	static tui_time_t old_t = { 0 };
	tui_time_t cur_t = { 0 };
	tui_obj_t *com_clock;
	tui_com_clock_attri_t *attri_me;

	com_clock = (tui_obj_t *)tui_timer_get_user_data(timer);
	if (com_clock == NULL) {
		printf("tui_com_clock_timer_cb L%d: faile\n", __LINE__);
		return;
	}

	attri_me = (tui_com_clock_attri_t *)tui_com_get_com_attri(com_clock);

	cur_t = tui_get_localtime();

	if (memcmp(&old_t, &cur_t, sizeof(tui_time_t))) {
		/*        .0
		 *
		 *
		 * .270		    .90
		 *
		 *
		 *	      .180
		 */
		tui_image_set_angle(attri_me->point_h_img_obj, (cur_t.hour * 5 + cur_t.min * 5 / 60) * 6, 0);
		tui_image_set_angle(attri_me->point_m_img_obj, cur_t.min * 6, 0);
		tui_image_set_angle(attri_me->point_s_img_obj, cur_t.sec * 6, 0);

		if (attri_me->cb) {
			attri_me->cb(com_clock, TUI_EVENT_VALUE_CHANGED, cur_t.sec);
		}
	}

}

tui_obj_t * tui_com_clock_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_clock_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };

	attri_com = malloc(sizeof(tui_com_clock_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_clock_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_clock_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_clock_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_clock_container_cb;
	tui_container_set_attri(ret, &attri_root);

	return ret;
}

int tui_com_clock_set_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri)
{
	tui_time_t cur_t = { 0 };
	tui_com_clock_attri_t *attri_me;
	tui_image_attri_t attri_pt = { 0 };//注意先清空结构体，避免随机值

	if (com_clock == NULL) {
		printf("tui_com_clock_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_clock_attri_t *)tui_com_get_com_attri(com_clock);

	memcpy(attri_me, attri, sizeof(tui_com_clock_attri_t));
	tui_obj_set_x(com_clock, attri_me->obj.pt.x);
	tui_obj_set_y(com_clock, attri_me->obj.pt.y);
	tui_obj_set_width(com_clock, attri_me->obj.size.width);
	tui_obj_set_height(com_clock, attri_me->obj.size.height);
	tui_obj_set_hidden(com_clock, attri_me->obj.hidden);

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_clock, attri_me->obj.obj_id);

	/* 创建对象 */
	attri_me->bg_img_obj = tui_image_create(com_clock);
	/* 设置属性 */
	attri_pt.obj.pt.x = attri_me->bg_img_pt.x;
	attri_pt.obj.pt.y = attri_me->bg_img_pt.y;
	tui_image_set_attri(attri_me->bg_img_obj, &attri_pt);
	tui_image_set_image_src(attri_me->bg_img_obj, 0, attri->bg_img_path);
	tui_image_set_cur_img_index(attri_me->bg_img_obj, 0);

	/* 创建对象 */
	attri_me->point_h_img_obj = tui_image_create(com_clock);
	/* 设置属性 */
	attri_pt.obj.pt.x = attri_me->point_h_img_pt.x;
	attri_pt.obj.pt.y = attri_me->point_h_img_pt.y;
	attri_pt.rotate_pt.x = attri_me->point_h_img_rotate_pt.x;
	attri_pt.rotate_pt.y = attri_me->point_h_img_rotate_pt.y;
	attri_pt.cb = NULL;
	tui_image_set_attri(attri_me->point_h_img_obj, &attri_pt);
	tui_image_set_image_src(attri_me->point_h_img_obj, 0, attri->point_h_img_path);
	tui_image_set_cur_img_index(attri_me->point_h_img_obj, 0);
	tui_image_set_rotation_center_coor(attri_me->point_h_img_obj, attri_pt.rotate_pt.x, attri_pt.rotate_pt.y);	/* 设置图片旋转的参考原点坐标 */

	/* 创建对象 */
	attri_me->point_m_img_obj = tui_image_create(com_clock);
	/* 设置属性 */
	attri_pt.obj.pt.x = attri_me->point_m_img_pt.x;
	attri_pt.obj.pt.y = attri_me->point_m_img_pt.y;
	attri_pt.rotate_pt.x = attri_me->point_m_img_rotate_pt.x;
	attri_pt.rotate_pt.y = attri_me->point_m_img_rotate_pt.y;
	attri_pt.cb = NULL;
	tui_image_set_attri(attri_me->point_m_img_obj, &attri_pt);
	tui_image_set_image_src(attri_me->point_m_img_obj, 0, attri->point_m_img_path);
	tui_image_set_cur_img_index(attri_me->point_m_img_obj, 0);
	tui_image_set_rotation_center_coor(attri_me->point_m_img_obj, attri_pt.rotate_pt.x, attri_pt.rotate_pt.y);	/* 设置图片旋转的参考原点坐标 */

	/* 创建对象 */
	attri_me->point_s_img_obj = tui_image_create(com_clock);
	/* 设置属性 */
	attri_pt.obj.pt.x = attri_me->point_s_img_pt.x;
	attri_pt.obj.pt.y = attri_me->point_s_img_pt.y;
	attri_pt.rotate_pt.x = attri_me->point_s_img_rotate_pt.x;
	attri_pt.rotate_pt.y = attri_me->point_s_img_rotate_pt.y;
	attri_pt.cb = NULL;
	tui_image_set_attri(attri_me->point_s_img_obj, &attri_pt);
	tui_image_set_image_src(attri_me->point_s_img_obj, 0, attri->point_s_img_path);
	tui_image_set_cur_img_index(attri_me->point_s_img_obj, 0);
	tui_image_set_rotation_center_coor(attri_me->point_s_img_obj, attri_pt.rotate_pt.x, attri_pt.rotate_pt.y);	/* 设置图片旋转的参考原点坐标 */

	attri_me->clock_timer = tui_timer_create(tui_com_clock_timer_cb, 500, TUI_TIMER_PRIO_HIGH, com_clock);

	cur_t = tui_get_localtime();
	tui_image_set_angle(attri_me->point_h_img_obj, (cur_t.hour * 5 + cur_t.min * 5 / 60 ) * 6, 0);
	tui_image_set_angle(attri_me->point_m_img_obj, cur_t.min * 6, 0);
	tui_image_set_angle(attri_me->point_s_img_obj, cur_t.sec * 6, 0);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_clock, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_clock_get_attri(tui_obj_t *com_clock, tui_com_clock_attri_t *attri)
{
	tui_com_clock_attri_t *attri_me;

	if (com_clock == NULL) {
		printf("tui_com_clock_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_clock_attri_t *)tui_com_get_com_attri(com_clock);

	*attri = *attri_me;

	return 0;
}

tui_obj_t * tui_com_clock_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_clock_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_clock_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_clock_create(par);
	if (ret == NULL) {
		printf("tui_com_clock_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_clock_create_json L%d: don't support child object\n", __LINE__, item->string);
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
			} else if (strcmp(item->string, "point_h_img_path") == 0) {
				attri.point_h_img_path = item->valuestring;
			} else if (strcmp(item->string, "point_h_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_h_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_h_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_h_img_rotate_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_h_img_rotate_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_h_img_rotate_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_m_img_path") == 0) {
				attri.point_m_img_path = item->valuestring;
			} else if (strcmp(item->string, "point_m_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_m_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_m_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_m_img_rotate_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_m_img_rotate_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_m_img_rotate_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_s_img_path") == 0) {
				attri.point_s_img_path = item->valuestring;
			} else if (strcmp(item->string, "point_s_img_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_s_img_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_s_img_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "point_s_img_rotate_pt") == 0) {
				if (tJSON_GetArraySize(item) == 2) {
					array = tJSON_GetArrayItem(item, 0);
					attri.point_s_img_rotate_pt.x = array->valueint;
					array = tJSON_GetArrayItem(item, 1);
					attri.point_s_img_rotate_pt.y = array->valueint;
				}
			} else if (strcmp(item->string, "cb") == 0)
				;
			else
				printf("tui_com_clock_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = (tui_com_clock_cb_t)tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_clock_set_attri(ret, &attri);

	return ret;
}
