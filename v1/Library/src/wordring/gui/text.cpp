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

#include <wordring/gui/window_service.h>

#include <wordring/gui/text.h>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_text.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------


#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

using namespace wordring::gui;

// text_render ----------------------------------------------------------------

text_render::text_render()
	: m_native(std::make_unique<detail::native_text_render_impl>())
{

}


detail::native_text_render* text_render::operator->()
{
	return m_native.get();
}

detail::native_text_render const* text_render::operator->() const
{
	return m_native.get();
}

// text_service ---------------------------------------------------------------

text_service::text_service()
	: m_native(std::make_unique<detail::native_text_service_impl>())
	, m_default_render(std::make_shared<text_render>())
{

}

detail::native_text_service* text_service::get_native()
{
	return m_native.get();
}

render::store text_service::get_default_render()
{
	return m_default_render;
}

// text -----------------------------------------------------------------------

text::text()
	: m_native(std::make_unique<detail::native_text_impl>())
{

}

detail::native_text* text::get_native()
{
	return m_native.get();
}

detail::native_text const* text::get_native() const
{
	return m_native.get();
}

detail::native_text* text::operator->()
{
	return m_native.get();
}

detail::native_text const* text::operator->() const
{
	return m_native.get();
}

// text_control ---------------------------------------------------------------

text_control::text_control()
	: control(rect_int(0, 0, 300, 200))
{
	text_service &ts = find_service()->get_text_service();
	set_render(ts.get_default_render());
}

void text_control::set_string(std::wstring s)
{
	m_text->set_string(s);
}

text& text_control::get_text()
{
	return m_text;
}

void text_control::do_paint(canvas& cv)
{
}

void text_control::do_size(size_int size)
{
	text_render* tr = static_cast<text_render*>(m_render.get());
	(*tr)->set_text(m_text.get_native());
}










