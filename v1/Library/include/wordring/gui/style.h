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

//#include <wordring/gui/control_data.h>

#include <wordring/atom.h>
#include <wordring/graphics/color.h>
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
	int32_t key;
	union
	{
		int32_t int_value;
		font *font_value;
	};

	style_value(int32_t key_, int32_t value_);

	bool operator <(style_value const &rhs) const;

	bool operator ==(style_value const &rhs) const;

	bool operator >(style_value const &rhs) const;
};

class style
{
public:
	typedef std::unique_ptr<style> store;

	typedef std::vector<style_value> storage_type;

	typedef storage_type::iterator iterator;
	typedef storage_type::const_iterator const_iterator;

	typedef std::pair<iterator, iterator> range_type;
	typedef std::pair<const_iterator, const_iterator> const_range_type;

public:
	enum : int32_t
	{
		style_class, ///< スタイル・クラス

		// 色
		bg_color, ///< 背景色
		fg_color, ///< 前景色

		control_specific = 20000, ///< これ以降はコントロールが使用します
	};

	struct active
	{
		enum : int32_t
		{
			bg_color = 1000,
			fg_color,
		};
	};

	struct focus
	{
		enum : int32_t
		{
			bg_color = 2000,
			fg_color,
		};
	};

	struct hover
	{
		enum : int32_t
		{
			bg_color = 3000,
			fg_color,
		};
	};
protected:
	storage_type m_storage; ///< key, value

protected:
	style();

public:
	static store create();

	const_iterator begin() const;

	const_iterator end() const;

	const_range_type equal_range(int32_t key) const;

	const_iterator find(int32_t key) const;

	void insert(int32_t key, int32_t value);
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

	bool find(int32_t key, style_value &result) const;

	bool find(int32_t key, rgb_color &result) const;
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
	/// C++オブジェクトとスタイルのマップ
	object_map_type m_object_map;
	/// アトムとスタイルのマップ
	atom_map_type m_atom_map;
	/// C++クラスとスタイルのマップ
	class_map_type m_class_map;

	/// 文字列と識別子を対応付けるサービス
	wordring::atom_service m_atom_service;

public:
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
