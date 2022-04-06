#include "tui.h"

static void tui_com_chart_container_cb(tui_obj_t *com_chart, tui_event_e event)
{
	tui_com_chart_attri_t *attri_me;

	attri_me = (tui_com_chart_attri_t *)tui_com_get_com_attri(com_chart);

	if (event == TUI_EVENT_DELETE) {
		;//free all
		if (attri_me->point_array)
			free(attri_me->point_array);
		if (attri_me->chart_cursor_obj)
			free(attri_me->chart_cursor_obj);
		free(attri_me);
	}
}

tui_obj_t * tui_com_chart_create(tui_obj_t * par)
{
	tui_obj_t *ret;
	tui_com_chart_attri_t *attri_com;
	tui_container_attri_t attri_root = { 0 };

	attri_com = malloc(sizeof(tui_com_chart_attri_t));
	if (attri_com == NULL) {
		printf("tui_com_chart_create L%d: faile\n", __LINE__);
		return NULL;
	}
	memset(attri_com, 0, sizeof(tui_com_chart_attri_t));

	ret = tui_container_create(par);
	if (ret == NULL) {
		printf("tui_com_chart_create L%d: faile\n", __LINE__);
		free(attri_com);
		return NULL;
	}

	attri_root.bg_color = 0;
	attri_root.attri_com = (void*)attri_com;
	attri_root.cb = tui_com_chart_container_cb;
	tui_container_set_attri(ret, &attri_root);

	return ret;
}

int tui_com_chart_set_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri)
{
	tui_container_attri_t attri_cursor = { 0 };
	tui_com_chart_attri_t *attri_me;
	tui_canvas_attri_t attri_bg = { 0 };
	tui_line_attri_t attri_line = { 0 };
	int bg_unit_w, i, hor_num, ver_num;
	tui_point_t bg_points[2];

#define CHART_UNIT_SIZE	33

	if (com_chart == NULL) {
		printf("tui_com_chart_set_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_chart_attri_t *)tui_com_get_com_attri(com_chart);

	memcpy(attri_me, attri, sizeof(tui_com_chart_attri_t));
	tui_obj_set_x(com_chart, attri_me->obj.pt.x);
	tui_obj_set_y(com_chart, attri_me->obj.pt.y);
	tui_obj_set_width(com_chart, attri_me->obj.size.width);
	tui_obj_set_height(com_chart, attri_me->obj.size.height);
	tui_obj_set_hidden(com_chart, attri_me->obj.hidden);

	attri_me->bg_chart_obj = tui_canvas_create(com_chart);
	attri_bg.bg_color = 0xFFAFAFAF;
	attri_bg.obj.size.width = attri_me->obj.size.width;
	attri_bg.obj.size.height = attri_me->obj.size.height;
	tui_canvas_set_attri(attri_me->bg_chart_obj, &attri_bg);

	attri_me->chart_line_obj = tui_line_create(com_chart);
	attri_line.color = 0xFF0012FF;
	attri_line.width = 1;
	tui_line_set_attri(attri_me->chart_line_obj, &attri_line);

	bg_unit_w = CHART_UNIT_SIZE;
	hor_num = attri_bg.obj.size.width / bg_unit_w + 1;
	ver_num = attri_bg.obj.size.height / bg_unit_w + 1;

	for (i = 0; i < hor_num; i++) {
		bg_points[0].x = i*bg_unit_w;
		bg_points[0].y = 0;
		bg_points[1].x = i*bg_unit_w;
		bg_points[1].y = 5;
		tui_canvas_draw_line(attri_me->bg_chart_obj, bg_points, 2, 1, 0xFF3F3F3F);
		while (bg_points[1].y <= ver_num*bg_unit_w) {
			bg_points[0].y += 10;
			bg_points[1].y += 10;
			tui_canvas_draw_line(attri_me->bg_chart_obj, bg_points, 2, 1, 0xFF3F3F3F);
		}
	}
	for (i = 0; i < ver_num; i++) {
		bg_points[0].x = 0;
		bg_points[0].y = i*bg_unit_w;
		bg_points[1].x = 5;
		bg_points[1].y = i*bg_unit_w;
		tui_canvas_draw_line(attri_me->bg_chart_obj, bg_points, 2, 1, 0xFF3F3F3F);
		while (bg_points[1].x <= hor_num*bg_unit_w) {
			bg_points[0].x += 10;
			bg_points[1].x += 10;
			tui_canvas_draw_line(attri_me->bg_chart_obj, bg_points, 2, 1, 0xFF3F3F3F);
		}
	}

	attri_me->point_array = malloc(attri_me->point_num*sizeof(tui_point_t));
	attri_me->chart_cursor_obj = malloc(attri_me->point_num*sizeof(tui_obj_t));
	if (attri_me->point_array && attri_me->chart_cursor_obj) {
		memset(attri_me->point_array, 0, attri_me->point_num*sizeof(tui_point_t));
		for (i = 0; i < attri_me->point_num; i++) {
			attri_me->chart_cursor_obj[i] = tui_container_create(com_chart);
			attri_cursor.bg_color = 0xFFFF0000;
			attri_cursor.obj.pt.x = 0;
			attri_cursor.obj.pt.y = 0;
			attri_cursor.obj.size.width = 10;
			attri_cursor.obj.size.height = 10;
			tui_container_set_attri(attri_me->chart_cursor_obj[i], &attri_cursor);
			tui_obj_set_border_radius(attri_me->chart_cursor_obj[i], 0x7fff);
		}
	} else {
		printf("tui_com_chart_set_attri L%d: malloc faile\n", __LINE__);
	}

	attri_me->obj.obj_id = attri->obj.obj_id;
	tui_obj_set_id(com_chart, attri_me->obj.obj_id);

	if (attri_me->cb != attri->cb) {
		attri_me->cb = attri->cb;
		attri_me->cb(com_chart, TUI_EVENT_OBJECT_INIT, -1);
	}

	return 0;
}

int tui_com_chart_get_attri(tui_obj_t *com_chart, tui_com_chart_attri_t *attri)
{
	tui_com_chart_attri_t *attri_me;

	if (com_chart == NULL) {
		printf("tui_com_chart_get_attri L%d: faile\n", __LINE__);
		return -1;
	}

	attri_me = (tui_com_chart_attri_t *)tui_com_get_com_attri(com_chart);

	*attri = *attri_me;

	return 0;
}

void tui_com_chart_set_point(tui_obj_t *com_chart, int32_t index, int32_t value)
{
	int w;
	tui_com_chart_attri_t *attri_me;
	tui_canvas_attri_t attri_bg = { 0 };

	if (com_chart == NULL) {
		printf("tui_com_chart_set_point L%d: faile\n", __LINE__);
		return;
	}

	attri_me = (tui_com_chart_attri_t *)tui_com_get_com_attri(com_chart);

	if (attri_me == NULL || attri_me->point_array == NULL || attri_me->chart_cursor_obj == NULL) {
		printf("tui_com_chart_set_point L%d: faile\n", __LINE__);
		return;
	}
	
	tui_canvas_get_attri(attri_me->bg_chart_obj, &attri_bg);

	w = attri_bg.obj.size.width / (attri_me->point_num-1);

	attri_me->point_array[index].x = w * index;
	attri_me->point_array[index].y = attri_bg.obj.size.height - value;

	tui_line_set_some_points_line(attri_me->chart_line_obj, attri_me->point_array, attri_me->point_num, 0);

	tui_obj_set_x(attri_me->chart_cursor_obj[index], attri_me->point_array[index].x - 5);
	tui_obj_set_y(attri_me->chart_cursor_obj[index], attri_me->point_array[index].y - 5);
}

tui_obj_t * tui_com_chart_create_json(tui_obj_t * par, tJSON* attri_json, tui_map_cb_t map_cb[])
{
	tui_obj_t *ret;
	tui_com_chart_attri_t attri = { 0 };
	tJSON *item, *array;
	int32_t num, i;

	if (attri_json == NULL) {
		printf("tui_com_chart_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	ret = tui_com_chart_create(par);
	if (ret == NULL) {
		printf("tui_com_chart_create_json L%d: faile\n", __LINE__);
		return NULL;
	}

	num = tJSON_GetArraySize(attri_json);
	for (i = 0; i < num; i++) {
		item = tJSON_GetArrayItem(attri_json, i);
		if (item->type == tJSON_Object) {
			printf("tui_com_chart_create_json L%d: don't support child object\n", __LINE__, item->string);
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
			} else if (strcmp(item->string, "point_num") == 0)
				attri.point_num = item->valueint;
			else if (strcmp(item->string, "cb") == 0)
				;
			else
				printf("tui_com_chart_create_json L%d: don't support item: %s\n", __LINE__, item->string);
		}
	}

	attri.cb = tui_com_get_func(attri.obj.obj_id, map_cb);

	tui_com_chart_set_attri(ret, &attri);

	return ret;
}
