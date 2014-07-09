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
	std::unique_ptr<detail::native_window> m_native_window; // pimpl

public:

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

	/// 親ウィンドウを取得します
	window* get_parent();
	/// 親ウィンドウを設定します
	void set_parent(window* parent);

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

	/// メッセージ・ハンドラ
	bool onCreate();

protected:
};

class window_impl : public window
{
public:
	window_impl();

	bool onCreate();
};

// control_window -------------------------------------------------------------

/** 
 * @brief native_control_window_impl初期化用のダミー・クラス
 * @details
 *    コンストラクタでnative_control_window_implを初期化します。
 */
class control_window_ : public window
{
public:
	control_window_();
};

/**
 * @brief コントロール・ウィンドウ
 * @details
 *    コントロールとして使う基底ウィンドウです。
 *    ここから派生します。
 * @param ControlT メッセージ配送先となるcontrol派生クラスを指定します。
 * @param WindowT  継承するwindowクラスを指定します。
 */
template <typename ControlT, typename WindowT = control_window_>
class control_window : public WindowT
{
protected:
	/// メッセージ配送先のcontrol派生オブジェクト
	ControlT* m_control;

public:
	control_window() : m_control(nullptr) { }

	/// control派生クラスのコンストラクタ内からcontrolのthisを設定します
	void set_control(ControlT* c) { m_control = c; }
	/// 
	virtual bool on_click() { return m_control->on_click(); }
};

// container_window -----------------------------------------------------------

/**
 * @brief native_container_window_impl初期化用のダミー・クラス
 * @details
 *    コンストラクタでnative_container_window_implを初期化します。
 */
class container_window_ : public window
{
public:
	container_window_();
};

/**
 * @brief コンテナ・ウィンドウ
 * @details
 *    コンテナとして使う基底ウィンドウです。
 *    ここから派生します。
 * @param T メッセージ配送先となるcontainer派生クラスを指定します。
 */
template <typename T>
class container_window : public control_window<T, container_window_>
{
public:
	container_window() { }
};

class button_window : public window
{
public:
	button_window();

};

} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_H
