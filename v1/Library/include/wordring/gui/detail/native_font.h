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

#include <wordring/wordring.h>

#include <memory>

namespace wordring
{
namespace gui
{

class font; // 先行宣言

namespace detail
{

class native_canvas; // 先行宣言

class native_font
{
public:
	typedef std::shared_ptr<native_font> store;

private:
	wordring::gui::font *m_public;

public:
	virtual ~native_font() { }

	/**
	 * @brief   フォントの公開側オブジェクトを取得する
	 */
	wordring::gui::font* get_public()
	{
		return m_public;
	}

	/**
	 * @brief   フォントの公開側オブジェクトを設定する
	 */
	void set_public(wordring::gui::font* f)
	{
		m_public = f;
	}

	virtual void attach(native_canvas const *cv) = 0;
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_FONT_H
