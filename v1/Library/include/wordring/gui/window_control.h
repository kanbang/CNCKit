/**
 * @file    wordring/gui/window_control.h
 *
 * @brief   ウィンドウを持つコンテナのヘッダ・ファイル
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

#ifndef WORDRING_window_control_H
#define WORDRING_window_control_H

#include <wordring/wordring.h>

#include <wordring/gui/window.h>
#include <wordring/gui/container.h>
#include <wordring/gui/layout.h>
#include <wordring/gui/shape_int.h>

#include <wordring/gui/detail/native_window.h>
namespace wordring
{
namespace gui
{

/**
 * @brief   ウィンドウを持つコンテナ
 *
 * @details
 *          ウィンドウの特性とコンテナの特性を合成します。
 */
class window_control : public container, public window
{
	// 構築・破棄 -------------------------------------------------------------
protected:
	window_control(
		rect_int rc, layout::store l, detail::native_window::store nw);

public:
	virtual ~window_control();

	// 親子関係 ---------------------------------------------------------------

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	/// コントロールがウィンドウの場合、trueを返します
	virtual bool is_window() const;

	virtual window* to_window();

	// 表示 -------------------------------------------------------------------

	// 大きさ・位置 -----------------------------------------------------------

	// マウス・メッセージ -----------------------------------------------------

	/// ウィンドウ上でマウス・ボタンが押された時、呼び出されます
	virtual void do_mouse_down_window(mouse &m);

	/// ウィンドウ上でマウス・ボタンが離された時、呼び出されます
	virtual void do_mouse_up_window(mouse &m);

	virtual void do_mouse_enter_window(mouse &m);

	virtual void do_mouse_leave_window();

	/// ウィンドウ上をマウス・ポインタが移動するとき呼び出されます
	virtual void do_mouse_move_window(mouse &m);

	/// マウス・ホイールが回されたとき呼び出されます
	virtual void do_mouse_wheel_window(mouse &m);

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_paint_window(canvas &cv);

	virtual void do_size_window(size_int size);


};

} // namespace gui
} // namespace wordring

#endif // WORDRING_window_control_H
