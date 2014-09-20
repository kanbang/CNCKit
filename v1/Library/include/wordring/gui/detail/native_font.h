/**
 * @file    wordring/gui/detail/native_font.h
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

#ifndef WORDRING_NATIVE_FONT_H
#define WORDRING_NATIVE_FONT_H

#include <wordring/debug.h>

#include <memory>

namespace wordring
{
namespace gui
{

class font; // 先行宣言

namespace detail
{

class dummy;

class native_font
{
public:
	typedef std::unique_ptr<native_font> store;

private:
	wordring::gui::font *m_public;

public:
	virtual ~native_font() { }

	wordring::gui::font* get_public()
	{
		return m_public;
	}

	void set_public(wordring::gui::font* f)
	{
		m_public = f;
	}
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_FONT_H
