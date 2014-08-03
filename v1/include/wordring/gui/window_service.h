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

#include <wordring/gui/message.h>

#include <memory>
#include <deque>

namespace wordring
{
namespace gui
{

class dummy;

class window_service
{
private:
	std::unique_ptr<detail::native_window_service> m_native;
	std::deque<message::store> m_queue;
public:
	int32_t m_debug_control_cnt;

public:
	window_service();
	virtual ~window_service();

	detail::native_window_service& get_native();

	int get_debug_message_count();

	void push(message::store m);
	message::store pop();

	void run();
	void quit();

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
