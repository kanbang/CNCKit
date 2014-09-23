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

#include <cassert>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_font.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------

#include <wordring/gui/detail/x11/x11_font.h>

#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

using namespace wordring::gui;

// font -----------------------------------------------------------------------

font::font(uint32_t code, std::wstring const &face)
	: m_native(detail::native_font_impl::create())
	, m_code(code)
	, m_face(face)
{
	m_native->set_public(this);
}

font::store font::create(uint32_t code, std::wstring const &face)
{
	return store(new font(code, face));
}

uint32_t font::create_code(
	int32_t size, int32_t family, int32_t weight, bool italic_, uint32_t face)
{
	uint32_t code =
		  (size << size_shift)
		| family
		| ((weight / 100) << weight_shift)
		| (italic_ ? italic : 0)
		| face;

	return code;
}

detail::native_font* font::get_native()
{
	return m_native.get();
}

void font::attach(detail::native_canvas const *cv)
{
	get_native()->attach(cv);
}

void font::detach()
{
	m_native = nullptr;
}

uint32_t font::get_code() const
{
	return m_code;
}

void font::set_code(uint32_t code)
{
	assert(m_code == 0);
	m_code = code;
}

int32_t font::get_size() const
{
	return (m_code & size_mask) >> size_shift;
}

int32_t font::get_weight() const
{
	return ((m_code & weight_mask) >> weight_shift) * 100;
}

bool font::is_italic() const
{
	return (m_code & italic_mask) ? true : false;
}

int32_t font::get_family() const
{
	return m_code & family_mask;
}

std::wstring const& font::get_face() const
{
	return m_face;
}

// font_service ---------------------------------------------------------------

font_service::font_service() : m_window_service(nullptr)
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

uint32_t font_service::create(
	int32_t size, int32_t family, int32_t weight, bool italic, std::wstring face)
{
	uint32_t atom = m_atom_service.get_atom(face);
	uint32_t code = font::create_code(size, family, weight, italic, atom);

	font::store f = m_storage[code];

	if (!f)
	{
		f = font::create(code, face);
		m_storage[code] = f;
	}

	return code;
}

font::store font_service::find(
	int32_t size, int32_t family, int32_t weight, bool italic, uint32_t face)
{
	uint32_t code = font::create_code(size, family, weight, italic, face);
	font::store result = m_storage[code];

	return result;
}

font::store font_service::find(uint32_t code)
{
	return m_storage[code];
}




