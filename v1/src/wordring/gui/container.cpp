/**
* @file    wordring/gui/container.cpp
*
* @brief   GUIコンテナの実装ファイル
*
* @details
*          フォームなどのGUIコンテナです。
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

#include <wordring/gui/window_service.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>

#include <cassert>
#include <memory>


using namespace wordring::gui;

// 構築・破棄 -----------------------------------------------------------------

container::container() : m_layout(new flow_layout)
{
}

container::container(layout *l) : m_layout(l)
{
}

container::~container()
{
}

void container::set_layout(std::unique_ptr<layout>&& l)
{
	m_layout = std::move(l);
}

// 情報 -----------------------------------------------------------------------

char const* container::get_control_name() const
{
	return "container";
}

bool container::is_container() const
{
	return true;
}

bool container::including(control *c) const
{
	for (store const &c1 : m_children)
	{
		if (c1.get() == c) { return true; }
	}
	return false;
}

// 親子関係 -------------------------------------------------------------------

container::storage_type& container::get_children()
{
	return m_children;
}

container* container::find_container()
{
	return this;
}

// 描画 -----------------------------------------------------------------------

void container::do_paint_internal(canvas& cv)
{
	do_paint(cv); // まず自分を描画する

	for (store& c : m_children)
	{
		//if(c->is_window()) { continue; } // ウィンドウはシステムによって更新
		c->do_paint_internal(cv);
	}
}

// 大きさ・位置 ---------------------------------------------------------------

void container::perform_layout()
{
	WORDRING_DEBUG_CONTROL;

	m_layout->perform_layout(this);

	for (store &s : m_children)
	{
		if (s->is_container())
		{
			container *c = static_cast<container*>(s.get());
			c->perform_layout();
		}
	}
	repaint();
}

/// レイアウトを要求します
void container::request_layout()
{
	find_root_container()->request_layout(this);
}

container* container::perform_layout_internal(container *c)
{
	assert(c != nullptr);
	//root_window_container &root = find_root();
	
	return nullptr;
}

void container::do_size_internal(size_int size)
{
	container* parent = get_parent();
	//assert(parent != nullptr);
	//if (layout_) { parent->perform_layout(); }
	//assert(parent != nullptr); // トップレベル・ウィンドウはオーバーライドする
	//perform_layout();
	do_size(m_size);

}




