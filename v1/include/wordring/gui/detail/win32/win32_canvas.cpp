/**
 * @file    wordring/gui/native/win32/win32_canvas.cpp
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

#include <cassert>

using namespace wordring::gui::detail;
using namespace wordring::gui;

native_canvas_impl::native_canvas_impl()
{

}

native_canvas_impl::native_canvas_impl(HDC hdc) : m_hdc(hdc)
{

}

native_canvas_impl::~native_canvas_impl()
{

}

HDC native_canvas_impl::get_dc()
{
	return m_hdc;
}

void native_canvas_impl::draw_string(std::string str, point_int pt)
{
	BOOL result = ::TextOutA(m_hdc, pt.x, pt.y, str.c_str(), str.size());
	assert(result != 0);
}

void native_canvas_impl::draw_string(std::wstring str, point_int pt)
{
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
