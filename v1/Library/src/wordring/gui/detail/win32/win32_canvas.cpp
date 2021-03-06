﻿/**
 * @file    wordring/gui/detail/win32/win32_canvas.cpp
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

#ifdef WORDRING_WS_WIN

#include <wordring/gui/shape_int.h>
#include <wordring/gui/color.h>
#include <wordring/gui/font.h>

#include <wordring/gui/detail/native_window.h>

#include <cassert>

#include <wordring/gui/detail/win32/win32_canvas.h>
#include <wordring/gui/detail/win32/win32_font.h>
#include <wordring/gui/detail/win32/win32_window.h>

#include <Windows.h>

using namespace wordring::gui::detail;
using namespace wordring::gui;

// native_canvas_impl ---------------------------------------------------------

native_canvas_impl::native_canvas_impl() : m_hdc(NULL)
{
	// ::GetDC()が失敗の時NULLを返すので、初期値をNULLにしておいても
	// 互換性を保てると判断。
}

native_canvas_impl::native_canvas_impl(HDC hdc)
	: m_hdc(hdc)
{
	assert(hdc);
	::SetBkMode(m_hdc, TRANSPARENT);
}

native_canvas_impl::~native_canvas_impl()
{
	assert(m_hdc);
	::SelectClipRgn(m_hdc, NULL);
}

HDC native_canvas_impl::get_handle()
{
	assert(m_hdc);
	return m_hdc;
}

point_int native_canvas_impl::get_origin() const
{
	assert(m_hdc);
	return m_origin;
}

void native_canvas_impl::set_origin(point_int pt)
{
	assert(m_hdc);
	m_origin = pt;
}

rect_int native_canvas_impl::get_viewport() const
{
	assert(m_hdc);
	return m_viewport;
}

void native_canvas_impl::set_viewport(rect_int rc)
{
	assert(m_hdc);
	assert(
		   0 <= rc.pt.x
		&& 0 <= rc.pt.y
		&& 0 <= rc.size.cx
		&& 0 <= rc.size.cy);


	if (get_viewport() == rc) { return; }

	m_viewport = rc;

	::SetMapMode(m_hdc, MM_TEXT);
	HRGN hrgn = ::CreateRectRgn(
		rc.left(), rc.top(), rc.right() + 1, rc.bottom() + 1);
	assert(hrgn != NULL);
	int r1 = ::SelectClipRgn(m_hdc, hrgn);

	BOOL r2 = ::DeleteObject(hrgn);
	assert(r2 != NULL);
}

void native_canvas_impl::draw_line(
	point_int pt1, point_int pt2, int32_t width, color_rgb rgb)
{
	assert(m_hdc);

	pt1 += m_origin; pt2 += m_origin; // ビューポート・オフセット

	HPEN hpen = ::CreatePen(
		PS_SOLID | PS_INSIDEFRAME, width, RGB(rgb.r, rgb.g, rgb.b));
	assert(hpen != NULL);

	HGDIOBJ obj = ::SelectObject(m_hdc, hpen);
	assert(obj != NULL);

	BOOL result = ::MoveToEx(m_hdc, pt1.x, pt1.y, NULL);
	assert(result != 0);
	result = ::LineTo(m_hdc, pt2.x, pt2.y);
	assert(result != 0);

	obj = ::SelectObject(m_hdc, ::GetStockObject(BLACK_PEN));
	assert(obj != NULL);

	result = ::DeleteObject(hpen);
	assert(result != 0);
}

void native_canvas_impl::draw_rect(rect_int rc, int32_t width, color_rgb rgb)
{
	assert(m_hdc);
	/*
	point_int
		pt1(rc.left(), rc.top()), pt2(rc.right(), rc.top()),
		pt3(rc.left(), rc.bottom()), pt4(rc.right(), rc.bottom());

	draw_line(pt1, pt2, width, rgb);
	draw_line(pt1, pt3, width, rgb);
	draw_line(pt2, pt4, width, rgb);
	draw_line(pt3, pt4, width, rgb);
	*/
	rc.pt += m_origin;

	HPEN hpen = ::CreatePen(
		PS_SOLID | PS_INSIDEFRAME, width, RGB(rgb.r, rgb.g, rgb.b));
	assert(hpen != NULL);

	HGDIOBJ obj = ::SelectObject(m_hdc, hpen);
	assert(obj != NULL);

	HGDIOBJ hbrush = ::SelectObject(m_hdc, ::GetStockObject(NULL_BRUSH));
	assert(hbrush != NULL);

	BOOL result = ::Rectangle(
		m_hdc, rc.left(), rc.top(), rc.right() + 1, rc.bottom() + 1);
	assert(result != 0);

	obj = ::SelectObject(m_hdc, ::GetStockObject(BLACK_PEN));
	assert(obj != NULL);

	result = ::DeleteObject(hpen);
	assert(result != 0);
}

void native_canvas_impl::fill_rect(rect_int rc, color_rgb rgb)
{
	assert(m_hdc);

	rc.pt += m_origin;

	HBRUSH hbrush = ::CreateSolidBrush(RGB(rgb.r, rgb.g, rgb.b));
	assert(hbrush != NULL);
	HPEN hpen = ::CreatePen(
		PS_SOLID | PS_INSIDEFRAME, 1, RGB(rgb.r, rgb.g, rgb.b));
	assert(hpen != NULL);
	
	HGDIOBJ obj = ::SelectObject(m_hdc, hbrush);
	assert(obj != NULL);
	obj = ::SelectObject(m_hdc, hpen);
	assert(obj != NULL);

	BOOL result = ::Rectangle(
		m_hdc, rc.left(), rc.top(), rc.right() + 1, rc.bottom() + 1);
	assert(result != 0);

	obj = ::SelectObject(m_hdc, ::GetStockObject(BLACK_PEN));
	assert(obj != NULL);
	obj = ::SelectObject(m_hdc, ::GetStockObject(WHITE_BRUSH));
	assert(obj != NULL);

	result = ::DeleteObject(hbrush);
	assert(result != 0);
	result = ::DeleteObject(hpen);
	assert(result != 0);
}

void native_canvas_impl::get_string_extents(
	std::wstring const &str, font *f, uint32_t limit)
{
	HGDIOBJ hfont = NULL;
	if (f != nullptr)
	{
		native_font_impl *nf = static_cast<native_font_impl*>(f->get_native());
		hfont = ::SelectObject(m_hdc, nf->get_handle(this));
	}

	DWORD li = ::GetFontLanguageInfo(m_hdc);

	bool b0 = li & GCP_REORDER;
	bool b1 = li & GCP_DBCS;
	bool b2 = li & GCP_DIACRITIC;
	bool b3 = li & FLI_GLYPHS;
	bool b4 = li & GCP_GLYPHSHAPE;
	bool b5 = li & GCP_KASHIDA;
	bool b6 = li & GCP_LIGATE;
	bool b7 = li & GCP_USEKERNING;
	bool b8 = li & GCP_REORDER;

	size_t len = str.length();

	GCP_RESULTSW gcp;
	::ZeroMemory(&gcp, sizeof(gcp));
	gcp.lStructSize = sizeof(GCP_RESULTSW);
	gcp.lpOutString = new wchar_t[len];
	gcp.lpDx = new int[len];
	gcp.lpCaretPos = new int[len];
	gcp.lpGlyphs = new WCHAR[len];
	gcp.lpGlyphs[0] = 0;
	gcp.nGlyphs = (UINT)len;

	DWORD result = ::GetCharacterPlacementW(
		m_hdc, str.c_str(), str.size(), limit, &gcp, li & FLI_MASK);// GCP_REORDER | GCP_GLYPHSHAPE);// GCP_GLYPHSHAPE);

	int i0 = gcp.lpCaretPos[0];
	int i1 = gcp.lpCaretPos[1];
	int i2 = gcp.lpCaretPos[2];
	int i3 = gcp.lpCaretPos[3];
	int i4 = gcp.lpCaretPos[4];

	delete[] gcp.lpOutString;
	delete[] gcp.lpDx;
	delete[] gcp.lpCaretPos;
	delete[] gcp.lpGlyphs;
	//delete[] gcp.

	if (hfont != NULL) { ::SelectObject(m_hdc, hfont); }
}

void native_canvas_impl::draw_string(
	std::string str, point_int pt, color_rgb rgb, font* f)
{
	assert(m_hdc);

	pt += m_origin;

	BOOL result = ::TextOutA(m_hdc, pt.x, pt.y, str.c_str(), str.size());
	assert(result != 0);
}

void native_canvas_impl::draw_string(
	std::wstring str, point_int pt, color_rgb rgb, font* f)
{
	assert(m_hdc);

	pt += m_origin;

	HGDIOBJ hfont = NULL;
	if (f != nullptr)
	{
		native_font_impl *nf = static_cast<native_font_impl*>(f->get_native());
		hfont = ::SelectObject(m_hdc, nf->get_handle(this));

		pt += nf->get_offset();
	}

	::SetTextColor(m_hdc, RGB(rgb.r, rgb.g, rgb.b));

	BOOL result = ::TextOutW(m_hdc, pt.x, pt.y, str.c_str(), str.size());
	assert(result != 0);

	if (hfont != NULL) { ::SelectObject(m_hdc, hfont); }
}

// native_window_canvas_impl --------------------------------------------------

native_window_canvas_impl::native_window_canvas_impl()
{
}

native_window_canvas_impl::~native_window_canvas_impl()
{
	assert(m_hwnd);
	assert(m_hdc);
	::ReleaseDC(m_hwnd, m_hdc);
}

void native_window_canvas_impl::set_window(native_window *nw)
{
	assert(nw);
	m_hwnd = static_cast<native_window_impl*>(nw)->get_handle();
	m_hdc = ::GetDC(m_hwnd);

	::SetMapMode(m_hdc, MM_TEXT);
}


// native_memory_canvas_impl --------------------------------------------------

native_memory_canvas_impl::native_memory_canvas_impl()
{
	BITMAPINFOHEADER bmih;

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = 640;
	bmih.biHeight = 480;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	m_bitmap = ::CreateDIBSection(NULL, (BITMAPINFO*)&bmih, 0, (void**)&m_bits, NULL, 0);
	assert(m_bitmap != NULL);
}

native_memory_canvas_impl::~native_memory_canvas_impl()
{
	BOOL result = ::DeleteObject(m_bitmap);
}




















#endif // WRODRING_WS_WIN
