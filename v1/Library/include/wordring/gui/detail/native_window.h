/**
 * @file    wordring/gui/detail/native_window.h
 *
 * @brief   ウィンドウのpimplインターフェース
 *
 * @details
 *          このファイルには、pimplの公開されるインターフェースを宣言します。
 *          環境依存ウィンドウの実装に継承されます。
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

#ifndef WORDRING_NATIVE_WINDOW_H
#define WORDRING_NATIVE_WINDOW_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>

#include <string>
#include <memory>

namespace wordring
{
namespace gui
{

class window; // 前方宣言

namespace detail
{

/**
 * @brief   ウィンドウpimplの実装側インターフェース
 *
 * @details
 *          ウィンドウの環境依存実装はこのクラスから派生します
 */
class native_window
{
public:
	typedef std::unique_ptr<native_window> store;

protected:
	wordring::gui::window* m_public; ///< インターフェースへのポインタ

	// 構築・破棄 -------------------------------------------------------------
public:
	native_window()
	{
	}

	virtual ~native_window()
	{
	}

	/**
	 * @brief   pimplのインターフェース側を設定します
	 *
	 * @details
	 *          pimplのインターフェース側コンストラクタの初期化子内ではthisが
	 *          確定していないため、コンストラクタ本体から設定します。
	 */
	void set_public(wordring::gui::window* w)
	{
		m_public = w;
	}

	/**
	 * @brief   pimplのインターフェース側を取得します
	 *
	 * @details
	 *          主にコールバック用に使われます。
	 */
	wordring::gui::window* get_public()
	{
		return m_public;
	}

	/**
	 * @brief   環境依存のウィンドウを作成します
	 *
	 * @param   parent 親ウィンドウ
	 * @param   rc     ウィンドウの領域を示す長方形
	 */
	virtual void create_window(wordring::gui::window* parent, rect_int rc) = 0;

	/// ウィンドウを破棄します
	virtual void destroy_window() = 0;

	// 表示 -------------------------------------------------------------------

	/// ウィンドウをアイコン化します
	virtual void close_window() = 0;

	/// ウィンドウを表示します
	virtual void show_window() = 0;

	/// ウィンドウを非表示にします
	virtual void hide_window() = 0;

	// 描画 -------------------------------------------------------------------

	/// ウィンドウ全体を再描画します
	virtual void repaint_window() = 0;

	/// ウィンドウの指定範囲を再描画します
	virtual void repaint_window(rect_int rc) = 0;

	// 大きさ・位置 -----------------------------------------------------------

	/// ウィンドウの大きさを設定します
	virtual void set_window_size(size_int size) = 0;

	/// ウィンドウの大きさを取得します
	virtual size_int get_window_size() const = 0;

	/// ウィンドウの位置を設定します
	virtual void set_window_position(point_int point) = 0;

	/// ウィンドウの位置を取得します
	virtual point_int get_window_position() const = 0;

	/// ウィンドウの位置と大きさを設定します
	virtual void set_window_rect(rect_int rc) = 0;

	/// ウィンドウの位置と大きさを取得します
	virtual rect_int get_window_rect() const = 0;

	// その他 -----------------------------------------------------------------

	/// ウィンドウの文字列を設定します
	virtual void set_window_text(std::string text) = 0;

	/// ウィンドウの文字列を設定します
	virtual void set_window_text(std::wstring text) = 0;
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_WINDOW_H
