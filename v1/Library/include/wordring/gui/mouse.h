/**
 * @file    wordring/gui/mouse.h
 *
 * @brief   
 *
 * @details
 *          
 *          
 *          
 *          
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_MOUSE_H
#define WORDRING_MOUSE_H

#include <wordring/wordring.h>

#include <wordring/gui/shape_int.h>

namespace wordring
{
namespace gui
{

class control;        // 先行宣言
class window_service; // 先行宣言

struct mouse
{
	struct button_
	{
		bool left   : 1;
		bool middle : 1;
		bool right  : 1;
	};

	struct state_
	{
		bool shift  : 1;
		bool ctrl   : 1;
		bool left   : 1;
		bool middle : 1;
		bool right  : 1;
	};

	point_int pt;
	button_   button; // left, middle, right
	state_    state; // shift, ctrl, left, middle, right

	mouse(int32_t x_, int32_t y_) : pt(x_, y_)
	{
	}
};

class mouse_service
{
private:
	window_service *m_window_service;

	control *m_capture;

	control *m_hover;
	control *m_active;

public:
	mouse_service();

	window_service* get_window_service();

	void set_window_service(window_service *ws);

	control* get_hover();

	control* set_hover(control *c, mouse &m);

	control* release_hover();

	control* get_capture();

	control* set_capture(control *c);

	void process_mouse_down(control *w, mouse &m);

	void process_mouse_up(control *w, mouse &m);

	void process_mouse_enter(control *w, mouse &m);

	void process_mouse_leave(control *w);

	void process_mouse_move(control *w, mouse &m);

	void process_mouse_wheel(control *w, mouse &m);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_MOUSE_H
