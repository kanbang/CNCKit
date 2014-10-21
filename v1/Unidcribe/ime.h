#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Windows.h>
#include "item.h"
#include <imm.h>

//#include "control.h"

class text_render;

class ime
{
public:
	typedef std::unique_ptr<ime> store;

private:
	text_render *m_render;
	font::store  m_font;

	uint32_t m_insert_point; // •¶Žš—ñ‚Ì‘}“üˆÊ’u
	uint32_t m_slength;      // •¶Žš—ñ’·

	std::vector<unsigned char> m_attributes;
	std::vector<uint32_t>      m_clauses;

public:
	ime(text_render *render, font::store  f);

	~ime();

	void set_insert_point(uint32_t ipoint);

	uint32_t get_insert_point() const;

	uint32_t get_slength() const;

	uint32_t get_cursol_position(HIMC himc) const;

	uint32_t get_delta(HIMC himc) const;

	void set_candidate_position(HIMC himc, uint32_t x, uint32_t y);

	std::wstring get_composition_string(HIMC himc);

	void get_attribute(HIMC himc);

	void get_clause(HIMC himc);

	std::wstring get_result_string(HIMC himc);
};