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
protected:
	rgb_color m_fg_color, m_bg_color;

	std::function<void(mouse&)> on_click;

public:
	button(rect_int rc);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_BUTTON_H
