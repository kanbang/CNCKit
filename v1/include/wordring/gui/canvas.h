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

#include <wordring/debug.h>

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

	explicit canvas(std::unique_ptr<detail::native_canvas>&& cv);

	canvas(canvas&& cv);

	void operator=(canvas&& cv);

	detail::native_canvas& get_native_canvas();

	void draw_string(std::string str, point_int pt);
	void draw_string(std::wstring str, point_int pt);

};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CANVAS_H
