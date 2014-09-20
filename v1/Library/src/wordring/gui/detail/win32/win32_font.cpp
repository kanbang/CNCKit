/**
 * @file   wordring/gui/detail/win32/win32_font.cpp 
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

#ifdef _WIN32

#include <wordring/debug.h>

#include <wordring/gui/detail/win32/win32_font.h>

#include <Windows.h>

#include <cassert>

using namespace wordring::gui;
using namespace wordring::gui::detail;

native_font_impl::native_font_impl(font_conf fc)
	: m_hfont(NULL)
{
}

native_font_impl::~native_font_impl()
{
	if (m_hfont != NULL)
	{
		::DeleteObject(m_hfont);
	}
}

native_font::store native_font_impl::create(font_conf fc)
{
	return native_font::store(new native_font_impl(fc));
}

HFONT native_font_impl::get_handle(HDC hdc)
{
	if (m_hfont == NULL)
	{
		attach(hdc, get_public()->get_conf());
	}

	return m_hfont;
}

void native_font_impl::attach(HDC hdc, font_conf const &fc)
{
	int height      = 0;
	int width       = 0;
	int escapement  = 0;
	int orientation = escapement;
	int weight      = 400;

	DWORD italic           = FALSE;
	DWORD under_line       = FALSE;
	DWORD strike_out       = FALSE;
	DWORD char_set         = DEFAULT_CHARSET;
	DWORD output_precision = OUT_TT_PRECIS;
	DWORD clip_precision   = CLIP_DEFAULT_PRECIS;
	DWORD quality          = DEFAULT_QUALITY;
	DWORD pitch_and_family = DEFAULT_PITCH;

	std::wstring family;

	if (fc.size != 0)
	{
		// 1インチには約72ポイントが含まれます
		height = -::MulDiv(fc.size, ::GetDeviceCaps(hdc, LOGPIXELSY), 72);
	}

	height = 16;
	
	switch (fc.weight)
	{
	case font::normal: weight = 400;       break;
	case font::bold:   weight = 700;       break;
	default:           weight = fc.weight; break;
	}
	assert(100 <= weight && weight <= 900);

	if (fc.style == font::italic || fc.style == font::oblique)
	{
		italic = TRUE;
	}

	switch (fc.family)
	{
	case font::sans_serif: pitch_and_family = FF_SWISS;      break;
	case font::serif:      pitch_and_family = FF_ROMAN;      break;
	case font::cursive:    pitch_and_family = FF_SCRIPT;     break;
	case font::fantasy:    pitch_and_family = FF_DECORATIVE; break;
	case font::monospace:  pitch_and_family = FF_MODERN;    break;
	}
	
	if (fc.face.empty()) { family = L"Meiryo"; }
	else { family = fc.face; }

	m_hfont = ::CreateFont(
		height, width, escapement, orientation, weight,
		italic, under_line, strike_out, char_set, output_precision, clip_precision,
		quality, pitch_and_family, family.c_str());

	assert(m_hfont != NULL);
}


#endif // _WIN32
