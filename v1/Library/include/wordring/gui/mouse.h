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
	struct button_
	{
		bool left : 1;
		bool middle : 1;
		bool right : 1;
	};

	struct state_
	{
		bool shift : 1;
		bool ctrl : 1;
		bool left : 1;
		bool middle : 1;
		bool right : 1;
	};

	point_int pt;
	button_ button; // left, middle, right
	state_ state; // shift, ctrl, left, middle, right

	mouse(int32_t x_, int32_t y_) : pt(x_, y_)
	{
	}
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_MOUSE_H
