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

#include <wordring/gui/shape_int.h>

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

	/**
	 * @brief   フォントの描画始点をオフセットとして取得する
	 *
	 * @details
	 *          Meiryoの大きさ調整によって、描画始点がずれるのを調整する目的で
	 *          最初に作られました。
	 *
	 *          Meiryoは初めてのWindows付属ユニコード・フォントであるため、これ
	 *          までのフォントと同じ大きさを指定すると小さく見えます。
	 *          そこで、全てのフォントで内部レディングの占める割合分、高さを
	 *          増やしています。
	 *          Meiryo以前のすべてのフォントの内部レディングは0なので、この
	 *          調整はうまくいきます。
	 *
	 *          しかし、描画の開始位置もずらさなければなりません。
	 *          そのために使われるオフセットを返します。
	 *
	 *          このメンバが返すオフセットはcanvasの描画メソッドによって使われ
	 *          ます。
	 */
	virtual point_int get_offset() const = 0;
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_FONT_H
