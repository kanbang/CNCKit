/**
* @file    wordring/gui/native/native_canvas.h
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

#include <wordring/geometry/shape.h>

#include <string>

#ifndef WORDRING_NATIVE_CANVAS_H
#define WORDRING_NATIVE_CANVAS_H

namespace wordring
{
namespace gui
{
namespace detail
{


class native_canvas
{
public:
	native_canvas() { }
	virtual ~native_canvas() { }

	virtual void draw(std::string str, point_int pt) = 0;
};




} // detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_CANVAS_H
