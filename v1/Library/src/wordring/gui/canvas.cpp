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

#include <wordring/wordring.h>

#include <wordring/gui/shape_int.h>
#include <wordring/gui/color.h>
#include <wordring/gui/font.h>

#include <wordring/gui/canvas.h>
#include <wordring/gui/detail/native_canvas.h>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_canvas.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------

#include <wordring/gui/detail/x11/x11_canvas.h>

#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------


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

detail::native_canvas* canvas::get_native()
{
	return m_native.get();
}

detail::native_canvas const* canvas::get_native() const
{
	return m_native.get();
}

detail::native_canvas* canvas::operator ->()
{
	return m_native.get();
}

