/**
* @file    wordring/gui/detail/native_canvas.h
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

#ifndef WORDRING_NATIVE_CANVAS_H
#define WORDRING_NATIVE_CANVAS_H

#include <wordring/debug.h>

#include <wordring/gui/font.h>
#include <wordring/gui/shape_int.h>
#include <wordring/graphics/color.h>

#include <string>

namespace wordring
{
namespace gui
{
namespace detail
{

class dummy;

class native_canvas
{
protected:
	rect_int m_viewport;
	point_int m_origin;

public:
	native_canvas()
	{
	}

	virtual ~native_canvas()
	{
	}

	/// 描画原点を取得します
	virtual point_int get_origin() const = 0;

	/// 描画原点を設定します
	virtual void set_origin(point_int pt) = 0;

	virtual rect_int get_viewport() const = 0;

	/**
	 * @brief   ビューポートを設定します
	 *
	 * @param   rc 長方形領域
	 */
	virtual void set_viewport(rect_int rc) = 0;

	/**
	 * @brief   線を描画します
	 *
	 * @param   pt1   始点
	 * @param   pt2   終点
	 * @param   width 太さ
	 * @param   rgb   色
	 */
	virtual void draw_line(
		point_int pt1, point_int pt2, int32_t width, rgb_color rgb) = 0;

	/**
	 * @brief   長方形を描画します
	 *
	 * @param   rc    長方形
	 * @param   width 太さ
	 * @param   rgb   色
	 */
	virtual void draw_rect(rect_int rc, int32_t width, rgb_color rgb) = 0;

	/**
	 * @brief   長方形を塗りつぶします
	 *
	 * @param   rc  長方形
	 * @param   rgb 色
	 */
	virtual void fill_rect(rect_int rc, rgb_color rgb) = 0;

	/**
	 * @brief   文字列を描画します
	 *
	 * @param   str 文字列
	 * @param   pt  文字列の左上位置
	 * @param   rgb 色
	 * @param   f   フォント
	 */

	virtual void draw_string(
		std::string str, point_int pt, rgb_color rgb, font* f) = 0;
	/**
	 * @brief   文字列を描画します
	 *
	 * @param   str 文字列
	 * @param   pt  文字列の左上位置
	 * @param   rgb 色
	 * @param   f   フォント
	 */
	virtual void draw_string(
		std::wstring str, point_int pt, rgb_color rgb, font* f) = 0;
};




} // detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_CANVAS_H
