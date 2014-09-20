/**
 * @file    wordring/gui/window.h
 *
 * @brief   ウィンドウの公開ヘッダー・ファイル
 *
 * @details
 *          
 *          
 *  @par    例
 *    - win32のwindowを例として説明します。
 *    - window(window.h)はメンバとしてnative_window(native_window.h)への
 *      ポインタを持ちます。
 *    - native_windowへのポインタは、window.cpp内にあるコンストラクタで
 *      native_window_impl(win32_window.h)がnewされ初期化されます。
 *    - native_window_implはnative_windowを継承しているので、これが可能です。
 *      一方で、native_window_implはwindowのヘッダーに読み込まれないため、
 *      利用者に対して隠蔽されます。
 *    - 結果として、windowクラスを機能させるには、window、native_window、
 *      native_window_implの三種類のクラスが必要です。
 *    - windowはメソッドが呼び出されると大部分をポインタを通して
 *      native_windowへ委譲します。
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


#ifndef WORDRING_WINDOW_H
#define WORDRING_WINDOW_H

#include <wordring/wordring.h>

#include <wordring/gui/window_service.h>
#include <wordring/gui/shape_int.h>
#include <wordring/gui/mouse.h>
#include <wordring/gui/canvas.h>

#include <wordring/gui/detail/native_window.h>

#include <memory>

namespace wordring
{
namespace gui
{

/**
 * @brief ウィンドウ
 *
 * 
 */
class window
{
protected:
	std::unique_ptr<detail::native_window> m_native; // pimpl

	// 構築・破棄 -------------------------------------------------------------
public:
	explicit window(detail::native_window::store nw);

	window();

	virtual ~window();

	/// pimplの実装側を取得します
	detail::native_window* get_native();

	/// pimplの実装側を取得します
	detail::native_window const* get_native() const;

	// マウス・メッセージ -----------------------------------------------------

	/**
	 * @brief   ウィンドウ上でマウス・ボタンが押された時、呼び出されます
	 *
	 * @param   m ボタンが押された時の状態
	 */
	virtual void do_mouse_down_window(mouse &m);

	/**
	 * @brief   ウィンドウ上で左マウス・ボタンが離された時、呼び出されます
	 *
	 * @param   m ボタンが離された時の状態
	 */
	virtual void do_mouse_up_window(mouse &m);

	virtual void do_mouse_enter_window(mouse &m);

	virtual void do_mouse_leave_window();

	// ウィンドウ上をマウス・ポインタが移動するとき呼び出されます
	virtual void do_mouse_move_window(mouse &m);

	/// マウス・ホイールが回されたとき呼び出されます
	virtual void do_mouse_wheel_window(mouse &m);
	
	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	/// ウィンドウ作成時に呼び出されます
	virtual void do_create_window();

	/// ウィンドウが破棄されるとき呼び出されます
	virtual void do_destroy_window();

	/// ウィンドウの描画更新が必要なとき呼び出されます
	virtual void do_paint_window(canvas& cv);

	/// ウィンドウの大きさが変更されるとき呼び出されます
	virtual void do_size_window(size_int size);
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_H
