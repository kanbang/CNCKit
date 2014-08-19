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
#include <wordring/gui/detail/native_message_service.h>

//#include <wordring/gui/root_window.h>

#include <wordring/gui/message.h>

#include <wordring/gui/mouse.h>

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

/**
 * @brief   レイアウトの調停と圧縮を行うサービスです
 *
 * @details
 *          レイアウト要求はネストするため、OSに処理を返さずにこのクラスで処理
 *          します。
 *          この処理は特にLinuxでの処理の重さを改善する狙いがあります。
 *          レイアウト要求の圧縮や調停を行う能力があります。
 */
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

	void erase(container *c);
};

class mouse_service
{
public:
	typedef std::deque<control*> storage_type;
	typedef storage_type::iterator iterator;
	typedef storage_type::reverse_iterator reverse_iterator;

private:
	storage_type m_queue; ///< 現在マウスが載っているコントロール

public:
	mouse_service();

	/// マウスの移動メッセージが上昇する時、呼び出されます
	void process_bubble_up(control *c, mouse &m);

	/// マウスの移動メッセージが最前面に到達した時、呼び出されます
	void process_bubble_top(control *c, mouse &m);

	/// マウス・ポインタがコントロール外に移動したか検査し、処理します
	bool process_mouse_out(control *c0, control *c, mouse &m);

	/// マウス・ポインタがコントロールから外れたとき呼び出されます
	void process_mouse_leave(control *c);
};

class style_service
{

};

class timer_service
{
public:
	typedef std::unique_ptr<timer_service> store;
	typedef std::deque<message::store> storage_type;
	typedef storage_type::iterator iterator;

	typedef std::mutex mutex_type;

private:
	window_service *m_window_service;

	storage_type m_queue;
	mutex_type m_mutex;
	std::thread m_thread;

	std::atomic_bool m_run_flag;

public:
	timer_service();

	virtual ~timer_service();

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

class message_service
{
public:
	typedef std::deque<message::store> storage_type;
	typedef storage_type::iterator iterator;

	typedef std::mutex mutex_type;

private:
	detail::native_message_service::store m_native; ///< pimpl
	window_service *m_window_service;

	mutex_type m_mutex;
	storage_type m_queue;

public:
	message_service();

	virtual ~message_service();

	detail::native_message_service* get_native();

	void set_window_service(window_service *ws);

	void push(message::store s);

	message::store pop();

	void erase(control *c);

	void run();

	void quit();

	// タイマー ---------------------------------------------------------------

	/// ms秒後にcで発火するタイマーを設定します
	void set_timer(control *c, std::chrono::milliseconds ms);

	void do_tack();
};

class window_service
{
public:
	typedef std::unique_ptr<root_window> root_store;
	typedef std::deque<root_store> storage_type;

private:
	storage_type m_windows; ///< スレッドのウィンドウ群

	message_service m_message_service; ///< メッセージ・キュー
	//timer_service m_timer_service;
	layout_service m_layout_service;
	mouse_service m_mouse_service; ///< マウス・カーソルのトラッキングを行う
	style_service m_style_service; ///< スタイル

public:
	window_service();

	virtual ~window_service();


	// ウィンドウ -------------------------------------------------------------

	/// ルート・ウィンドウを末尾に追加します
	root_window* push_back(root_store s);

	root_store remove(root_window* rw);

	// キュー -----------------------------------------------------------------

	/// メッセージmをキューの末尾に追加します
	void post_message(message::store m);

	/// c及びcを祖先に持つコントロールの全てのメッセージをキューから取り除きます
	void erase_message(control *c);

	// メッセージ・ループ -----------------------------------------------------

	/// メッセージ・ループを実行します
	void run();

	/// メッセージ・ループを停止します
	void quit();

	// タイマー ---------------------------------------------------------------

	/// ms秒後にcで発火するタイマーを設定します
	void set_timer(control *c, std::chrono::milliseconds ms);

	// レイアウト -------------------------------------------------------------

	// マウス -----------------------------------------------------------------

	layout_service& get_layout_service();

	mouse_service& get_mouse_service();

	style_service& get_style_service();
};









} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_SERVICE_H
