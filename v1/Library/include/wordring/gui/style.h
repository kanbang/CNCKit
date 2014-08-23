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
#include <vector>
#include <string>

namespace wordring
{
namespace gui
{

class control;

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

struct value
{
	typedef std::unique_ptr<value> store;
};

struct color_value : value
{
	rgb_color val;

	operator rgb_color();
};

struct atom_value : value
{
	typedef std::vector<int32_t> storage_type;

	typedef storage_type::const_iterator const_iterator;
	typedef storage_type::const_reverse_iterator const_reverse_iterator;

	storage_type storage;

	void erase(int32_t atom);

	void push_back(int32_t atom);

	const_iterator begin() const;

	const_iterator end() const;

	const_reverse_iterator rbegin() const;

	const_reverse_iterator rend() const;
};

class style
{
public:
	typedef std::unique_ptr<style> store;

	typedef std::pair<int32_t, value::store> value_type;
	typedef std::vector<value_type> storage_type;

public:
	enum : int32_t
	{
		class_name,
		background_color,
	};

protected:
	storage_type m_storage; ///< key, value

public:
	value* find(int32_t key);
};

class style_service
{
public:
	typedef std::unordered_map<control const*, style::store> id_map_type;
	typedef std::unordered_map<int32_t, style::store> class_map_type;

private:
	id_map_type m_id_map;
	class_map_type m_class_map;

	atom_service m_atom_service;

public:
	/// コントロールcに関連付けられたスタイルを消去します
	void erase(control const *c);

	/// クラス・アトムに関連付けられたスタイルを消去します
	void erase(int32_t atom);

	/// クラス名に関連付けられたスタイルを消去します
	void erase(std::wstring name);

	/**
	 * @brief   コントロールcに関連付けられたスタイルを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(control const *c);

	/**
	 * @brief   クラス・アトムに関連付けられたスタイルを検索します
	 *
	 * @param   atom クラス・アトム
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(int32_t atom);

	/**
	 * @brief   クラス名に関連付けられたスタイルを検索します
	 *
	 * @param   name クラス名
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	style* find(std::wstring name);

	value* find(control *c, int32_t key);

	int32_t get_atom(std::wstring name);

	style* insert(control const *c);

	style* insert(int32_t atom);

	style* insert(std::wstring name);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_STYLE_H
