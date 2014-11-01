/**
 * @file    
 *
 * @brief   wordring/gui/detail/native_text.h
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

#ifndef WORDRING_NATIVE_TEXT_H
#define WORDRING_NATIVE_TEXT_H

#include <memory>
#include <string>

namespace wordring
{
namespace gui
{
namespace detail
{

class dummy;

class native_text_service
{
public:
	typedef std::unique_ptr<native_text_service> store;
};

class native_text
{
public:
	typedef std::unique_ptr<native_text> store;

public:
	virtual void set_string(std::wstring s) = 0;

	//virtual void set_base_font(font::store f) = 0;
};

class native_text_render
{
public:
	typedef std::unique_ptr<native_text_render> store;

public:
	virtual void set_text(native_text *nt) = 0;
};


} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_TEXT_H
