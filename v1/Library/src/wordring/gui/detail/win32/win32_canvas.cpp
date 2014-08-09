/**
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

#ifdef _WIN32

#include <wordring/debug.h>

#include <wordring/gui/detail/win32/win32_canvas.h>

#include <wordring/geometry/shape.h>
#include <wordring/graphics/color.h>
#include <wordring/gui/font.h>

#include <Windows.h>

#include <cassert>

using namespace wordring::gui::detail;
using namespace wordring::gui;

native_canvas_impl::native_canvas_impl()
{

}

native_canvas_impl::native_canvas_impl(HDC hdc) : m_hdc(hdc)
{
	::SetBkMode(m_hdc, TRANSPARENT);
}

native_canvas_impl::~native_canvas_impl()
{
	::SelectClipRgn(m_hdc, NULL);
}

HDC native_canvas_impl::get_dc()
{
	return m_hdc;
}

void native_canvas_impl::set_viewport(rect_int rc)
{
	m_pt = rc.pt;
	m_size = rc.size;

	HRGN hrgn = ::CreateRectRgn(
		rc.pt.x, rc.pt.y, rc.pt.x + rc.size.cx, rc.pt.y + rc.size.cy);
	assert(hrgn != NULL);
	int r1 = ::SelectClipRgn(m_hdc, hrgn);
	BOOL r2 = ::DeleteObject(hrgn);
	assert(r2 != NULL);

}

void native_canvas_impl::draw_line(
	point_int pt1, point_int pt2, int32_t width, rgb_color rgb)
{
	pt1 += m_pt; pt2 += m_pt; // ビューポート・オフセット

	HPEN hpen = ::CreatePen(
		PS_SOLID | PS_INSIDEFRAME, width, RGB(rgb.r, rgb.g, rgb.b));
	assert(hpen != NULL);

	HGDIOBJ obj = ::SelectObject(m_hdc, hpen);
	assert(obj != NULL);

	BOOL result = ::MoveToEx(m_hdc, pt1.x, pt1.y, NULL);
	assert(result != 0);
	result = ::LineTo(m_hdc, pt2.x, pt2.y);
	if (result == 0)
	{
		int i = 0;
	}
	assert(result != 0);

	result = ::DeleteObject(hpen);
	assert(result != 0);
}

void native_canvas_impl::draw_rect(rect_int rc, int32_t width, rgb_color rgb)
{

}

void native_canvas_impl::fill_rect(rect_int rc, rgb_color rgb)
{
	rc.pt += m_pt;

	HBRUSH hbrush = ::CreateSolidBrush(RGB(rgb.r, rgb.g, rgb.b));
	assert(hbrush != NULL);
	HPEN hpen = ::CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	assert(hpen != NULL);
	
	HGDIOBJ obj = ::SelectObject(m_hdc, hbrush);
	assert(obj != NULL);
	obj = ::SelectObject(m_hdc, hpen);
	assert(obj != NULL);

	BOOL result = ::Rectangle(
		m_hdc, rc.pt.x, rc.pt.y, rc.pt.x + rc.size.cx, rc.pt.y + rc.size.cy);
	assert(result != 0);

	result = ::DeleteObject(hbrush);
	assert(result != 0);
	result = ::DeleteObject(hpen);
	assert(result != 0);
}

void native_canvas_impl::draw_string(
	std::string str, point_int pt, rgb_color rgb, font* f)
{
	pt += m_pt;

	BOOL result = ::TextOutA(m_hdc, pt.x, pt.y, str.c_str(), str.size());
	assert(result != 0);
}

void native_canvas_impl::draw_string(
	std::wstring str, point_int pt, rgb_color rgb, font* f)
{
	pt += m_pt;

	BOOL result = ::TextOutW(m_hdc, pt.x, pt.y, str.c_str(), str.size());
	assert(result != 0);
}

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




















#endif // _WIN32
