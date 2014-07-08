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
	typedef wordring::gui::detail::native_window window_type;

private:

	void append(window_type* window_ptr);
	void remove();

	virtual void run();
};

typedef win32_window_service_impl native_window_service_impl;





} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_SERVICE_H
