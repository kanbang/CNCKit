﻿/**
 * @file    wordring/gui/window_service.h
 *
 * @brief   ウィンドウ・サービスのヘッダー・ファイル
 *
 * @details
 *          ウィンドウ・サービスはメッセージ・ポンプに相当します。\n
 *
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

#include <wordring/wordring.h>

#include <wordring/gui/message.h>
#include <wordring/gui/mouse.h>
#include <wordring/gui/style.h>
#include <wordring/gui/font.h>
#include <wordring/gui/text.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/detail/native_message_service.h>

//#include <wordring/gui/root_window.h>

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
	mouse_service m_mouse_service; ///< マウス・カーソルのトラッキングを行う
	//control_data_service m_data_service; ///< コントロールの外部データ
	font_service m_font_service;
	text_service m_text_service;

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

	mouse_service& get_mouse_service();

	// コントロール・データ ---------------------------------------------------

	//control_data_service& get_data_service();

	// スタイル ---------------------------------------------------------------


	font_service& get_font_service();

	// テキスト・サービス -----------------------------------------------------

	text_service& get_text_service();
};









} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_SERVICE_H
