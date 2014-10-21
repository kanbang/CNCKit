
#pragma once

#include "item.h"

#include "ime.h"

#include <string>

class text_render;

class text
{
	friend class text_render;

public:
	typedef std::vector<item> storage_type;

	typedef storage_type::iterator       iterator;
	typedef storage_type::const_iterator const_iterator;

protected:
	std::wstring      m_string;
	std::vector<item> m_storage;

protected:
	void set_string(std::wstring s);

	void insert_string(uint32_t ipoint, std::wstring s);

	void erase_string(uint32_t ipoint, uint32_t slength);

	wchar_t const* get_sdata() const;

	uint32_t get_slength() const;

	void clear();

	void push_back(item const &i);

	void assign(const_iterator it1, const_iterator it2);

	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;
};

class control;

class item_breaker
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	SCRIPT_DIGITSUBSTITUTE *m_sds;

	output_function m_output;

public:
	item_breaker(SCRIPT_DIGITSUBSTITUTE *sds);

	void set_output_function(output_function output);

	void operator()(wchar_t const *pch, uint32_t slength);
};

class font_binder
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	font::store m_base_font;

	output_function m_output;

public:
	font_binder(font::store f);

	void set_output_function(output_function output);

	void operator()(render_item ri);
};

class image_binder
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	std::vector<item> m_storage;

	output_function m_output;

public:
	//image_binder();

	void set_output_function(output_function output);

	void operator()(render_item ri);
};

class line_breaker
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	output_function m_output;

	HDC m_hdc;

	uint32_t m_width; // 最大幅

	uint32_t m_axpx; // 前進x位置
	uint32_t m_aypx; // 前進y位置

	uint32_t m_dy; // 行高さ検出用

	uint32_t m_ach; // 前進文字数
	uint32_t m_act; // 前進キャレット数

public:
	line_breaker(HDC hdc, uint32_t width);

	void set_output_function(output_function output);

	void operator()(render_item ri);

	void break_line();

	void output(render_item &&ri);
};

class text_render
{
public:
	typedef std::vector<render_item> storage_type;

	typedef storage_type::iterator       iterator;
	typedef storage_type::const_iterator const_iterator;

protected:
	SCRIPT_DIGITSUBSTITUTE *m_sds;

	text    *m_text;
	control *m_control;

	storage_type  m_storage;

	font::store m_base_font;

	uint32_t m_width;
	uint32_t m_height;

	uint32_t m_nct;

	ime::store m_ime;

public:
	text_render(SCRIPT_DIGITSUBSTITUTE *sds, text *txt);

	void set_control(control *c);

	void parse(HDC hdc);

	void set_size(HDC hdc, uint32_t cx, uint32_t cy);

	// 文字数を返す
	uint32_t get_slength() const;

	// クラスタ数を返す
	uint32_t get_clength() const;

	// キャレット数を返す
	uint32_t get_ctlength() const;

	// キャレット -------------------------------------------------------------

	void set_caret_position(uint32_t ict);

	void move_caret_left();

	void move_caret_right();
	
	void move_caret_up();
	
	void move_caret_down();

	void move_caret_home();

	void move_caret_end();

	iterator find_item_from_ctlength(uint32_t nct);

	iterator find_item_from_slength(uint32_t slength);

	// キャレット数から文字数を返す
	uint32_t query_slength_from_ctlength(uint32_t nct) const;

	// 文字数からキャレット数を返す
	uint32_t query_ctlength_from_slength(uint32_t slength) const;

	uint32_t get_nct(uint32_t xct, uint32_t yct) const;

	// キャレット数からxy位置を検索
	point find_pt_by_nct(uint32_t nct) const;

	// xy位置からキャレット数を返す
	uint32_t find_nct_by_pt(uint32_t xpt, uint32_t ypt) const;

	uint32_t find_max_yct() const;

	uint32_t find_max_xct(uint32_t yct) const;

	uint32_t find_index_by_yct(uint32_t yct) const;

	uint32_t find_xpt_from_xct(uint32_t xct, uint32_t yct) const;

	uint32_t find_xct_from_xpt(uint32_t xpt, uint32_t yct) const;

	uint32_t find_ypt_from_yct(uint32_t yct) const;

	uint32_t find_yct_from_ypt(uint32_t ypt) const;

	void paint(HDC hdc);

	// ich 文字列での開始位置
	// nch 文字数
	void paint_under_line(HDC hdc, uint32_t ich, uint32_t nch);

	void invalidate(HWND hwnd);

	void clear();

	void set_string(HDC hdc, std::wstring s);

	// ipoint 文字列の位置
	void insert_string(HDC hdc, uint32_t ipoint, std::wstring s);

	// ipoint 文字列の位置
	void erase_string(uint32_t ipoint, uint32_t slength);

	void assign(text::const_iterator it1, text::const_iterator it2);

	void set_base_font(font::store f);

	font::store get_base_font();

	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;

	void set_focus(HWND hwnd);

	void kill_focus(HWND hwnd);

	void start_composition(HWND hwnd);

	void end_composition(HWND hwnd);

	void process_composition(HWND hwnd, WPARAM wParam, LPARAM lParam);
};

class control
{
protected:
	SCRIPT_DIGITSUBSTITUTE *m_sds;

	text   m_text;
	text_render m_render;

	font::store m_base_font;

public:
	control(SCRIPT_DIGITSUBSTITUTE *sds);

	virtual ~control();

	text_render* get_render();

	void set_string(HDC hdc, std::wstring s);

	void set_base_font(font::store f);

	void on_char(WPARAM wparam);

	void on_l_button_down(uint32_t xpt, uint32_t ypt);
};
