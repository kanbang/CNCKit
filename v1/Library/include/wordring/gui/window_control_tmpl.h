/**
 * @file    wordring/gui/window_control_tmpl.h
 *
 * @brief   ウィンドウを持つコントロール用のテンプレート・クラス
 *
 * @details
 *          ウィンドウを持つコントロールはここから派生させます。
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

#ifndef WORDRING_WINDOW_CONTROL_TMPL_H
#define WORDRING_WINDOW_CONTROL_TMPL_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>

#include <wordring/gui/window.h>

#include <list>
#include <iostream>

namespace wordring
{
namespace gui
{

class dummy;

/**
 * @brief   コントロールを作成するためのテンプレート・クラス
 *
 * @details ウィンドウの特性とコントロールの特性を合成できます。
 *          ウィンドウを持つすべてのコントロールは、ここから派生するべきです。
 *          コンテナの場合、window_container_tmplから派生してください。
 *          window_container_tmplは、window_control_tmplを継承しています。
 *
 * @param   T1 基本コントロール
 * @param   T2 基本ウィンドウ
 */
template <typename T1, typename T2>
class window_control_tmpl : public T1, public T2
{
public:
	typedef typename T1 control_type;
	typedef typename T2 window_type;

	// 構築・破棄 -------------------------------------------------------------
protected:
	/**
	 * @brief   window_container_tmpl専用のコンストラクタ
	 *
	 * @param   l レイアウト
	 * @param   nw ネイティブ・ウィンドウ
	 */
	window_control_tmpl(layout::store l, detail::native_window::store nw)
		: control_type(std::move(l)), window_type(std::move(nw))
	{
	}

	explicit window_control_tmpl(detail::native_window::store nw)
		: window_type(std::move(nw))
	{
	}

public:
	virtual ~window_control_tmpl()
	{
	}

	virtual void create(container *parent, rect_int rc)
	{
		m_parent = parent;
		// トップレベル・コンテナの場合、親がnullもあり得る
		window *pw = nullptr;
		if (parent)
		{
			pw = &(parent->find_window());
			// 位置の正規化
			point_int pt = parent->query_position_from_window();
			rc.pt += pt;
		}
		window_type *pT2 = static_cast<window_type*>(this);
		pT2->get_native()->create_window(pw, rc); // pwを親としてウィンドウを作成する
	}

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const
	{
		return "window_control_tmpl<T1, T2>";
	}

	virtual bool is_window() const { return true;  }

	virtual window& find_window() { return *this; }

	// 表示 -------------------------------------------------------------------

	virtual void show()
	{
		window_type *pT2 = static_cast<window_type*>(this);
		pT2->get_native()->show_window();
	}

	virtual void hide()
	{
		window_type *pT2 = static_cast<window_type*>(this);
		pT2->get_native()->hide_window();
	}

	// 親子関係 ---------------------------------------------------------------

	virtual void set_parent(container *parent)
	{
		assert(false);
		/*
		window* w = find_window();
		// 親 *ウィンドウ* からの相対位置
		point_int pt = query_position_from_window();
		window_type::create_window(w); // ウィンドウ作成
		window_type::set_window_position(pt);
		window_type::set_parent_window(this);
		*/
	}

	// 大きさ・位置 -----------------------------------------------------------

/*
	/// コントロールの大きさを設定する
	virtual void set_size(size_int size)
	{
		m_size = size;
		window_type::set_window_size(size);
	}
*/
	/// コントロールの大きさを取得する
/*	virtual size_int get_size() const
	{
		window_type const *pT2 = static_cast<window_type const*>(this);
		size_int result = pT2->get_native()->get_window_size();
		//assert(m_size == result); // TODO: クライアントエリアとトップレベルの違い
		result = m_size;
		return result;
	}
	*/
/*
	/// コントロールの位置を設定する
	virtual void set_position(point_int pt)
	{
		m_pt = pt;

		container* c = get_parent(); // 親がデスクトップの場合、nullptr
		if (c != nullptr) { pt += c->query_position_from_window(); }

		window_type::set_window_position(pt);
	}
*/
	/*
	/// コントロールの位置を取得する
	virtual point_int get_position() const
	{
	point_int result = window_type::get_window_position();
	// assert(m_position == result); // TODO: クライアントエリアとトップレベルの違い
	return result;
	}
	*/

	/// コントロールの長方形を設定する
	virtual void set_rect(rect_int rc)
	{
		control_type *pT1 = static_cast<control_type*>(this);

		if (rc == pT1->get_rect()) { return; } // ループ・ガード

		m_pt = rc.pt;
		m_size = rc.size;

		container *parent = pT1->get_parent(); // 親がデスクトップの場合、nullptr

		if (parent) { rc.pt += parent->query_position_from_window(); }

		window_type *pT2 = static_cast<window_type*>(this);
		pT2->get_native()->set_window_rect(rc);

		container* c = parent ? parent : pT1->find_container();
		c->request_layout();
	}
/*
	/// コントロールの長方形を取得する
	virtual rect_int get_rect() const
	{
		window_type const *pT2 = static_cast<window_type const*>(this);
		rect_int result = pT2->get_native()->get_window_rect();
		return result;
	}
*/
	/// ウィンドウからの相対位置を取得する
	virtual point_int query_position_from_window() const
	{
		return point_int(0, 0);
	}

	/// ウィンドウからの相対長方形を取得する
	virtual rect_int query_rect_from_window() const
	{
		control_type const *pT1 = static_cast<control_type const*>(this);
		return pT1->get_rect();
	}

	// マウス・メッセージ -----------------------------------------------------

	virtual void do_mouse_move_window(point_int pt)
	{
		control_type *pT1 = static_cast<control_type*>(this);
		pT1->do_mouse_move_internal(pt);
	}

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_paint_window(canvas &cv)
	{
		control_type *pT1 = static_cast<control_type*>(this);
		pT1->do_paint_internal(cv);
	}

	virtual void do_size_window(size_int size)
	{
		control_type *pT1 = static_cast<control_type*>(this);
		pT1->set_rect(rect_int(get_position(), size));
	}

	// ------------------------------------------------------------------------

};

} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_CONTROL_TMPL_H
