/**
 * @file    wordring/gui/root_window.h
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

#ifndef WORDRING_ROOT_WINDOW_CONTAINER_H
#define WORDRING_ROOT_WINDOW_CONTAINER_H

#include <wordring/gui/window_container_tmpl.h>

#include <wordring/gui/container.h>

#include <wordring/gui/detail/native_window.h>

namespace wordring
{
namespace gui
{

class root_window;

class root_container : public window_container_tmpl<container, window>
{
public:
	typedef window_container_tmpl<container, window> base_type;
	typedef std::unique_ptr<root_container> store;

protected:
	root_window *m_root_window;
	std::deque<container*> m_layouts;

public:
	root_container();

	void create(root_window *rw);

	virtual char const* get_control_name() const { return "root_container"; }

	/**
	 * @brief   ルート・コンテナを検索します
	 */
	virtual root_container* find_root_container();

	virtual void set_root_window(root_window *rw);
	virtual root_window& find_root_window();

	virtual window_service& find_service();

	virtual void request_layout(container *c);
};

class root_window : public window
{
protected:
	window_service &m_service;
	root_container::store m_client;

public:
	root_window(window_service &ws, rect_int rc);

	root_window(
		window_service& ws, ///< ウィンドウ・サービス
		rect_int rc, ///< ウィンドウの長方形
		detail::native_window *nw,
		control *client);

	window_service& get_service() { return m_service; }

	template <typename T>
	T* assign(T *pT)
	{
		m_client->assign(pT);
		return pT;
	}

	void show();
	void hide();

	void set_rect(rect_int rc);

	virtual void do_destroy_w()
	{
		m_service.quit();
	}

	virtual void do_size_w(size_int size);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_ROOT_WINDOW_CONTAINER_H
