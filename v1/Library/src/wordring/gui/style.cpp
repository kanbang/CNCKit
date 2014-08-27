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

// color_value ----------------------------------------------------------------

color_value::operator rgb_color()
{
	return val;
}

// style ----------------------------------------------------------------------

style_value* style::find(int32_t key)
{
	storage_type::iterator it = std::find_if(
		m_storage.begin(),
		m_storage.end(),
		[=](value_type &v)->bool{ return v.first == key; });

	return (it == m_storage.end()) ? nullptr : it->second.get();
}

style_value const* style::find(int32_t key) const
{
	style *self = const_cast<style*>(this);
	return self->find(key);
}

// style_cache ----------------------------------------------------------------

void style_cache::push_back(style const *s)
{
	m_storage.push_back(s);
}

style_value const* style_cache::find(int32_t key) const
{
	style_value const *result = nullptr;

	for (style const* s : m_storage)
	{
		result = s->find(key);
		if (result != nullptr) { break; }
	}

	return result;
}

// style_service --------------------------------------------------------------

void style_service::erase(control const *c)
{
	m_object_map.erase(c);
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

void style_service::erase(std::type_index type)
{
	m_default_map.erase(type);
}

style* style_service::find(control const *c)
{
	object_map_type::iterator it = m_object_map.find(c);
	if (it == m_object_map.end()) { return nullptr; }

	return it->second.get();
}

style const* style_service::find(control const *c) const
{
	style_service *self = const_cast<style_service*>(this);
	return self->find(c);
}

style* style_service::find(int32_t atom)
{
	class_map_type::iterator it = m_class_map.find(atom);
	if (it == m_class_map.end()) { return nullptr; }

	return it->second.get();
}

style const* style_service::find(int32_t atom) const
{
	style_service *self = const_cast<style_service*>(this);
	return self->find(atom);
}

style* style_service::find(std::wstring name)
{
	int32_t atom = m_atom_service.get_atom(name);

	return find(atom);
}

style* style_service::find(std::type_index type)
{
	default_map_type::iterator it = m_default_map.find(type);
	if (it == m_default_map.end()) { return nullptr; }

	return it->second.get();
}

style_cache style_service::find_styles(control const *c) const
{
	style_cache result;

	// オブジェクトのスタイルを検索
	style const *s0 = find(c);
	if (s0 == nullptr) { return result; } // 空のキャッシュを返す
	result.push_back(s0);

	// オブジェクトに設定されているスタイル・クラスからスタイルを検索

	// まず、スタイル・クラス識別子を検索する
	class_atom_value const *cls =
		static_cast<class_atom_value const*>(result.find(style::class_atom));
	if (cls == nullptr) { return result; } // クラス識別子が設定されていない場合終了

	// スタイル・クラスは後に登録された方が優先なので逆から検索していく
	class_atom_value::const_reverse_iterator it = cls->rbegin();
	while (it != cls->rend())
	{
		int32_t atom = *it;
		style const *s1 = find(atom);
		if (s1 != nullptr) { result.push_back(s1); }
	}

	return result;
}

int32_t style_service::get_atom(std::wstring name)
{
	return m_atom_service.get_atom(name);
}

style* style_service::insert(control const *c)
{
	style::store s = std::make_unique<style>();
	style *result = s.get();
	m_object_map[c] = std::move(s);

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

style* style_service::insert(std::type_index type)
{
	style::store s = std::make_unique<style>();
	style *result = s.get();
	m_default_map[type] = std::move(s);

	return result;
}






