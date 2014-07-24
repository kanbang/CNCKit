/**
 * @file    wordring/opengl/detail/win32/win32_gl_canvas.h
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
#ifndef WORDRING_WIN32_GL_CANVAS_H
#define WORDRING_WIN32_GL_CANVAS_H

#include <wordring/opengl/detail/native_gl_canvas.h>

#include <wordring/gui/detail/native_canvas.h>

#include <Windows.h>

#include <GL/glew.h>
#include <GL/GL.h>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_canvas_impl : public native_gl_canvas
{
protected:
	wordring::gui::detail::native_canvas& m_native_canvas;

public:
	native_gl_canvas_impl(
		wordring::gui::canvas& cv, wordring::gui::size_int size)
		: m_native_canvas(cv.get_native_canvas()){}
};


} // namespace detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_WIN32_GL_CANVAS_H
