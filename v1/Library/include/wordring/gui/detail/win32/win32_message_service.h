/**
 * @file    wordring/gui/detail/win32/win32_message_service.h
 *
 * @brief   ウィンドウ・サービスの環境固有ヘッダー・ファイル
 *
 * @details
 *          一般的なGUIライブラリにおけるメッセージ・ポンプ相当です。\n
 *          pimplによって実装を隠蔽します。\n
 *          このファイルは、win32専用です。ライブラリ利用者は、このヘッダー・
 *          ファイルを使用しません。
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_WIN32_MESSAGE_SERVICE_H
#define WORDRING_WIN32_MESSAGE_SERVICE_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_message_service.h>
#include <wordring/gui/detail/native_window.h>

#include <Windows.h>

#include <utility>
#include <map>

#include <cassert>

namespace wordring
{
namespace gui
{
namespace detail
{

/**
 * @brief native_message_serviceの実装
 * 
 * pimplの実装側です。
 */
class win32_message_service_impl : public native_message_service
{
private:
	/// マップ
	std::map<HWND, native_window*> m_map;
	HANDLE m_events[64];

public:
	win32_message_service_impl();
	virtual ~win32_message_service_impl();

	virtual void run();
	virtual void quit();

	/**
	 * @brief   空のイベントを送信します
	 *
	 * @details このメンバは、ライブラリが実装するメッセージキューの処理を開始
	 *          するタイミングを提供するために用意されています。
	 *
	 *          ウィンドウ・サービスは二つのメッセージキューを管理しています。
	 *          ウィンドウ・システムのメッセージキューとライブラリのメッセージ
	 *          キューの二つです。
	 *
	 *          ライブラリのメッセージキューへメッセージをポストするタイミング
	 *          でこのメンバを呼び出すと、システムのイベント
	 *          キューに空のメッセージが積まれます。
	 *          システムのイベント・キューからメッセージを取り出す
	 *          ときに空のメッセージがあるとwindow_service::do_tack()が
	 *          呼び出され、この中からライブラリのメッセージキューの処理を開始
	 *          します。
	 *
	 *          以上の仕組みによって二つのメッセージキューが同期されます。
	 */
	virtual void tick();

public:
	/// ハンドルとウィンドウ・オブジェクトのセットをマップに追加します
	static void assign(HWND hwnd, native_window* pwin);

	/// ハンドルからウィンドウ・オブジェクトを検索します
	static native_window* find(HWND hwnd)
	{
		std::map<HWND, native_window*>& map =
			win32_message_service_impl::tls_window_service->m_map;

		std::map<HWND, native_window*>::iterator it = map.find(hwnd);
		if (it == map.end()) { return nullptr; }

		return it->second;
	}

	/// ハンドルを指定してウィンドウ・オブジェクトとのセットを削除します
	static void remove(HWND hwnd)
	{
		std::map<HWND, native_window*>& map =
			win32_message_service_impl::tls_window_service->m_map;

		size_t n = map.erase(hwnd);
		assert(n == 1);
	}

	static __declspec(thread) win32_message_service_impl* tls_window_service;
};

typedef win32_message_service_impl native_message_service_impl;

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_MESSAGE_SERVICE_H
