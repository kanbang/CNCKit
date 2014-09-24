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

#include <wordring/wordring.h>

#include <wordring/atom.h>

#include <wordring/gui/detail/native_font.h>

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace wordring
{
namespace gui
{

class window_service; // 先行宣言
class font_service;   // 先行宣言

namespace  detail
{

class native_canvas; // 先行宣言

} // namespace detail

/**
 * @brief   フォントを表すクラス
 *
 * @details
 *          フォントの公開側クラスです。
 *          pimplによって環境固有の実装を隠蔽しています。
 *          検索効率のために、フォント名を含めて32ビットにコード化します。
 */
class font
{
	friend class font_service;

public:
	typedef std::shared_ptr<font> store;
	typedef std::weak_ptr<font>   reference;

public:
	enum : uint32_t
	{
		weight_mask = 0xF0000000, // 31-28
		italic_mask = 0x8000000,  // 27
		family_mask = 0x7000000,  // 26-24
		size_mask   = 0xFF0000,   // 23-16
		face_mask   = 0xFFFF,     // 15-0

		weight_shift = 28,
		size_shift   = 16,

		normal     = 0,
		italic     = 0x8000000,
		sans_serif = 0x1000000, // 1
		serif      = 0x2000000, // 2
		cursive    = 0x3000000, // 3
		fantasy    = 0x4000000, // 4
		monospace  = 0x5000000, // 5
	};

private:
	detail::native_font::store m_native;

	uint32_t                   m_code;
	std::wstring               m_face;

protected:
	font(uint32_t code, std::wstring const &face);

public:
	static store create(uint32_t code, std::wstring const &face);

	detail::native_font* get_native();

	void attach(detail::native_canvas const *cv);

	void detach();

	static uint32_t create_code(
		int32_t size, int32_t family, int32_t weight, bool italic_, uint32_t face);

	uint32_t get_code() const;

	void     set_code(uint32_t code);

	int32_t  get_size() const;

	int32_t  get_family() const;

	int32_t  get_weight() const;

	bool     is_italic() const;

	std::wstring const& get_face() const;
};

class font_service
{
public:
	typedef std::unordered_map<uint32_t, font::store> storage_type;
	typedef storage_type::iterator iterator;

private:
	window_service         *m_window_service;
	wordring::atom_service  m_atom_service;

	storage_type            m_storage;

public:
	font_service();

	window_service* get_window_service();

	void set_window_service(window_service *ws);

	uint32_t create(
		int32_t      size,
		int32_t      family = font::sans_serif,
		int32_t      weight = 400,
		bool         italic = false,
		std::wstring face   = L"");

	font::store find(
		int32_t size, int32_t  family, int32_t  weight, bool italic, uint32_t face);

	font::store find(uint32_t code);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_FONT_H
