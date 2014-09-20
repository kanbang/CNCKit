/**
* @file    wordring/gui/detail/win32/win32_canvas.h
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

#ifndef WORDRING_WIN32_CANVAS_H
#define WORDRING_WIN32_CANVAS_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_canvas.h>

#include <Windows.h>

#include <cstdint>

namespace wordring
{
namespace gui
{
namespace detail
{

class native_canvas_impl : public native_canvas
{
protected:
	HDC m_hdc;

public:
	native_canvas_impl();

	native_canvas_impl(HDC hdc);

	virtual ~native_canvas_impl();

	HDC get_handle();

	/// 描画原点を取得します
	virtual point_int get_origin() const;

	/// キャンバスの描画原点を設定します
	virtual void set_origin(point_int pt);

	virtual rect_int get_viewport() const;

	/**
	 * @brief   ビューポートを設定します
	 *
	 * @param   rc 長方形領域
	 */
	virtual void set_viewport(rect_int rc);

	/**
	 * @brief   線を描画します
	 *
	 * @param   pt1   始点
	 *
	 * @param   pt2   終点
	 *
	 * @param   width 太さ
	 *
	 * @param   rgb   色
	 */
	virtual void draw_line(
		point_int pt1, point_int pt2, int32_t width, rgb_color rgb);

	/**
	 * @brief   長方形を描画します
	 *
	 * @param   rc    長方形
	 *
	 * @param   width 太さ
	 *
	 * @param   rgb   色
	 */
	virtual void draw_rect(rect_int rc, int32_t width, rgb_color rgb);

	/**
	 * @brief   長方形を塗りつぶします
	 *
	 * @param   rc  長方形
	 *
	 * @param   rgb 色
	 */
	virtual void fill_rect(rect_int rc, rgb_color rgb);

	/**
	 * @brief   文字列を描画します
	 *
	 * @param   str 文字列
	 *
	 * @param   pt  文字列の左上位置
	 *
	 * @param   rgb 色
	 *
	 * @param   f   フォント
	 */
	virtual void draw_string(
		std::string str, point_int pt, rgb_color rgb, font* f);
	/**
	 * @brief   文字列を描画します
	 *
	 * @param   str 文字列
	 *
	 * @param   pt  文字列の左上位置
	 *
	 * @param   rgb 色
	 *
	 * @param   f   フォント
	 */
	virtual void draw_string(
		std::wstring str, point_int pt, rgb_color rgb, font* f);
};

class native_memory_canvas_impl : public native_canvas_impl
{
protected:
	HBITMAP m_bitmap;
	BYTE* m_bits;

public:
	native_memory_canvas_impl();
	virtual ~native_memory_canvas_impl();

};


} // detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_CANVAS_H
