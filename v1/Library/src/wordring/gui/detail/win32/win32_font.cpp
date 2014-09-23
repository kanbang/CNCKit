/**
 * @file   wordring/gui/detail/win32/win32_font.cpp 
 *
 * @brief   Win32用フォント実装ファイル
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

#ifdef WORDRING_WS_WIN

#include <wordring/gui/canvas.h>
#include <wordring/gui/shape_int.h>

#include <cassert>

#include <wordring/gui/detail/win32/win32_font.h>
#include <wordring/gui/detail/win32/win32_canvas.h>

#include <Windows.h>

using namespace wordring::gui;
using namespace wordring::gui::detail;

native_font_impl::native_font_impl() : m_hfont(NULL)
{
}

native_font_impl::~native_font_impl()
{
	if (m_hfont != NULL)
	{
		::DeleteObject(m_hfont);
	}
}

native_font::store native_font_impl::create()
{
	return native_font::store(new native_font_impl());
}

HFONT native_font_impl::create(native_canvas const *cv, LONG dh)
{
	HFONT hfont = NULL;

	font const *public_ = get_public();

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

	std::wstring const &family = public_->get_face();

	// dhはInternalLeadingを引くために使用
	// 1インチには約72ポイントが含まれます
	//height = -::MulDiv(fc.size, ::GetDeviceCaps(hdc, LOGPIXELSY), 72);
	height = public_->get_size() + dh;

	weight = public_->get_weight();

	italic = public_->is_italic() ? TRUE : FALSE;

	switch (public_->get_family())
	{
	case font::sans_serif: pitch_and_family = FF_SWISS;      break;
	case font::serif:      pitch_and_family = FF_ROMAN;      break;
	case font::cursive:    pitch_and_family = FF_SCRIPT;     break;
	case font::fantasy:    pitch_and_family = FF_DECORATIVE; break;
	case font::monospace:  pitch_and_family = FF_MODERN;     break;
	}
	
	// hdc変更不可
	native_canvas_impl const *ncv = static_cast<native_canvas_impl const*>(cv);
	HDC hdc = const_cast<native_canvas_impl*>(ncv)->get_handle();

	hfont = ::CreateFont(
		height, width, escapement, orientation, weight,
		italic, under_line, strike_out, char_set, output_precision, clip_precision,
		quality, pitch_and_family, family.c_str());
	assert(hfont != NULL);

	// 内部レディングを測る
	HGDIOBJ old = ::SelectObject(hdc, hfont);
	BOOL ret = ::GetTextMetrics(hdc, &m_tm);
	assert(ret != 0);
	::SelectObject(hdc, old);

	return hfont;
}

void native_font_impl::attach(native_canvas const *cv)
{
	HFONT hfont = create(cv, 0);

	// 内部レディングが0以外の場合、フォントを消して再作成する
	if (m_tm.tmInternalLeading != 0)
	{
		// (フォントの高さに対する内部レディングの割合) + 1
		float dh = float(m_tm.tmInternalLeading) / m_tm.tmHeight + 1;

		BOOL ret = ::DeleteObject(hfont);
		assert(ret != 0);
		hfont = create(cv, LONG(m_tm.tmInternalLeading * dh));
	}

	m_hfont = hfont;
}

HFONT native_font_impl::get_handle(native_canvas const *cv)
{
	if (m_hfont == NULL)
	{
		attach(cv);
	}

	return m_hfont;
}

point_int native_font_impl::get_offset() const
{
	return point_int(0, -m_tm.tmInternalLeading);
}

#endif // WRODRING_WS_WIN
