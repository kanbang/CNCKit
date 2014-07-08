/**
 * @file    wordring/gui/window.h
 *
 * @brief   ウィンドウの公開ヘッダー・ファイル
 *
 * @details
 *          
 *          
 *  @par    例
 *        - win32のwindowを例として説明します。
 *        - window(window.h)はメンバとしてnative_window(native_window.h)への
 *          ポインタを持ちます。
 *        - native_windowへのポインタは、window.cpp内にあるコンストラクタで
 *          native_window_impl(win32_window.h)がnewされ初期化されます。
 *        - native_window_implはnative_windowを継承しているので、これが可能です。
 *          一方で、native_window_implはwindowのヘッダーに読み込まれないため、
 *          利用者に対して隠蔽されます。
 *        - 結果として、windowクラスを機能させるには、window、native_window、
 *          native_window_implの三種類のクラスが必要です。
 *        - windowはメソッドが呼び出されると大部分をポインタを通して
 *          native_windowへ委譲します。
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

#include <memory>
#include <functional>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/window_service.h>
#include <wordring/geometry/shape.h>

//#include <wordring/gui/container.h>

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
	std::unique_ptr<wordring::gui::detail::native_window> m_native; // pimpl

public:
	std::function<void()> on_create; /// イベント・ハンドラ
	std::function<void()> on_click; /// イベント・ハンドラ

public:
	explicit window(detail::native_window* p);
	window();
	virtual ~window();

	/** @brief ウィンドウを作成します
	 *  @detail 小ウィンドウは作成時に親ウィンドウが必要です。
	 */
	virtual void create(window * parent);
	/// ウィンドウを最小化します
	virtual void close();
	/// ウィンドウを破棄します
	virtual void destroy();

	/// 親ウィンドウを取得します
	virtual window* get_parent();
	/// 親ウィンドウを設定します
	virtual void set_parent(window* parent);

	/// ウィンドウの大きさを設定します
	virtual void set_size(size_int size);
	/// ウィンドウの大きさを取得します
	virtual size_int get_size() const;
	/** @brief ウィンドウの位置を設定します
	 *  @detail 親ウィンドウあるいはデスクトップからの相対位置です。
	 */
	virtual void set_position(point_int point);
	virtual point_int get_position() const;

	detail::native_window* get_native();
protected:
};

class control_window : public window
{
public:
	control_window();
};

class container_window : public window
{
public:
	container_window() { }

	/*
	virtual void set_size(size_int size);
	virtual size_int get_size() const;

	virtual void set_position(point_int point);
	virtual size_int get_position() const;
	*/
};

class button_window : public window
{
public:
	button_window();

};

} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_H
