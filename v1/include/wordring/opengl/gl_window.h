/**
 * @file    wordring/opengl/gl_window.h
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

#ifndef WORDRING_GL_WINDOW_H
#define WORDRING_GL_WINDOW_H

#include <wordring/gui/container_window.h>
#include <wordring/opengl/detail/native_gl_window.h>


namespace wordring
{
namespace opengl
{

class dummy;

class gl_window : public wordring::gui::container_window
{
protected:
public:
	gl_window();
	gl_window(detail::native_gl_window* nw);
	virtual ~gl_window();
};













} // namespace opengl
} // namespace wordring

#endif // WORDRING_GL_WINDOW_H
