/**
 * @file    wordring/opengl/detail/native_gl_window.h
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

#ifndef WORDRING_NATIVE_GL_WINDOW_H
#define WORDRING_NATIVE_GL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_window : public wordring::gui::detail::native_window
{
public:
	native_gl_window() { }
	virtual ~native_gl_window() { }

	virtual void make_current() = 0;
	virtual void unmake_current() = 0;

	virtual void create_context() = 0;
	virtual void delete_context() = 0;
};











} // detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_NATIVE_GL_WINDOW_H
