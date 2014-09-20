/**
 * @file    wordring/gui/button.cpp
 *
 * @brief   ボタン・コントロールの実装ファイル
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

#include <wordring/wordring.h>

#include <wordring/gui/button.h>

#include <wordring/gui/style.h>
#include <wordring/gui/window_service.h>

using namespace wordring::gui;

button::state::state()
	: hover(false)
	, button(button::up)
{

}

button::button(rect_int rc)
	: control(rc)
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
	m_state.button = down;

	repaint();

	return true;
}

void button::do_mouse_over(mouse &m)
{
	m_state.hover = true;
	repaint();
}

void button::do_mouse_out(mouse &m)
{
	m_state.hover = false;
	m_state.button = up;
	repaint();
}

bool button::do_mouse_up(mouse &m)
{
	m_state.button = up;

	repaint();

	bool result = (on_click) ? on_click(m) : false;

	return result;
}

void button::do_paint(canvas &cv)
{
	window_service *ws = find_service();
	assert(ws);
	style_service &ss = ws->get_style_service();
	style_cache sc = ss.find_styles(typeid(button), this);

	rgb_color bg_color(0x20, 0x20, 0x20, 0xFF);

	if (m_state.button == down)
	{
		bg_color = rgb_color(0x80, 0x80, 0x80, 0xFF);
		sc.find(style::active::bg_color, bg_color);
	}
	else if (m_state.hover)
	{
		bg_color = rgb_color(0xA0, 0xA0, 0xA0, 0xFF);
		sc.find(style::hover::bg_color, bg_color);
	}
	else
	{
		sc.find(style::bg_color, bg_color);
	}
	cv->fill_rect(
		rect_int(point_int(0, 0), get_size()), bg_color);

	rgb_color color(0xFF, 0, 0, 0xFF);

	font_conf fc;
	fc.family = font::sans_serif;
	fc.size = 12;
	font f(fc);

	cv->draw_string(L"Aプロポーショナル → 16px", point_int(0, 0), color, &f);
}
