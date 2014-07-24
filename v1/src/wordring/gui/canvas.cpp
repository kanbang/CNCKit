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

detail::native_canvas& canvas::get_native_canvas()
{
	return *m_native;
}

void canvas::draw_string(std::string str, point_int pt)
{
	m_native->draw_string(str, pt);
}

void canvas::draw_string(std::wstring str, point_int pt)
{
	m_native->draw_string(str, pt);
}




