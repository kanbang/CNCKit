/**
 * @file    wordring/gui/container.h
 *
 * @brief   GUIコンテナ
 *
 * @details
 *          ボタンやスクロールバーなど、GUIのコントロールを載せるコンテナです。
 *          containerクラスはインターフェースとして宣言されます。
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

#ifndef WORDRING_CONTAINER_H
#define WORDRING_CONTAINER_H

#include <wordring/debug.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container_window.h>

#include <vector>
#include <string>
#include <memory>

#include <cassert>

namespace wordring
{
namespace gui
{

class container : public control
{
public:
	typedef std::unique_ptr<control> store;

public:
	virtual void add(store&& child);

protected:
	std::vector<store> m_children;
};

template <typename T>
class window_container_tmpl : public container, public T
{
public:
	typedef typename T window_type;

protected:
public:
	window_container_tmpl() { }

	virtual ~window_container_tmpl() { }

	virtual window* find_window() { return this; }

	virtual void set_parent(container* parent)
	{
		window* w = parent->find_window();
		point_int pt = query_position_from_window(); // 親ウィンドウからの相対
		T::create_window(w); // ウィンドウ作成
		T::set_window_position(pt);
		T::set_parent_window(this);
	}


	/// コントロールの大きさを設定する
	virtual void set_size(size_int size)
	{
		T::set_window_size(size);
		m_size = size;
	}
	/// コントロールの大きさを取得する
	virtual size_int get_size() const
	{
		size_int result = T::get_window_size();
		//assert(m_size == result);
		return result;
	}

	virtual void set_position(point_int point)
	{
		T::set_window_position(point);
		m_position = point;
	}

	virtual point_int get_position() const
	{
		point_int result = T::get_window_position();
		assert(m_position == result);
		return result;
	}

protected:
	virtual point_int query_position_from_window() const
	{
		return point_int(0, 0);
	}

	virtual void do_create() { if (on_create) { on_create(); } }

	virtual void do_destroy() { if (on_destroy) { on_destroy(); } }

	virtual void do_paint(canvas& cv) { if (on_paint) { on_paint(cv); } }

	virtual void do_size(size_int size) { if (on_size) { on_size(size); } }
};

class form : public window_container_tmpl<container_window>
{
public:
	virtual window* find_window() { return this;  }
	/// 子コントロールから呼び出され、自身を返します
	virtual form* find_form() { return this; }

	virtual void set_parent(container* parent)
	{
		assert(parent == nullptr); // フォームは親を持たない
		create_window(nullptr);
	}

	/// 先祖への問い合わせ連鎖によってフォームからの相対位置を求めます
	virtual point_int query_position_from_form() const
	{
		return point_int(0, 0);
	}

	virtual void set_title(std::string title)
	{
		m_native_window->set_window_text(title);
	}

	virtual void set_title(std::wstring title)
	{
		m_native_window->set_window_text(title);
	}
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_H
