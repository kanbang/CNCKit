/**
 * @file    wordring/gui/font.h
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

#ifndef WORDRING_FONT_H
#define WORDRING_FONT_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_font.h>

#include <string>
#include <unordered_set>
#include <functional>

namespace wordring
{
namespace gui
{

class window_service; // 先行宣言

struct font_conf
{
	short style;
	short variant;
	short weight;
	short size;
	short line_height;
	short family;

	std::wstring face;

	font_conf();
};

class font
{
public:
	enum : short
	{
		normal = 0xFFF,

		// font-style
		italic, oblique,

		// font-variant
		small_caps,
		
		// font-weight
		bold, bolder, lighter,

		// font-size
		xx_large, x_large, large, medium, small, small_ = small, x_small, xx_small,
		larger, smaller,

		// line-height

		// font-family
		sans_serif, serif, cursive, fantasy, monospace,
	};

private:
	detail::native_font::store m_native;
	font_conf m_font_conf;

public:
	font(font_conf fc);

	detail::native_font* get_native();

	font_conf const& get_conf() const;
};

/// unordered_map用
class font_hash
{
public:
	size_t operator ()(font const &f) const;
};

/// unordered_map用
class font_equal_to
	: public std::binary_function<font, font, bool>
{
public:
	typedef font first_argument_type;
	typedef font second_argument_type;
	typedef bool result_type;

	bool operator ()(font const &lhs, font const &rhs) const;
};

class font_service
{
public:
	typedef std::unordered_set<font, font_hash, font_equal_to> storage_type;

private:
	window_service *m_window_service;
	storage_type m_storage;

public:
	font_service();

	window_service* get_window_service();

	void set_window_service(window_service *ws);

};

} // namespace gui
} // namespace wordring

#endif // WORDRING_FONT_H
