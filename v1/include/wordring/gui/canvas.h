/**
 * @file    wordring/gui/canvas.h
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

#ifndef WORDRING_CANVAS_H
#define WORDRING_CANVAS_H

#include <wordring/gui/detail/native_canvas.h>
#include <wordring/geometry/shape.h>

#include <memory>
#include <string>

namespace wordring
{
namespace gui
{

class canvas
{
protected:
	std::unique_ptr<detail::native_canvas> m_native;

public:
	canvas();

	canvas(std::unique_ptr<detail::native_canvas>&& cv);

	void draw(std::string str, point_int pt);

};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CANVAS_H
