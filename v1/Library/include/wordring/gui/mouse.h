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

#include <wordring/gui/shape_int.h>

namespace wordring
{
namespace gui
{

class dummy;

struct mouse
{
	enum
	{
		shift = 0x1, ctrl = 0x2, left = 0x4, middle = 0x8, right = 0x10,
	};

	point_int pt;
	uint32_t button; // left, middle, right
	uint32_t state; // shift, ctrl, left, middle, right

	mouse(int32_t x_, int32_t y_, uint32_t button_, uint32_t state_)
		: pt(x_, y_), button(button_), state(state_)
	{
	}
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_MOUSE_H
