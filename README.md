<h1 align="center"> TUI组件的实现和使用 </h1>

## 组件描述
TUI组件是通过基础控件实现复杂界面功能模块，用户也可以自定义组件模块（参考template.c），或者根据自己需求修改系统提供的组件。组件里面的json解析函数，是方便将来UIStudio工具对组件的扩展。用户在使用的时候需要包含头文件`tui_component.h`和`tjson.h`，然后将文件夹📁component里面的`.C`文件参加编译，最后结合`TUISYS/tui_project`仓库里面的tui库文件，在tui内核初始化完成后调用。

## 日历（calendar.c）
只需要设置日历组件的坐标位置和宽高，其中calendar.c里面有中国农历的计算公式，目前没有使用，方便后面扩展。测试代码如下：
``` c
void tui_com_calendar_test(void)
{
	tui_obj_t * obj;
	tui_com_calendar_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	obj = tui_com_calendar_create(tui_layer_top());

	/* 通用属性 */
	attri.obj.pt.x = 200;
	attri.obj.pt.y = 100;
	attri.obj.size.width = 600;
	attri.obj.size.height = 400;

	tui_com_calendar_set_attri(obj, &attri);
}
```
实际效果如图：
<p align="center">
<img src="https://gitee.com/tuisys/image/raw/main/calendar.gif">
</p>

## 时钟（clock.c）
设置时钟组件的坐标位置和宽高，将`.png`图片资源通过UIStudio工具导入打包，重点设置图片对应的路径和旋转图片的旋转坐标，测试代码如下：
``` c
void tui_com_clock_test(void)
{
	tui_obj_t * obj;
	tui_com_clock_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	obj = tui_com_clock_create(tui_layer_top());

	/* 通用属性 */
	attri.obj.pt.x = 1024-382;
	attri.obj.pt.y = 60;
	attri.obj.size.width = 382;
	attri.obj.size.height = 382;

	attri.bg_img_path = "V:\\image\\clock_bg.png";
	attri.bg_img_pt.x = 0;
	attri.bg_img_pt.y = 0;

	attri.point_h_img_path = "V:\\image\\point_h.png";
	attri.point_h_img_pt.x = 186;
	attri.point_h_img_pt.y = 108;
	attri.point_h_img_rotate_pt.x = 6;
	attri.point_h_img_rotate_pt.y = 78;

	attri.point_m_img_path = "V:\\image\\point_m.png";
	attri.point_m_img_pt.x = 185;
	attri.point_m_img_pt.y = 66;
	attri.point_m_img_rotate_pt.x = 7;
	attri.point_m_img_rotate_pt.y = 120;

	attri.point_s_img_path = "V:\\image\\point_s.png";
	attri.point_s_img_pt.x = 185;
	attri.point_s_img_pt.y = 71;
	attri.point_s_img_rotate_pt.x = 7;
	attri.point_s_img_rotate_pt.y = 115;
	tui_com_clock_set_attri(obj, &attri);
}
```
效果如图：
<p align="center">
<img src="https://gitee.com/tuisys/image/raw/main/clock.gif">
</p>

## 仪表盘（dial.c）
设置仪表盘组件的坐标位置和宽高，将`.png`图片资源通过UIStudio工具导入打包，重点设置图片对应的路径和旋转图片的旋转坐标，测试代码如下：
``` c
void tui_com_dial_test(void)
{
	tui_obj_t * obj;
	tui_com_dial_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	obj = tui_com_dial_create(tui_layer_top());

	/* 通用属性 */
	attri.obj.pt.x = 412;
	attri.obj.pt.y = 200;
	attri.obj.size.width = 382;
	attri.obj.size.height = 382;

	attri.bg_img_path = "V:\\image\\dial_bg.png";
	attri.bg_img_pt.x = 0;
	attri.bg_img_pt.y = 0;
	attri.point_img_path = "V:\\image\\dial_point.png";
	attri.point_img_pt.x = 186;
	attri.point_img_pt.y = 50;
	attri.point_img_rotate_pt.x = 5;
	attri.point_img_rotate_pt.y = 165;
	strcpy(attri.txt, "0");
	attri.txt_pt.x = 150;
	attri.txt_pt.y = 180;
	attri.txt_fnt_size = 60;
	attri.txt_fnt_color = 0xFFFFFFFF;
	tui_com_dial_set_attri(obj, &attri);
	tui_com_dial_set_point_angle(obj, 100);
}
```
效果如图：
<p align="center">
<img src="https://gitee.com/tuisys/image/raw/main/dial.gif">
</p>

## 折线表（chart.c）
设置折线表组件的坐标位置和宽高，设置折线的点个数，通过`tui_com_chart_set_point()`函数设置对应索引点的位置，表的底部值是`0`，测试代码如下：
``` c
void tui_com_chart_test(void)
{
	tui_obj_t * obj;
	tui_com_chart_attri_t attri = { 0 };//注意先清空结构体，避免随机值
	obj = tui_com_chart_create(tui_layer_top());

	/* 通用属性 */
	attri.obj.pt.x = 5;
	attri.obj.pt.y = 65;
	attri.obj.size.width = 595;
	attri.obj.size.height = 295;
	attri.point_num = 10;

	tui_com_chart_set_attri(obj, &attri);

	tui_com_chart_set_point(obj, 0, 50);
	tui_com_chart_set_point(obj, 1, 150);
	tui_com_chart_set_point(obj, 2, 20);
	tui_com_chart_set_point(obj, 3, 0);
	tui_com_chart_set_point(obj, 4, 200);
	tui_com_chart_set_point(obj, 5, 150);
	tui_com_chart_set_point(obj, 6, 80);
	tui_com_chart_set_point(obj, 7, 10);
	tui_com_chart_set_point(obj, 8, 60);
	tui_com_chart_set_point(obj, 9, 250);
}
```
效果如图：
<p align="center">
<img src="https://gitee.com/tuisys/image/raw/main/chart.gif">
</p>

## 创建组件模板（template.c）
该文件没有实际功能，主要是提供给开发者扩展自己的组件，按照这个模板文件书写代码，规范格式，避免遗漏代码。
