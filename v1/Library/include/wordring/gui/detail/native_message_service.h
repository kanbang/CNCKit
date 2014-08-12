/**
 * @file    wordring/gui/detail/native_message_service.h
 *
 * @brief   メッセージ・サービスのpimplインターフェース
 *
 * @details
 *          
 *          このファイルには、pimplの公開されるインターフェースを定義します。\n
 *          メッセージ・サービスは、一般的なGUIライブラリにおける
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

#ifndef WORDRING_NATIVE_MESSAGE_SERVICE_H
#define WORDRING_NATIVE_MESSAGE_SERVICE_H

#include <wordring/debug.h>

#include <memory>

namespace wordring
{
namespace gui
{

class message_service;

namespace detail
{

class native_message_service
{
public:
	typedef std::unique_ptr<native_message_service> store;

protected:
	message_service *m_public;

public:
	native_message_service() : m_public(nullptr)
	{
	}

	virtual ~native_message_service()
	{
	}

	/**
	 * @brief   コールバック用にpimplの公開側を登録します
	 *
	 * @param   ws ウィンドウ・サービス
	 */
	void set_public(message_service *ws)
	{
		m_public = ws;
	}

	message_service* get_public()
	{
		return m_public;
	}

	virtual void tick() = 0;

	virtual void run() = 0;
	virtual void quit() = 0;
};





} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_MESSAGE_SERVICE_H
