/**
 * @file    wordring/gui/message.h
 *
 * @brief   
 *
 * @details
 *          
 *          
 *          
 *          
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_MESSAGE_H
#define WORDRING_MESSAGE_H

#include <cassert>

#include <cstdint>
#include <memory>

#include <functional>
#include <chrono>

namespace wordring
{
namespace gui
{

class control;

struct message
{
	typedef std::unique_ptr<message> store;

	enum
	{
		layout, timer
	};

	control *m_control;
	int32_t m_type;

protected:
	message(control *c, int32_t type);

public:
	virtual ~message();

	static store create(control *c, int32_t type);

	bool operator ==(control const* c) const;

	control& get_control();
};

struct timer_message : message
{
	typedef std::function<void()> callback_type;

	callback_type m_callback;

protected:
	timer_message(control *c);

	timer_message(control *c, callback_type f);

public:
	virtual ~timer_message();

	static message::store create(control *c);

	static message::store create(control *c, callback_type f);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_MESSAGE_H
