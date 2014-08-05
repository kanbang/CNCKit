/**
 * @file    wordring/gui/root_window.cpp
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

#include <wordring/debug.h>

#include <wordring/gui/root_window.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_child_window.h>
#endif // _WIN32

using namespace wordring::gui;

// root_container -------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

root_container::root_container() : base_type(full_layout::create(),
		std::move(detail::native_control_window_impl::create()))
{

}

root_container::~root_container()
{

}

root_container::store root_container::create()
{
	return store(new root_container);
}

void root_container::create(root_window *rw)
{
	m_root_window = rw;
	get_native()->create_window(rw, rect_int());
}

// 情報 -----------------------------------------------------------------------

char const* root_container::get_control_name() const
{
	return "root_container";
}

root_container* root_container::find_root_container()
{
	return this;
}

void root_container::set_root_window(root_window *rw)
{
	m_root_window = rw;
}

root_window& root_container::find_root_window()
{
	return *m_root_window;
}


window_service& root_container::find_service()
{
	return m_root_window->get_service();
}

// レイアウト調停 -------------------------------------------------------------

void root_container::request_layout(container *c)
{
	// キューが空の場合、開始位置にいる
	bool start = (m_layout_requests.empty()) ? true : false;

	// TODO: 子孫が予約されている場合、祖先と挿げ替える機能を実装したほうが良い

	// 祖先のコンテナがレイアウト予約されている場合、子孫は予約する必要がない
	for (container *c0 : m_layout_requests)
	{
		if (c0->is_ancestor_of(c))
		{
			return;
		}
	}
	// レイアウトを予約
	m_layout_requests.push_back(c);

	if (start) // 開始位置にいる場合、レイアウトを開始する
	{
		while (!m_layout_requests.empty())
		{
			container *c1 = m_layout_requests.front();
			m_layout_requests.pop_front();
			c1->perform_layout();
		}
	}
}

// root_window ----------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

root_window::root_window(window_service &ws, rect_int rc)
	: m_service(ws), m_client(root_container::create())
{
	get_native()->create_window(nullptr, rc);
	m_client->create(this);
	m_client->set_size(size_int(100, 100));
}

root_window::root_window(
	window_service& ws, rect_int rc, detail::native_window::store nw)
	: m_service(ws), window(std::move(nw)), m_client(root_container::create())
{
	get_native()->create_window(nullptr, rc);
	m_client->create(this);
	m_client->set_size(size_int(100, 100));
}

control* root_window::assign(control::store s)
{
	control *c = static_cast<control*>(s.get());
	m_client->assign(std::move(s));

	return c;
}

// 表示 -----------------------------------------------------------------------

void root_window::show()
{
	get_native()->show_window();
}

void root_window::hide()
{
	get_native()->hide_window();
}

// 大きさ・位置 ---------------------------------------------------------------

void root_window::set_rect(rect_int rc)
{
	get_native()->set_window_rect(rc);
}

// 一般メッセージ -------------------------------------------------------------

void root_window::do_size_window(size_int size)
{
	if (m_client) { m_client->set_size(size); }
}





