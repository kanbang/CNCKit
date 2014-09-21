/**
 * @file    wordring/gui/font.cpp
 *
 * @brief   フォントの実装ファイル
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

#include <wordring/gui/font.h>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_font.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------

#include <wordring/gui/detail/x11/x11_font.h>

#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

using namespace wordring::gui;

// font_conf ------------------------------------------------------------------

font_conf::font_conf()
	: style(font::normal)
	, variant(font::normal)
	, weight(font::normal)
	, size(font::normal)
	, line_height(font::normal)
	, family(font::serif)
{
	
}

// font -----------------------------------------------------------------------

font::font(font_conf const &fc)
	: m_native(detail::native_font_impl::create(fc))
	, m_font_conf(fc)
{
	m_native->set_public(this);
}

detail::native_font* font::get_native()
{
	return m_native.get();
}

font_conf const& font::get_conf() const
{
	return m_font_conf;
}

void font::attach(detail::native_canvas const *cv)
{
	get_native()->attach(cv);
}

void font::detach()
{
	m_native = nullptr;
}

// font_hash ------------------------------------------------------------------

size_t font_hash::operator ()(font const &f) const
{
	std::wstring val;
	font_conf const &fc = f.get_conf();

	val.push_back(fc.style);
	val.push_back(fc.variant);
	val.push_back(fc.weight);
	val.push_back(fc.size);
	val.push_back(fc.line_height);
	val.push_back(fc.family);
	val.append(fc.face);

	return std::hash<std::wstring>()(val);
}

// font_equal_to --------------------------------------------------------------

bool font_equal_to::operator()(font const &lhs, font const &rhs) const
{
	font_conf const &lfc = lhs.get_conf();
	font_conf const &rfc = rhs.get_conf();

	return lfc.style       == rfc.style
		&& lfc.variant     == rfc.variant
		&& lfc.weight      == rfc.weight
		&& lfc.size        == rfc.size
		&& lfc.line_height == rfc.line_height
		&& lfc.family      == rfc.family
		&& lfc.face        == rfc.face;
}

// font_service ---------------------------------------------------------------

font_service::font_service()
	: m_window_service(nullptr)
{

}

window_service* font_service::get_window_service()
{
	return m_window_service;
}

void font_service::set_window_service(window_service *ws)
{
	m_window_service = ws;
}







