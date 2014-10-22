/**
 * @file    wordring/gui/detail/win32/win32_ime.h
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

#ifndef WORDRING_WIN32_IME_H
#define WORDRING_WIN32_IME_H

#include <wordring/wordring.h>

#include <wordring/gui/shape_int.h>

#include <wordring/gui/detail/win32/win32_font.h>

#include <imm.h>
#include <Windows.h>

#include <memory>
#include <string>
#include <vector>

namespace wordring
{
namespace gui
{
namespace detail
{

class native_text_render_impl;

class native_ime_impl
{
public:
	typedef std::unique_ptr<native_ime_impl> store;

private:
	native_text_render_impl *m_render;
	native_font_impl *m_font;

	uint32_t m_insert_point; // 文字列の挿入位置
	uint32_t m_slength;      // 文字列長

	std::vector<unsigned char> m_attributes; ///< 文字属性情報
	std::vector<uint32_t>      m_clauses;    ///< 文節情報

public:
	native_ime_impl(native_text_render_impl *render, native_font_impl *f);

	~native_ime_impl();

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





} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_IME_H
