/**
 * @file    wordring/gui/button.cpp
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

#include <wordring/debug.h>

#include <wordring/gui/button.h>

#include <wordring/gui/style.h>
#include <wordring/gui/window_service.h>

using namespace wordring::gui;

button::button(rect_int rc)
	: control(rc)
	, m_state(up)
{
}

control::store button::create(rect_int rc)
{
	return control::store(new button(rc));
}

control::store button::create(int32_t x, int32_t y, int32_t cx, int32_t cy)
{
	return control::store(new button(rect_int(x, y, cx, cy)));
}

bool button::do_mouse_down(mouse &m)
{
	m_state = down;

	repaint();

	return true;
}

bool button::do_mouse_up(mouse &m)
{
	m_state = up;

	repaint();

	bool result = (on_click) ? on_click(m) : false;

	return result;
}

void button::do_paint(canvas &cv)
{
	window_service *ws = find_service();
	assert(ws);
	style_service &ss = ws->get_style_service();
	style_cache sc = ss.find_styles(this);
	color_value const *bg =
		static_cast<color_value const*>(sc.find(style::background_color));

	cv->fill_rect(rect_int(-10, -10, 300, 300), rgb_color(0x0, 0x0, 0xFF));
}

