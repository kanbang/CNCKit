/**
 * @file    wordring/opengl/detail/win32/win32_gl_window.h
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

#ifndef WORDRING_WIN32_GL_WINDOW_H
#define WORDRING_WIN32_GL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/opengl/detail/native_gl_window.h>
#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/detail/win32/win32_window_class.h>

#include <gl/GL.h>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_window_impl : public wordring::gui::detail::native_window_impl
{
protected:
	HGLRC m_hglrc;
};


} // namespace detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_WIN32_GL_WINDOW_H
