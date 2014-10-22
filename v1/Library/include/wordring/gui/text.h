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

class text_service
{
private:
	detail::native_text_service::store m_native;

public:
	text_service();

	detail::native_text_service* get_native();
};

/**
 * @brief   テキスト片
 */
class text_control : public control
{
public:
	typedef std::unique_ptr<text_control> store;

private:
	detail::native_text::store m_text;

public:
	text_control();

	void set_string(std::wstring s);

	detail::native_text* get_native_text();
};

class text_render : public render
{
private:
	detail::native_text_render::store m_native;

	text_render();
};












} // namespace gui
} // namespace wordring

#endif // WORDRING_TEXT_H
