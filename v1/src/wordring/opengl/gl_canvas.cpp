/**
 * @file    wordring/opengl/gl_canvas.cpp
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

#include <wordring/opengl/gl_canvas.h>
#include <wordring/opengl/detail/native_gl_canvas.h>

#ifdef _WIN32
#include <wordring/opengl/detail/win32/win32_gl_canvas.h>
#endif // _WIN32

using namespace wordring::opengl;

gl_canvas::gl_canvas(wordring::gui::canvas& cv, wordring::gui::size_int size)
	: m_native(new detail::native_gl_canvas_impl(cv, size))
{

}

gl_canvas::~gl_canvas()
{

}

