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

#include <wordring/gui/control.h>

#include <cstdint>
#include <cassert>
#include <memory>

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
		layout,
	};

	control *m_control;
	int32_t m_type;

protected:
	message(control *c, int32_t type) : m_control(c), m_type(type)
	{
		
	}

public:
	virtual ~message()
	{
	}

	static store create(control *c, int32_t type)
	{
		return std::move(store(new message(c, type)));
	}

	control& get_control()
	{
		assert(m_control != nullptr);
		return *m_control;
	}
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_MESSAGE_H
