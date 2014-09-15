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

// style_value ----------------------------------------------------------------

style_value::style_value(int32_t key_) : key(key_)
{

}

// color_value ----------------------------------------------------------------

color_value::color_value(int32_t key_, rgb_color value_)
	: style_value(key_), value(value_)
{

}

style_value::store color_value::create(int32_t key_, rgb_color value_)
{
	return style_value::store(new color_value(key_, value_));
}

// int32_value ----------------------------------------------------------------

// style_class_value ----------------------------------------------------------

style_class_value::style_class_value() : style_value(style::style_class)
{

}

style_value::store style_class_value::create()
{
	return style_value::store(new style_class_value());
}

// style ----------------------------------------------------------------------

style::style()
{

}

style::store style::create()
{
	return style::store(new style());
}

style_value* style::find(int32_t key)
{
	storage_type::iterator it = std::find_if(
		m_storage.begin(),
		m_storage.end(),
		[=](style_value::store &s)->bool{ return s->key == key; });

	return (it == m_storage.end()) ? nullptr : it->get();
}

style_value const* style::find(int32_t key) const
{
	style *self = const_cast<style*>(this);
	return self->find(key);
}

void style::insert(style_value::store s)
{
	m_storage.push_back(std::move(s));
	std::sort(m_storage.begin(), m_storage.end()
		, [](style_value::store &s1, style_value::store &s2)->bool {
			return s1->key < s2->key; });
}

void style::insert(int32_t key, int32_t val)
{

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


void style_service::erase(void const *c)
{
	m_object_map.erase(c);
}

void style_service::erase(int32_t atom)
{
	m_atom_map.erase(atom);
}

void style_service::erase(std::wstring name)
{
	int32_t atom = m_atom_service.get_atom(name);
	erase(atom);
}

void style_service::erase(std::type_index type)
{
	m_class_map.erase(type);
}

style* style_service::find(void const *p)
{
	object_map_type::iterator it = m_object_map.find(p);
	return (it == m_object_map.end()) ? nullptr : it->second.get();
}

style const* style_service::find(void const *p) const
{
	style_service *self = const_cast<style_service*>(this);
	return self->find(p);
}

style* style_service::find(int32_t atom)
{
	atom_map_type::iterator it = m_atom_map.find(atom);
	return (it == m_atom_map.end()) ? nullptr : it->second.get();
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
	class_map_type::iterator it = m_class_map.find(type);
	return (it == m_class_map.end()) ? nullptr : it->second.get();
}

style_cache style_service::find_styles(void const *p) const
{
	style_cache result;

	// オブジェクトのスタイルを検索
	style const *s0 = find(p);
	if (s0 == nullptr) { return result; } // 空のキャッシュを返す
	result.push_back(s0);

	// オブジェクトに設定されているスタイル・クラスからスタイルを検索

	// まず、スタイル・クラス識別子を検索する
	style_class_value const* sc =
		static_cast<style_class_value const*>(s0->find(style::style_class));

	std::vector<int32_t>::const_iterator
		it1 = sc->value.begin(),
		it2 = sc->value.end();

	// スタイル・クラス識別子が設定されていない場合終了
	if (it1 == it2) { return result; }

	while (it1 != it2)
	{
		int32_t atom = *it1++;
		style const *s1 = find(atom);
		if (s1 != nullptr) { result.push_back(s1); }
	}

	return result;
}

int32_t style_service::get_atom(std::wstring name)
{
	return m_atom_service.get_atom(name);
}

style* style_service::insert(void const *p)
{
	style::store s = style::create();
	style * result = s.get();
	m_object_map[p] = std::move(s);

	return result;
}

style* style_service::insert(int32_t atom)
{
	style::store s = style::create();
	style * result = s.get();
	m_atom_map[atom] = std::move(s);

	return result;
}

style* style_service::insert(std::wstring name)
{
	return insert(get_atom(name));
}

style* style_service::insert(std::type_index type)
{
	style::store s = style::create();
	style * result = s.get();
	m_class_map[type] = std::move(s);

	return result;
}


