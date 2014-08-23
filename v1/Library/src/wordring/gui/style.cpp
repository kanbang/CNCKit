/**
 * @file    wordring/gui/style.cpp
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

#include <wordring/debug.h>

#include <wordring/gui/control.h>

#include <wordring/gui/style.h>

#include <string>
#include <algorithm>

#include <cassert>

using namespace wordring::gui;

// atom_service ---------------------------------------------------------------

int32_t atom_service::get_atom(std::wstring name)
{
	int32_t result;

	index_type::iterator it = m_index.find(name);
	if (it == m_index.end())
	{
		result = m_reverse_index.size();
		m_index[name] = result;
		m_reverse_index.push_back(name);
	}
	else
	{
		result = it->second;
	}

	return result;
}

std::wstring atom_service::get_string(int32_t atom)
{
	assert(atom < static_cast<int32_t>(m_reverse_index.size()));
	return m_reverse_index[atom];
}

// value ----------------------------------------------------------------------

// color_value ----------------------------------------------------------------

color_value::operator rgb_color()
{
	return val;
}

// atom_value -----------------------------------------------------------------

void atom_value::erase(int32_t atom)
{
	storage.erase(
		std::remove(storage.begin(), storage.end(), atom), storage.end());
}

void atom_value::push_back(int32_t atom)
{
	assert(std::find(begin(), end(), atom) == end());
	storage.push_back(atom);
}

atom_value::const_iterator atom_value::begin() const
{
	return storage.begin();
}

atom_value::const_iterator atom_value::end() const
{
	return storage.end();
}

atom_value::const_reverse_iterator atom_value::rbegin() const
{
	return storage.rbegin();
}

atom_value::const_reverse_iterator atom_value::rend() const
{
	return storage.rend();
}

// style ----------------------------------------------------------------------

value* style::find(int32_t key)
{
	storage_type::iterator it = std::find_if(
		m_storage.begin(),
		m_storage.end(),
		[=](value_type &v)->bool{ return v.first == key; });

	return (it == m_storage.end()) ? nullptr : it->second.get();
}

// style_service --------------------------------------------------------------

void style_service::erase(control const *c)
{
	m_id_map.erase(c);
}

void style_service::erase(int32_t atom)
{
	m_class_map.erase(atom);
}

void style_service::erase(std::wstring name)
{
	int32_t atom = m_atom_service.get_atom(name);
	erase(atom);
}

style* style_service::find(control const *c)
{
	id_map_type::iterator it = m_id_map.find(c);
	if (it == m_id_map.end()) { return nullptr; }

	return it->second.get();
}

style* style_service::find(int32_t atom)
{
	class_map_type::iterator it = m_class_map.find(atom);
	if (it == m_class_map.end()) { return nullptr; }

	return it->second.get();
}

style* style_service::find(std::wstring name)
{
	int32_t atom = m_atom_service.get_atom(name);

	return find(atom);
}

value* style_service::find(control *c, int32_t key)
{
	value *result = nullptr;
	style *s = find(c); // オブジェクトのスタイルを検索

	if (s == nullptr) { return nullptr; }

	// オブジェクトのスタイルからkeyの値を検索
	if ((result = s->find(key)) != nullptr) { return result; }

	// コントロールのクラスを検索
	atom_value *cls = static_cast<atom_value*>(s->find(style::class_name));
	if (cls == nullptr) { return nullptr; }

	atom_value::const_reverse_iterator it = cls->rbegin();
	while (it != cls->rend())
	{
		// クラスからスタイルを検索
		if ((s = find(*it++)) != nullptr)
		{
			// クラスのスタイルからkeyの値を検索
			if ((result = s->find(key)) != nullptr) { return result; }
		}
	}

	return nullptr;
}

int32_t style_service::get_atom(std::wstring name)
{
	return m_atom_service.get_atom(name);
}

style* style_service::insert(control const *c)
{
	style::store s = std::make_unique<style>();
	style *result = s.get();
	m_id_map[c] = std::move(s);

	return result;
}

style* style_service::insert(int32_t atom)
{
	style::store s = std::make_unique<style>();
	style *result = s.get();
	m_class_map[atom] = std::move(s);

	return result;
}

style* style_service::insert(std::wstring name)
{
	return insert(get_atom(name));
}








