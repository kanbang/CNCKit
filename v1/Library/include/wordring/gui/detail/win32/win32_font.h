/**
 * @file    wordring/gui/detail/win32/win32_font.h
 *
 * @brief   Win32用のフォント・ヘッダーファイル
 *
 * @details
 *          pimplの実装側ヘッダです。
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

public:
	TEXTMETRIC m_tm;

	// 生成・破棄 -------------------------------------------------------------
private:
	explicit native_font_impl();

public:
	virtual ~native_font_impl();

	static native_font::store create();

	HFONT create(native_canvas const *cv, LONG il);

	virtual void attach(native_canvas const *cv);

	// ハンドル ---------------------------------------------------------------

	HFONT get_handle(native_canvas const *cv);

	//

	virtual point_int get_offset() const;
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_FONT_H
