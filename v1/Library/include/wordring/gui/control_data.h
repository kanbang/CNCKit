/**
 * @file    wordring/gui/control_data.h
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

#ifndef WORDRING_CONTROL_DATA_H
#define WORDRING_CONTROL_DATA_H

#include <wordring/atom.h>

#include <unordered_map>
#include <typeindex>

#include <memory>

#include <cstdint>

namespace wordring
{
namespace gui
{

class control; // 先行宣言

class control_data
{
public:
	typedef std::unique_ptr<control_data> store;

	enum : int32_t
	{
		style = 10000,
		control_specific = 20000,
	};

private:
	int32_t m_type;

protected:
	control_data(int32_t type);

public:
	bool is_type_of(int32_t type);
};

class control_data_service
{
public:
	typedef std::unordered_multimap<control const*, control_data::store>
		object_map_type;
	typedef std::unordered_multimap<int32_t, control_data::store>
		atom_map_type;
	typedef std::unordered_multimap<std::type_index, control_data::store>
		class_map_type;

	typedef object_map_type::iterator object_iterator;
	typedef object_map_type::const_iterator const_object_iterator;

	typedef atom_map_type::iterator atom_iterator;
	typedef atom_map_type::const_iterator const_atom_iterator;

	typedef class_map_type::iterator class_iterator;
	typedef class_map_type::const_iterator const_class_iterator;

	typedef std::pair<object_iterator, object_iterator> object_range_type;
	typedef std::pair<const_object_iterator, const_object_iterator>
		const_object_range_type;

	typedef std::pair<atom_iterator, atom_iterator> atom_range_type;
	typedef std::pair<const_atom_iterator, const_atom_iterator>
		const_atom_range_type;
	typedef std::pair<class_iterator, class_iterator> class_range_type;
	typedef std::pair<const_class_iterator, const_class_iterator>
		const_class_range_type;



private:
	/// C++オブジェクトとデータのマップ
	object_map_type m_object_map;
	/// アトムとデータのマップ
	atom_map_type m_atom_map;
	/// C++クラスとデータのマップ
	class_map_type m_class_map;

	/// 文字列と識別子を対応付けるサービス
	wordring::atom_service m_atom_service;

public:
	// 消去 -------------------------------------------------------------------

	/// C++オブジェクトに関連付けられたデータを消去します
	void erase(control const *c);

	/// アトムに関連付けられたデータを消去します
	void erase(int32_t atom);

	/// アトムに関連付けられたデータを消去します
	void erase(std::wstring name);

	/// C++クラスに関連付けられたデータを消去します
	void erase(std::type_index type);

	// 検索 -------------------------------------------------------------------

	/// 
	object_range_type equal_range(control const *c);

	/// 
	atom_range_type equal_range(int32_t atom);

	/// 
	atom_range_type equal_range(std::wstring name);

	/// 
	class_range_type equal_range(std::type_index type);

	/**
	 * @brief   C++オブジェクトに関連付けられたデータを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	control_data* find(control const *c, int32_t type);

	/**
	 * @brief   C++オブジェクトに関連付けられたデータを検索します
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	control_data const* find(control const *c, int32_t type) const;

	/**
	 * @brief   アトムに関連付けられたデータを検索します
	 *
	 * @param   atom 識別子
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	control_data* find(int32_t atom);

	/**
	 * @brief   アトムに関連付けられたデータを検索します
	 *
	 * @param   atom 識別子
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	control_data const* find(int32_t atom) const;

	/**
	 * @brief   アトムに関連付けられたデータを検索します
	 *
	 * @param   name アトム名
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	control_data* find(std::wstring name);

	/**
	 * @brief   C++クラスに関連付けられたデータを検索します
	 *
	 * @param   type C++クラスのtype_index
	 *
	 * @return  登録されていない場合、nullptrを返します
	 */
	control_data* find(std::type_index type);

	/**
	 * @brief   文字列に対応する一意の識別子を取得します
	 *
	 * @param   name 文字列
	 *
	 * @return  アトム
	 */
	int32_t get_atom(std::wstring name);

	control_data* insert(control const *c, control_data::store s);

	control_data* insert(int32_t atom, control_data::store s);

	control_data* insert(std::wstring name, control_data::store s);

	control_data* insert(std::type_index type, control_data::store s);
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_DATA_H
