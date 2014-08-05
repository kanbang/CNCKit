/**
 * @file    wordring/opengl/detail/native_gl_service.h
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
#ifndef WORDRING_NATIVE_GL_SERVICE_H
#define WORDRING_NATIVE_GL_SERVICE_H

#include <wordring/gui/canvas.h>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_service
{
public:
	native_gl_service() { }
	virtual ~native_gl_service() { }

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

#endif // WORDRING_NATIVE_GL_SERVICE_H
