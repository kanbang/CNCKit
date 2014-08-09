/**
 * @file    wordring/gui/window_service.h
 *
 * @brief   ウィンドウ・サービスのヘッダー・ファイル
 *
 * @details
 *          ウィンドウ・サービスはメッセージ・ポンプに相当します。\n
 *          Boost.Asioのコンセプトに類似します。\n
 *          
 *          このヘッダー・ファイルは、ライブラリ利用者に公開されます。
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_WINDOW_SERVICE_H
#define WORDRING_WINDOW_SERVICE_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/detail/native_window_service.h>

//#include <wordring/gui/root_window.h>

#include <wordring/gui/message.h>

#include <memory>
#include <list>
#include <deque>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <functional>

namespace wordring
{
namespace gui
{

class root_window;
class container;
class window_service;

class layout_service
{
public:
	typedef std::deque<container*> storage_type;
	typedef storage_type::iterator iterator;

private:
	storage_type m_queue;

public:
	void push(container *c);

	container* pop();

	void remove(container *c);
};

class timer_service
{
public:
	typedef std::unique_ptr<timer_service> store;
	typedef std::list<message::store> storage_type;
	typedef storage_type::iterator iterator;

	typedef std::mutex mutex_type;

private:
	window_service &m_window_service;

	storage_type m_queue;
	mutex_type m_mutex;
	std::thread m_thread;

	std::atomic_bool m_run_flag;

protected:
	timer_service(window_service &ws);

public:
	virtual ~timer_service();

	static store create(window_service &ws);

	// キュー -----------------------------------------------------------------

	/// メッセージmをキューの末尾に追加します
	void push(message::store m);

	/// キューの先頭にあるメッセージを取り出します
	message::store pop();

	/// cに関連付けられている全てのメッセージをキューから取り除きます
	void remove(control *c);

	iterator begin();

	iterator end();

	// メッセージ・ループ -----------------------------------------------------

	/// メッセージ・ループを実行します
	void run();

	/// メッセージ・ループを停止します
	void quit();
};

class window_service
{
public:
	typedef std::mutex mutex_type;
	typedef std::unique_ptr<root_window> root_store;

private:
	std::unique_ptr<detail::native_window_service> m_native; ///< pimpl

	std::list<root_store> m_root_windows; ///< スレッドのウィンドウ群
	
	mutex_type m_mutex;
	std::list<message::store> m_queue; ///< メッセージ・キュー

	timer_service::store m_timer;

public:
	int32_t m_debug_control_cnt;

public:
	window_service();
	virtual ~window_service();

	detail::native_window_service* get_native();

	/// ルート・ウィンドウを末尾に追加します
	root_window* push_back(root_store s);

	// キュー -----------------------------------------------------------------

	/// メッセージmをキューの末尾に追加します
	void push(message::store m);

	/// キューの先頭にあるメッセージを取り出します
	message::store pop();

	/// cに関連付けられている全てのメッセージをキューから取り除きます
	void remove(control *c);

	// メッセージ・ループ -----------------------------------------------------

	/// メッセージ・ループを実行します
	void run();

	/// メッセージ・ループを停止します
	void quit();

	// タイマー ---------------------------------------------------------------

	/// ms秒後にcで発火するタイマーを設定します
	void set_timer(control *c, int32_t ms);

	/// 
	void create_timer_thread();

	/// 
	void tick_timer();

	/**
	 * @brief   空のメッセージを送信します
	 *
	 * @details
	 *          このメンバは、ライブラリが実装するメッセージキューの処理を開始
	 *          するタイミングを提供するために用意されています。
	 *
	 *          ウィンドウ・サービスは二つのメッセージキューを管理しています。
	 *          ウィンドウ・システムのメッセージキューとライブラリのメッセージ
	 *          キューの二つです。
	 *
	 *          ライブラリのメッセージキューへメッセージをポストするタイミング
	 *          でこのメンバを呼び出すと、ウィンドウ・システムのメッセージ
	 *          キューに空のメッセージが積まれます。
	 *          ウィンドウ・システムのメッセージキューからメッセージを取り出す
	 *          ときに空のメッセージがあるとwindow_service::do_tick_message()が
	 *          呼び出され、この中からライブラリのメッセージキューの処理を開始
	 *          します。
	 *
	 *          以上の仕組みによって二つのメッセージキューが同期されます。
	 */
	virtual void post_tick_message();

	virtual void do_tick_message();
};









} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_SERVICE_H
