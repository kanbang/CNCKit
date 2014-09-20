/**
 * @file    wordring/gui/detail/win32/win32_font.h
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

#ifndef WORDRING_WIN32_FONT_H
#define WORDRING_WIN32_FONT_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_font.h>
#include <wordring/gui/font.h>

#include <Windows.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class dummy;

class native_font_impl : public native_font
{
private:
	HFONT m_hfont;

private:
	explicit native_font_impl(font_conf fc);

public:
	virtual ~native_font_impl();

	static native_font::store create(font_conf fc);

	HFONT get_handle(HDC hdc);

	void attach(HDC hdc, font_conf const &fc);
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_FONT_H
