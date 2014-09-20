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

style_value::style_value(int32_t key_, int32_t value_)
	: key(key_), int_value(value_)
{

}

bool style_value::operator <(style_value const &rhs) const
{
	return key < rhs.key;
}

bool style_value::operator ==(style_value const &rhs) const
{
	return key == rhs.key;
}

bool style_value::operator >(style_value const &rhs) const
{
	return key > rhs.key;
}

// style ----------------------------------------------------------------------

style::style()
{

}

style::store style::create()
{
	return style::store(new style());
}

style::const_iterator style::begin() const
{
	return m_storage.begin();
}

style::const_iterator style::end() const
{
	return m_storage.end();
}

style::const_range_type style::equal_range(int32_t key) const
{
	style_value sv(key, 0);
	return std::equal_range(begin(), end(), sv);
}

style::const_iterator style::find(int32_t key) const
{
	style_value sv(key, 0);
	return std::find(begin(), end(), sv);
}

void style::insert(int32_t key, int32_t value)
{
	style_value sv(key, value);
	m_storage.push_back(sv);
	std::sort(m_storage.begin(), m_storage.end());
}

// style_cache ----------------------------------------------------------------

style_cache::style_cache(style_service const &ss) : m_style_service(ss)
{

}

void style_cache::push_back(style const *s)
{
	m_storage.push_back(s);
}

bool style_cache::find(int32_t key, style_value &result) const
{
	for (style const* s : m_storage)
	{
		style::const_iterator it = s->find(key);
		if (it != s->end())
		{
			result = *it;
			return true;
		}
	}

	return false;
}

bool style_cache::find(int32_t key, rgb_color &result) const
{
	style_value sv(key, 0);
	if (find(key, sv) == false) { return false; }

	rgb_color rgb(static_cast<uint32_t>(sv.int_value));
	result = rgb;

	return true;
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

style const* style_service::find(std::type_index type) const
{
	class_map_type::const_iterator it = m_class_map.find(type);
	return (it == m_class_map.end()) ? nullptr : it->second.get();
}

style_cache style_service::find_styles(void const *p) const
{
	style_cache result(*this);

	// オブジェクトのスタイルを検索
	style const *s0 = find(p);
	if (s0 == nullptr) { return result; } // 空のキャッシュを返す
	result.push_back(s0);

	// オブジェクトに設定されているスタイル・クラスからスタイルを検索

	// まず、スタイル・クラス識別子を検索する
	style::const_range_type er = s0->equal_range(style::style_class);

	style::const_iterator
		it1 = er.first,
		it2 = er.second;

	// スタイル・クラス識別子が設定されていない場合終了
	if (it1 == it2) { return result; }

	while (it1 != it2)
	{
		int32_t atom = (it1++)->int_value;
		style const *s1 = find(atom);
		if (s1 != nullptr) { result.push_back(s1); }
	}

	return result;
}

style_cache style_service::find_styles(std::type_index ti, void const *p) const
{
	style_cache result = find_styles(p);

	style const *s = find(ti);
	if (s != nullptr) { result.push_back(s); }

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


