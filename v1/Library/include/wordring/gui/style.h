/**
 * @file    wordring/gui/style.h
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

#ifndef WORDRING_STYLE_H
#define WORDRING_STYLE_H

#include <wordring/wordring.h>

#include <wordring/atom.h>
#include <wordring/gui/color.h>
#include <wordring/gui/font.h>

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <string>
#include <iterator>
#include <cassert>

namespace wordring
{
namespace gui
{

class layout_style; // 先行宣言
class color_style;  // 先行宣言
class text_style;   // 先行宣言

class style
{
public:
	typedef std::shared_ptr<style> store;
	typedef std::weak_ptr<style>   reference;

public:
	enum class type : uint16_t
	{
		control,
		color,
		text,

	};

	enum class state : uint16_t
	{
		normal,
		active,
		focus,
		hover,
	};

	enum class index : uint32_t
	{
		min_width,
		max_width,
		min_height,
		max_height,

		margin_left,
		margin_right,
		margin_top,
		margin_bottom,

		padding_left,
		padding_right,
		padding_top,
		padding_bottom,

		border_width,
		border_style,

		text_color,
		foreground_color,
		background_color,
		border_color,
	};

	

protected:
	type  m_type;
	state m_state;

	store    m_next;

protected:
	style(type tp, state st);

public:
	// 見つからない場合nullptr
	style const* find(type tp, state st) const
	{
		style const *s = this;
		while (s)
		{
			if (s->m_type == tp && s->m_state == st) break;
			s = s->m_next.get();
		}
		return s;
	}

	// 見つからない場合nullptr
	style* find(type tp, state st)
	{
		style *s = this;
		while (s)
		{
			if (s->m_type == tp && s->m_state == st) break;
			s = s->m_next.get();
		}
		return s;
	}

	// 見つからない場合nullptr
	style const* find(type tp, state st1, state st2) const
	{
		style const *s1 = this;
		style const *s2 = nullptr;
		while (s1)
		{
			if (s1->m_type == tp && s1->m_state == st1) break;
			if (s1->m_type == tp && s1->m_state == st2) s2 = s1;
			s1 = s1->m_next.get();
		}
		return s1 ? s1 : s2;
	}

	style* push_back(style::store s)
	{
		style::store &next = m_next;
		while (next)
		{
			next = next->m_next;
		}
		next = s;
		return s.get();
	}
};

class control_style : public style
{
public:
	int16_t min_width;
	int16_t max_width;
	int16_t min_height;
	int16_t max_height;

	int16_t margin_left;
	int16_t margin_right;
	int16_t margin_top;
	int16_t margin_bottom;

	int16_t padding_left;
	int16_t padding_right;
	int16_t padding_top;
	int16_t padding_bottom;

	int16_t border_width;
	int16_t border_style;

	color_rgb color;
	color_rgb foreground_color;
	color_rgb background_color;
	color_rgb border_color;

	int16_t     line_height;
	font::store font;

	int8_t align;
	

private:
	explicit control_style(style::state st);

public:
	static style::store create(style::state st);
};

class text_style : public style
{

};




} // namespace gui
} // namespace wordring

#endif // WORDRING_STYLE_H
