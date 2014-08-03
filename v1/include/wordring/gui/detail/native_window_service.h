/**
 * @file    wordring/gui/detail/native_window_service.h
 *
 * @brief   ウィンドウ・サービスのpimplインターフェース
 *
 * @details
 *          
 *          このファイルには、pimplの公開されるインターフェースを定義します。\n
 *          ウィンドウ・サービスは、一般的なGUIライブラリにおける
 *          メッセージ・ポンプ相当です。
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_NATIVE_WINDOW_SERVICE_H
#define WORDRING_NATIVE_WINDOW_SERVICE_H

#include <wordring/debug.h>

namespace wordring
{
namespace gui
{

class window_service;

namespace detail
{

class native_window_service
{
protected:
	window_service *m_public;

public:
	native_window_service() : m_public(nullptr)
	{
	}

	virtual ~native_window_service()
	{
	}

	/**
	 * @brief   コールバック用にpimplの公開側を登録します
	 *
	 * @param   ws ウィンドウ・サービス
	 */
	void set_public(window_service *ws)
	{
		m_public = ws;
	}

	window_service* get_public()
	{
		return m_public;
	}

	virtual void post_tick_message() =0;

	virtual void run() = 0;
	virtual void quit() = 0;
};





} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_WINDOW_SERVICE_H
