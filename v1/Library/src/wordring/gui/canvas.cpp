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

detail::native_canvas& canvas::get_native_canvas()
{
	return *m_native;
}

void canvas::set_viewport(rect_int rc)
{
	m_native->set_viewport(rc);
}

void canvas::draw_line(
	point_int pt1, point_int pt2, int32_t width, rgb_color rgb)
{
	m_native->draw_line(pt1, pt2, width, rgb);
}

void canvas::draw_rect(rect_int rc, int32_t width, rgb_color rgb)
{
	m_native->draw_rect(rc, width, rgb);
}

void canvas::fill_rect(rect_int rc, rgb_color rgb)
{
	m_native->fill_rect(rc, rgb);
}

void canvas::draw_string(
	std::string str, point_int pt, rgb_color rgb, font* f)
{
	m_native->draw_string(str, pt, rgb, f);
}

void canvas::draw_string(
	std::wstring str, point_int pt, rgb_color rgb, font* f)
{
	m_native->draw_string(str, pt, rgb, f);
}




