/**
 * @file    wordring/opengl/gl_canvas.h
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

#ifndef WORDRING_GL_CANVAS_H
#define WORDRING_GL_CANVAS_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>
#include <wordring/gui/canvas.h>
//#include <wordring/opengl/detail/native_gl_canvas.h>

#include <memory>

namespace wordring
{
namespace opengl
{
namespace detail
{
class native_gl_canvas;
}
class dummy;

class gl_canvas
{
protected:
	std::unique_ptr<detail::native_gl_canvas> m_native;

public:
	gl_canvas(wordring::gui::canvas& cv, wordring::gui::size_int size);
	virtual ~gl_canvas();
};


} // namespace opengl
} // namespace wordring

#endif // WORDRING_GL_CANVAS_H
