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
#include <wordring/gui/detail/win32/win32_canvas.h>

using namespace wordring::gui;


canvas::canvas() : m_native(new detail::native_canvas_impl)
{
}

canvas::canvas(std::unique_ptr<detail::native_canvas>&& cv)
{
	m_native = std::move(cv);
}

void canvas::draw(std::string str, point_int pt)
{
	m_native->draw(str, pt);
}




