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

class control; // 先行宣言
class style_service; // 先行宣言

struct style_value
{
	uint32_t key;
	int32_t  value;

	style_value(uint32_t key_, int32_t value_);

	style_value();

	operator bool() const;

	void operator =(int32_t value_);

	bool operator <(style_value const &rhs) const;

	bool operator ==(style_value const &rhs) const;

	bool operator >(style_value const &rhs) const;
};

class style
{
public:
	typedef std::unique_ptr<style> store;

	typedef std::vector<style_value> storage_type;
	typedef std::vector<font::store> font_storage_type;

	typedef storage_type::iterator iterator;
	typedef storage_type::const_iterator const_iterator;

	typedef std::pair<iterator, iterator> range_type;
	typedef std::pair<const_iterator, const_iterator> const_range_type;

public:
	struct state
	{
		enum : uint32_t
		{
			mask   = 0xFF000000, // 上位8ビット
			normal = 0,          // 0
			active = 0x1000000,  // 1
			focus  = 0x2000000,  // 2
			hover  = 0x3000000,  // 3
		};
	};

	enum : uint32_t
	{
		mask  = 0xFFFFFF, // 下位24ビット 
		error = 0,

		style_class, ///< スタイル・クラス

		width,
		min_width,
		max_width,

		height,
		min_height,
		max_height,

		font,
		color,

		foreground_key = 10000,
		background_key = 10100,
		border_key     = 10200,
		margin_key     = 10300,
		padding_key    = 10400,

		control_specific = 20000, ///< これ以降はコントロールが使用します
	};

	struct foreground
	{
		enum : uint32_t
		{
			color = foreground_key,
		};
	};

	struct background
	{
		enum : uint32_t
		{
			color = background_key,
			image,
			repeat,
		};
	};
	
	struct border
	{
		enum : uint32_t
		{
			color = border_key,
			width,
			style,
		};
	};

	struct margin
	{
		enum : int32_t
		{
			all = style::margin_key,
			left,
			right,
			top,
			bottom,
		};
	};

	struct padding
	{
		enum : int32_t
		{
			all = style::padding_key,
			left,
			right,
			top,
			bottom,
		};
	};

protected:
	storage_type m_storage; ///< key, value

protected:
	style();

public:
	static store create();

	//style_value& operator [](uint32_t key);

	const_iterator begin() const;

	const_iterator end() const;

	iterator begin();

	iterator end();

	const_range_type equal_range(uint32_t key) const;

	const_iterator find(uint32_t key) const;

	iterator find(uint32_t key);

	void insert(uint32_t key, int32_t value);

	void insert(uint32_t key, uint32_t value);

	void insert(uint32_t key, wordring::gui::font::store value);
};

class style_cache
{
private:
	/// 文字列やフォントを検索するために使います
	style_service const &m_style_service;
	std::vector<style const*> m_storage;

public:
	style_cache(style_service const &ss);

	void push_back(style const *s);

	style_value find(uint32_t key) const;

	color_rgb find(uint32_t key, color_rgb default_) const;

	int32_t find(uint32_t key, int32_t default_) const;

	uint32_t find(uint32_t key, uint32_t default_) const;
};

class style_service
{
public:
	typedef std::unordered_map<void const*, style::store>
		object_map_type;
	typedef std::unordered_map<int32_t, style::store>
		atom_map_type;
	typedef std::unordered_map<std::type_index, style::store>
		class_map_type;

	typedef object_map_type::iterator object_iterator;
	typedef object_map_type::const_iterator const_object_iterator;

	typedef atom_map_type::iterator atom_iterator;
	typedef atom_map_type::const_iterator const_atom_iterator;

	typedef class_map_type::iterator class_iterator;
	typedef class_map_type::const_iterator const_class_iterator;

private:
	window_service *m_window_service;

	/// C++オブジェクトとスタイルのマップ
	object_map_type m_object_map;
	/// アトムとスタイルのマップ
	atom_map_type m_atom_map;
	/// C++クラスとスタイルのマップ
	class_map_type m_class_map;

	/// 文字列と識別子を対応付けるサービス
	wordring::atom_service m_atom_service;

public:
	// 構築・破棄 -------------------------------------------------------------
	
	style_service();

	// 
	window_service& get_window_service();

	void set_window_service(window_service *ws);

	font_service& get_font_service();

	// 消去 -------------------------------------------------------------------

	/// C++オブジェクトに関連付けられたデータを消去します
	void erase(void const *p);

	/// アトムに関連付けられたデータを消去します
	void erase(int32_t atom);

	/// アトムに関連付けられたデータを消去します
	void erase(std::wstring name);

	/// C++クラスに関連付けられたデータを消去します
	void erase(std::type_index type);

	// 検索 -------------------------------------------------------------------

	/**
	 * @brief   C++オブジェクトに関連付けられたスタイルを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(void const *p);

	/**
	 * @brief   C++オブジェクトに関連付けられたスタイルを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style const* find(void const *p) const;

	/**
	 * @brief   アトムに関連付けられたスタイルを検索します
	 *
	 * @param   atom 識別子
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(int32_t atom);

	/**
	 * @brief   アトムに関連付けられたスタイルを検索します
	 *
	 * @param   atom 識別子
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style const* find(int32_t atom) const;

	/**
	 * @brief   アトムに関連付けられたスタイルを検索します
	 *
	 * @param   name アトム名
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(std::wstring name);

	/**
	 * @brief   C++クラスに関連付けられたスタイルを検索します
	 *
	 * @param   type C++クラスのtype_index
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(std::type_index type);

	/**
	 * @brief   C++クラスに関連付けられたスタイルを検索します
	 *
	 * @param   type C++クラスのtype_index
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style const* find(std::type_index type) const;

	/**
	 * @brief   C++オブジェクトに関連付けられたスタイルを全て検索します
	 *
	 * @details
	 *          C++オブジェクト及び、オブジェクトに関連付けられた
	 *          スタイル・クラスをキーにすべてのスタイルを集めて返します。
	 */
	style_cache find_styles(void const *p) const;

	style_cache find_styles(std::type_index ti, void const *p) const;

	/**
	 * @brief   文字列に対応する一意の識別子を取得します
	 *
	 * @param   name 文字列
	 *
	 * @return  アトム
	 */
	int32_t get_atom(std::wstring name);

	style* insert(void const *p);

	style* insert(int32_t atom);

	style* insert(std::wstring name);

	style* insert(std::type_index type);
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_STYLE_H
