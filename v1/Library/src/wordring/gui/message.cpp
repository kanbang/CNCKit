/**
 * @file    wordring/gui/message.cpp
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

#include <wordring/gui/message.h>
#include <wordring/gui/control.h>

using namespace wordring::gui;

// message --------------------------------------------------------------------

message::message(control *c, int32_t type) : m_control(c), m_type(type)
{

}

message::~message()
{

}

message::store message::create(control *c, int32_t type)
{
	return store(new message(c, type));
}

bool message::operator ==(control const* c) const
{
	return m_control == c;
}

control& message::get_control()
{
	assert(m_control != nullptr);
	return *m_control;
}

// timer_message --------------------------------------------------------------

timer_message::timer_message(control *c) : message(c, message::timer)
{
}

timer_message::timer_message(control *c, callback_type f)
	: message(c, message::timer)
	, m_callback(std::move(f))
{
}

timer_message::~timer_message()
{

}

message::store timer_message::create(control *c)
{
	return store(new timer_message(c));
}


message::store timer_message::create(control *c, callback_type f)
{
	return store(new timer_message(c, std::move(f)));
}