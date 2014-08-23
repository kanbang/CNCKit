/**
 * @file    wordring/gui/canvas.cpp
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

#include <wordring/gui/canvas.h>
#include <wordring/gui/detail/native_canvas.h>
#include <wordring/gui/detail/win32/win32_canvas.h>

#include <wordring/gui/shape_int.h>
#include <wordring/graphics/color.h>
#include <wordring/gui/font.h>

using namespace wordring::gui;


canvas::canvas() : m_native(new detail::native_canvas_impl)
{
}

canvas::canvas(std::unique_ptr<detail::native_canvas>&& cv) : m_native(std::move(cv))
{
}

canvas::canvas(canvas&& cv) : m_native(std::move(cv.m_native))
{
}

void canvas::operator=(canvas&& cv)
{
	m_native = std::move(cv.m_native);
}

detail::native_canvas& canvas::get_native()
{
	return *m_native;
}

detail::native_canvas* canvas::operator ->()
{
	return m_native.get();
}

