/**
 * @file    wordring/gui/button.h
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

#ifndef WORDRING_BUTTON_H
#define WORDRING_BUTTON_H

#include <wordring/wordring.h>

#include <wordring/gui/control.h>
#include <wordring/gui/style.h>
#include <wordring/gui/mouse.h>
#include <wordring/gui/shape_int.h>

#include <wordring/gui/color.h>

#include <string>
#include <functional>

namespace wordring
{
namespace gui
{

class dummy;

class button : public control
{
public:
	enum : int32_t
	{
		up = 0,
		down = 1,
		
		style_specific = style::control_specific,
		hover_bg_color,
		down_bg_color,
	};

	struct state
	{
		bool hover : 1;
		uint32_t button : 1;

		state();
	};

	std::function<bool(mouse&)> on_click;
protected:
	state m_state;

protected:
	button(rect_int rc);

public:
	static control::store create(rect_int rc);

	static control::store create(int32_t x, int32_t y, int32_t cx, int32_t cy);

	static style* load_default(style_service &ss);

	virtual bool do_mouse_down(mouse &m);

	/// マウス・ポインタがコントロールに入ったとき呼び出されます
	virtual void do_mouse_over(mouse &m);

	/// マウス・ポインタがコントロールから出たとき呼び出されます
	virtual void do_mouse_out(mouse &m);

	virtual bool do_mouse_up(mouse &m);

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_BUTTON_H
