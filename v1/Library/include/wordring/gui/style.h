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

#include <wordring/graphics/color.h>

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

class atom_service
{
public:
	typedef std::unordered_map<std::wstring, int32_t> index_type;
	typedef std::vector<std::wstring> reverse_index_type;

private:
	index_type m_index;
	reverse_index_type m_reverse_index;

public:
	int32_t get_atom(std::wstring name);

	std::wstring get_string(int32_t atom);
};

struct style_value
{
	typedef std::unique_ptr<style_value> store;
};

struct color_value : style_value
{
	rgb_color val;

	operator rgb_color();
};

template <typename T>
struct vector_value_tmpl : style_value
{
	typedef T value_type;

	typedef std::vector<value_type> storage_type;

	typedef typename storage_type::const_iterator const_iterator;
	typedef typename storage_type::const_reverse_iterator const_reverse_iterator;

	storage_type storage;

	void erase(value_type v)
	{
		storage.erase(
			std::remove(storage.begin(), storage.end(), atom), storage.end());
	}

	void push_back(value_type v)
	{
		assert(std::find(begin(), end(), atom) == end());
		storage.push_back(atom);
	}

	const_iterator begin() const
	{
		return storage.begin();
	}

	const_iterator end() const
	{
		return storage.end();
	}

	const_reverse_iterator rbegin() const
	{
		return storage.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return storage.rend();
	}
};

typedef vector_value_tmpl<int32_t> class_atom_value;

class style
{
public:
	typedef std::unique_ptr<style> store;

	typedef std::pair<int32_t, style_value::store> value_type;
	typedef std::vector<value_type> storage_type;

public:
	enum : int32_t
	{
		class_atom,
		background_color,
	};

protected:
	storage_type m_storage; ///< key, value

public:
	style_value* find(int32_t key);

	style_value const* find(int32_t key) const;
};

class style_cache
{
private:
	std::vector<style const*> m_storage;

public:
	void push_back(style const *s);

	style_value const* find(int32_t key) const;
};

class style_service
{
public:
	typedef std::unordered_map<control const*, style::store> object_map_type;
	typedef std::unordered_map<int32_t, style::store> class_map_type;
	typedef std::unordered_map<std::type_index, style::store> default_map_type;

private:
	object_map_type m_object_map;
	class_map_type m_class_map;
	default_map_type m_default_map;

	atom_service m_atom_service;

public:
	/// コントロール・オブジェクトに関連付けられたスタイルを消去します
	void erase(control const *c);

	/// スタイル・クラスに関連付けられたスタイルを消去します
	void erase(int32_t atom);

	/// スタイル・クラスに関連付けられたスタイルを消去します
	void erase(std::wstring name);

	/// コントロール・クラスに関連付けられたスタイルを消去します
	void erase(std::type_index type);

	/**
	 * @brief   コントロール・オブジェクトに関連付けられたスタイルを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(control const *c);

	/**
	 * @brief   コントロール・オブジェクトに関連付けられたスタイルを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style const* find(control const *c) const;
	
	/**
	 * @brief   スタイル・クラスに関連付けられたスタイルを検索します
	 *
	 * @param   atom スタイル・クラス識別子
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(int32_t atom);

	/**
	 * @brief   スタイル・クラスに関連付けられたスタイルを検索します
	 *
	 * @param   atom スタイル・クラス識別子
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style const* find(int32_t atom) const;

	/**
	 * @brief   スタイル・クラスに関連付けられたスタイルを検索します
	 *
	 * @param   name スタイル・クラス名
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(std::wstring name);

	/**
	 * @brief   スタイル・クラスに関連付けられたスタイルを検索します
	 *
	 * @param   type コントロール・クラスのtype_index
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(std::type_index type);

	/**
	 * @brief   コントロールに関連付けられたスタイルを全て検索します
	 *
	 * @details 
	 *          コントロール・オブジェクト及び、オブジェクトに関連付けられた
	 *          スタイル・クラスをキーにすべてのスタイルを集めて返します。
	 */
	style_cache find_styles(control const *c) const;

	int32_t get_atom(std::wstring name);

	style* insert(control const *c);

	style* insert(int32_t atom);

	style* insert(std::wstring name);

	style* insert(std::type_index type);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_STYLE_H
