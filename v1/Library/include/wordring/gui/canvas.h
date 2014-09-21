/**
 * @file    wordring/gui/canvas.h
 *
 * @brief   描画用キャンバス
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

#include <wordring/wordring.h>

#include <wordring/gui/font.h>
#include <wordring/gui/shape_int.h>

#include <wordring/gui/color.h>
#include <wordring/gui/detail/native_canvas.h>

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

	void operator =(canvas&& cv);

	detail::native_canvas* get_native();

	detail::native_canvas const* get_native() const;

	detail::native_canvas* operator ->();

};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CANVAS_H
