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
/*
style* button::load_default(style_service &ss)
{
	style *result = ss.insert(typeid(button));

	font_service &fs = ss.get_font_service();

	uint32_t f = fs.create(13, font::sans_serif, 400, false, L"Meiryo UI");

	// 通常時
	result->insert(style::state::normal | style::font, f);
	result->insert(
		style::state::normal | style::color, color_rgb(0xFF, 0xFF, 0xFF, 0xFF));
	result->insert(
		style::state::normal | style::background::color,
		color_rgb(0x40, 0x40, 0x40, 0xFF));

	// アクティブ
	result->insert(style::state::active | style::font, f);
	result->insert(
		style::state::active | style::color, color_rgb(0, 0, 0, 0xFF));
	result->insert(
		style::state::active | style::background::color,
		color_rgb(0xA0, 0xA0, 0xA0, 0xFF));

	// ホバー
	result->insert(style::state::hover | style::font, f);
	result->insert(
		style::state::hover | style::color, color_rgb(0xFF, 0xFF, 0xFF, 0xFF));
	result->insert(
		style::state::hover | style::background::color,
		color_rgb(0x80, 0x80, 0x80, 0xFF));

	// フォーカス
	result->insert(style::state::focus | style::font, f);
	result->insert(
		style::state::focus | style::color, color_rgb(0xFF, 0xFF, 0x80, 0xFF));
	result->insert(
		style::state::focus | style::background::color,
		color_rgb(0x80, 0x80, 0x80, 0xFF));

	return result;
}
*/
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

	style::state st = style::state::normal;
	if (m_state.button == down) st = style::state::active;
	else if (m_state.hover) st = style::state::hover;

//	f = sc.find(state | style::font, f);


	style::store s;
	control_style const *cs = find_style(st);
	if (cs == nullptr)
	{
		s = control_style::create(style::state::normal);
		cs = static_cast<control_style const*>(s.get());
	}
	color_rgb bg_color = cs->background_color;
	color_rgb color = cs->color;

	font::store f = cs->font;

	cv->fill_rect(
		rect_int(point_int(0, 0), get_size()), bg_color);
	//font f(16);// , font::cursive, 400, false, L"");

	cv->draw_string(
		L"AQgqプロポーショナル → 16px", point_int(0, 0), color, f.get());
}
