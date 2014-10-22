/**
 * @file    wordring/gui/text.cpp
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

#include <wordring/wordring.h>

#include <wordring/gui/text.h>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_text.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------


#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

using namespace wordring::gui;

// text_service ---------------------------------------------------------------

text_service::text_service()
	: m_native(std::make_unique<detail::native_text_service_impl>())
{

}

detail::native_text_service* text_service::get_native()
{
	return m_native.get();
}

// text_control ---------------------------------------------------------------

text_control::text_control()
	: control(rect_int(0, 0, 300, 200))
	, m_text(std::make_unique<detail::native_text_impl>())
{

}

void text_control::set_string(std::wstring s)
{
	m_text->set_string(s);
}

detail::native_text* text_control::get_native_text()
{
	return m_text.get();
}


// text_render ----------------------------------------------------------------

text_render::text_render()
	: m_native(std::make_unique<detail::native_text_render_impl>())
{

}













