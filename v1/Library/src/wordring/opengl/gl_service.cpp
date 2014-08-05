/**
 * @file    wordring/opengl/gl_service.cpp
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

#include <wordring/opengl/gl_service.h>
#include <wordring/gui/canvas.h>
#include <wordring/gui/window.h>

#ifdef _WIN32
#include <wordring/opengl/detail/win32/win32_gl_service.h>
#endif // _WIN32

using namespace wordring::opengl;

gl_service::gl_service() : m_native(new detail::native_gl_service_impl())
{

}

gl_service::gl_service(wordring::gui::window& w, int flg, int depth, int bpp)
	: m_native(new detail::native_gl_service_impl(w, flg, depth, bpp))
{

}

gl_service::gl_service(wordring::gui::canvas& cv, int flg, int depth, int bpp)
	: m_native(new detail::native_gl_service_impl(cv, flg, depth, bpp))
{

}

gl_service::~gl_service()
{

}

void gl_service::assign(wordring::gui::window& w, int flg, int depth, int bpp)
{
	m_native->assign(w, flg, depth, bpp);
}

void gl_service::assign(wordring::gui::canvas& cv, int flg, int depth, int bpp)
{
	m_native->assign(cv, flg, depth, bpp);
}

void gl_service::make_current(wordring::gui::canvas& cv)
{
	m_native->make_current(cv);
}

void gl_service::unmake_current(wordring::gui::canvas& cv)
{
	m_native->unmake_current(cv);
}

