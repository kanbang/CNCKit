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

#include <memory>

namespace wordring
{
namespace gui
{

class window_service
{
private:
	std::unique_ptr<detail::native_window_service> m_native;

public:
	window_service();
	virtual ~window_service() { }

	detail::native_window_service* get_native_window_service();

	void run();
	void quit();
};









} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_SERVICE_H
