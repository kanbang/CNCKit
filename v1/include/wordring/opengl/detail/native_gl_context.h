/**
 * @file    wordring/opengl/detail/native_gl_context.h
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
#ifndef WORDRING_NATIVE_GL_CONTEXT_H
#define WORDRING_NATIVE_GL_CONTEXT_H

#include <wordring/gui/canvas.h>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_context
{
public:
	native_gl_context() { }
	virtual ~native_gl_context() { }

	virtual void assign(
		wordring::gui::window& w, int flg, int depth, int bpp) = 0;
	virtual void assign(
		wordring::gui::canvas& cv, int flg, int depth, int bpp) = 0;
	

	virtual void make_current(wordring::gui::canvas& cv) = 0;
	virtual void unmake_current(wordring::gui::canvas& cv) = 0;
};

} // namespace detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_NATIVE_GL_CONTEXT_H
