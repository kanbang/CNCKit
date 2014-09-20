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

#include <wordring/wordring.h>

#include <wordring/gui/font.h>

#include <wordring/gui/detail/native_font.h>

#include <Windows.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class native_canvas; // 先行宣言

class native_font_impl : public native_font
{
private:
	HFONT m_hfont;

private:
	explicit native_font_impl(font_conf fc);

public:
	virtual ~native_font_impl();

	static native_font::store create(font_conf fc);

	HFONT get_handle(native_canvas const *cv);

	virtual void attach(native_canvas const *cv);
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_FONT_H
