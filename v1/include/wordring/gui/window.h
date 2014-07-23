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

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/window_service.h>

#include <wordring/geometry/shape.h>

#include <wordring/gui/canvas.h>

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
	std::unique_ptr<detail::native_window> m_native_window; // pimpl

public:
	explicit window(detail::native_window* nw);
	window();
	virtual ~window();

	/// pimplの実装側インターフェースを返します
	detail::native_window* get_native_window();

	/** @brief ウィンドウを作成します
	 *  @details 子ウィンドウは作成時に親ウィンドウが必要です。
	 */
	void create(window * parent);
	/// ウィンドウを最小化します
	void close();
	/// ウィンドウを破棄します
	void destroy();

	/// ウィンドウを表示します
	void show();
	/// ウィンドウを非表示にします
	void hide();

	/// 親ウィンドウを設定します
	void set_parent(window* parent);
	/// 親ウィンドウを取得します
	window* get_parent();

	/// ウィンドウの大きさを設定します
	void set_size(size_int size);
	/// ウィンドウの大きさを取得します
	size_int get_size() const;
	/** @brief ウィンドウの位置を設定します
	 *  @details 親ウィンドウあるいはデスクトップからの相対位置です。
	 */
	void set_position(point_int point);
	/// ウィンドウの位置を返します
	point_int get_position() const;


	/// ウィンドウ作成時に呼び出されます
	virtual void do_create();
	/// ウィンドウが破棄されるとき呼び出されます
	virtual void do_destroy();
	/// ウィンドウの描画更新が必要なとき呼び出されます
	virtual void do_paint(canvas& cv);
};
/*
class button_window : public window
{
public:
	button_window();

};
*/
} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_H
