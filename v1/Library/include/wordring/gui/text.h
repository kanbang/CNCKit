/**
 * @file    wordring/gui/text.h
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

#ifndef WORDRING_TEXT_H
#define WORDRING_TEXT_H

#include <wordring/gui/control.h>
#include <wordring/gui/render.h>

#include <wordring/gui/detail/native_text.h>

#include <memory>
#include <string>

namespace wordring
{
namespace gui
{
	
class dummy;

class text_render : public render
{
private:
	detail::native_text_render::store m_native;

public:
	text_render();

	detail::native_text_render* operator->();

	detail::native_text_render const* operator->() const;
};

class text_service
{
private:
	detail::native_text_service::store m_native;

	render::store m_default_render;

public:
	text_service();

	detail::native_text_service* get_native();

	text_render::store get_default_render();
};

class text
{
private:
	detail::native_text::store m_native;

public:
	text();

	detail::native_text* get_native();

	detail::native_text const* get_native() const;

	detail::native_text* operator->();

	detail::native_text const* operator->() const;
};

/**
 * @brief   テキスト片
 */
class text_control : public control
{
public:
	typedef std::unique_ptr<text_control> store;

private:
	text m_text;

public:
	text_control();

	void set_string(std::wstring s);

	text& get_text();

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);

	virtual void do_size(size_int size);
};












} // namespace gui
} // namespace wordring

#endif // WORDRING_TEXT_H
