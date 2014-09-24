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
	enum class type : int16_t
	{
		layout, color, text,
	};

	enum class state : int16_t
	{
		normal, active, focus, hover,
	};

protected:
	type  m_type;
	state m_state;

	store m_next;

protected:
	style(type type_, state state_)
		: m_type(type_)
		, m_state(state_)
	{
	}

public:
	style const* find(type type_, state st = state::normal) const
	{
		style const *s = this;
		while (s)
		{
			if (s->m_type == type_ && s->m_state == st) break;
			s = s->m_next.get();
		}
		return s;
	}

	style const* find(type tp, state st1, state st2) const
	{
		style const *s1 = this;
		style const *s2 = nullptr;
		while (s1)
		{
			if (s1->m_type == tp)
			{
				if (s1->m_state == st1) break;
				if (s1->m_state == st2) s2 = s1;
			}
			s1 = s1->m_next.get();
		}
		return s1 ? s1 : s2;
	}
};

class layout_style : public style
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

private:
	layout_style();

public:
	static style::store create();
};

class color_style : public style
{
public:
	color_rgb color;
	color_rgb foreground_color;
	color_rgb background_color;
	color_rgb border_color;

public:
	color_style(style::state st)
		: style(style::type::color, st)
		, color(color_rgb(0, 0, 0))
		, foreground_color(color_rgb(0, 0, 0))
		, background_color(color_rgb(0xFF, 0xFF, 0xFF))
		, border_color(color_rgb(0x80, 0x80, 0x80))
	{
	}

	static style::store create(style::state st)
	{
		return style::store(new color_style(st));
	}
};

class text_style : public style
{
public:
	int16_t line_height;
	font::store m_font;

};


} // namespace gui
} // namespace wordring

#endif // WORDRING_STYLE_H
