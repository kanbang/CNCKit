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

#include <wordring/debug.h>

#include <wordring/gui/control.h>
#include <wordring/gui/mouse.h>

#include <wordring/gui/shape_int.h>

#include <wordring/graphics/color.h>

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
	enum
	{
		up, down, hover,
	};

protected:
	std::function<bool(mouse&)> on_click;
	int32_t m_state;

protected:
	button(rect_int rc);

public:
	static control::store create(rect_int rc);

	static control::store create(int32_t x, int32_t y, int32_t cx, int32_t cy);

	virtual bool do_mouse_down(mouse &m);

	virtual bool do_mouse_up(mouse &m);

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_BUTTON_H
