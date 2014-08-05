/**
 * @file    wordring/opengl/detail/win32/win32_gl_service.h
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

#ifndef WORDRING_WIN32_GL_SERVICE_H
#define WORDRING_WIN32_GL_SERVICE_H

#include <wordring/gui/canvas.h>
#include <wordring/gui/window.h>
#include <wordring/opengl/detail/native_gl_service.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <atomic>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_service_impl : public native_gl_service
{
public:
	HGLRC m_hglrc;

public:
	native_gl_service_impl();

	native_gl_service_impl(
		wordring::gui::window& w, int flg, int depth, int bpp);

	native_gl_service_impl(
		wordring::gui::canvas& cv, int flg, int depth, int bpp);

	virtual ~native_gl_service_impl();

	virtual void assign(wordring::gui::window& w, int flg, int depth, int bpp);
	virtual void assign(wordring::gui::canvas& cv, int flg, int depth, int bpp);

private:
	virtual void create(wordring::gui::window& w, int flg, int depth, int bpp);
	virtual void create(wordring::gui::canvas& cv, int flg, int depth, int bpp);
	virtual void create(HDC hdc, int flg, int depth, int bpp);

private:
	virtual void make_current(HDC hdc);
public:
	virtual void make_current(wordring::gui::canvas& cv);
	virtual void unmake_current(wordring::gui::canvas& cv);
	virtual void unmake_current();

private:
	/// glewInit()が未呼び出しであれば、呼び出す
	virtual void initialize(HDC hdc);

private:
	/// glewInit()が呼び出し済みであればtrue、呼び出し前はfalse
	static std::atomic_bool g_initialized;
};

} // namespace detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_WIN32_GL_SERVICE_H
