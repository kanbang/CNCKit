/**
 * @file    wordring/gui/detail/win32/win32_window_service.h
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

#ifndef WORDRING_WIN32_WINDOW_SERVICE_H
#define WORDRING_WIN32_WINDOW_SERVICE_H

#include <wordring/gui/detail/native_window_service.h>
#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/detail/win32/win32_window.h>

#include <Windows.h>

#include <utility>
#include <vector>

#include <cassert>

namespace wordring
{
namespace gui
{
namespace detail
{

/**
 * @brief native_window_serviceの実装
 * 
 * pimplの実装側です。
 */
class win32_window_service_impl : public native_window_service
{
private:
	/// マップ
	std::map<HWND, native_window_impl*> m_map;

public:
	win32_window_service_impl()
	{
		win32_window_service_impl::tls_window_service = this;
	}

	virtual void run();


public:
	/// ハンドルとウィンドウ・オブジェクトのセットをマップに追加します
	static void assign(HWND hwnd, native_window_impl* pwin)
	{
		win32_window_service_impl::tls_window_service->m_map[hwnd] = pwin;
	}

	/// ハンドルからウィンドウ・オブジェクトを検索します
	static native_window_impl* find(HWND hwnd)
	{
		std::map<HWND, native_window_impl*>& map =
			win32_window_service_impl::tls_window_service->m_map;

		std::map<HWND, native_window_impl*>::iterator it = map.find(hwnd);
		if (it == map.end()) { return nullptr; }

		return it->second;
	}

	/// ハンドルを指定してウィンドウ・オブジェクトとのセットを削除します
	static void remove(HWND hwnd)
	{
		std::map<HWND, native_window_impl*>& map =
			win32_window_service_impl::tls_window_service->m_map;

		size_t n = map.erase(hwnd);
		assert(n == 1);
	}

	static __declspec(thread) win32_window_service_impl* tls_window_service;
};

typedef win32_window_service_impl native_window_service_impl;





} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_SERVICE_H
