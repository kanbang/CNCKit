/**
 * @file    wordring/gui/control_data.cpp
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

#include <wordring/gui/control_data.h>

#include <memory>

using namespace wordring::gui;

// control_data ---------------------------------------------------------------

control_data::control_data(int32_t type) : m_type(type)
{

}

bool control_data::is_type_of(int32_t type)
{
	return m_type == type;
}

// control_data_service -------------------------------------------------------

void control_data_service::erase(control const *c)
{
	m_object_map.erase(c);
}

void control_data_service::erase(int32_t atom)
{
	m_atom_map.erase(atom);
}

void control_data_service::erase(std::wstring name)
{
	int32_t atom = m_atom_service.get_atom(name);
	erase(atom);
}

void control_data_service::erase(std::type_index type)
{
	m_class_map.erase(type);
}

control_data_service::object_range_type
control_data_service::equal_range(control const *c)
{
	return m_object_map.equal_range(c);
}

control_data_service::atom_range_type
control_data_service::equal_range(int32_t atom)
{
	return m_atom_map.equal_range(atom);
}

control_data_service::atom_range_type
control_data_service::equal_range(std::wstring name)
{
	return equal_range(m_atom_service.get_atom(name));
}

control_data_service::class_range_type
control_data_service::equal_range(std::type_index type)
{
	return m_class_map.equal_range(type);
}

control_data* control_data_service::find(control const *c, int32_t type)
{
	object_range_type range = equal_range(c);
	object_iterator it = range.first;
	while (it != range.second)
	{
		control_data *cd = it->second.get();
		if (cd->is_type_of(type)) { return cd; }
		++it;
	}
	return nullptr;
}

control_data const*
control_data_service::find(control const *c, int32_t type) const
{
	control_data_service *self = const_cast<control_data_service*>(this);
	return self->find(c, type);
}

control_data* control_data_service::find(int32_t atom)
{
	atom_map_type::iterator it = m_atom_map.find(atom);
	if (it == m_atom_map.end()) { return nullptr; }

	return it->second.get();
}

control_data const* control_data_service::find(int32_t atom) const
{
	control_data_service *self = const_cast<control_data_service*>(this);
	return self->find(atom);
}

control_data* control_data_service::find(std::wstring name)
{
	int32_t atom = m_atom_service.get_atom(name);

	return find(atom);
}

control_data* control_data_service::find(std::type_index type)
{
	class_map_type::iterator it = m_class_map.find(type);
	if (it == m_class_map.end()) { return nullptr; }

	return it->second.get();
}

int32_t control_data_service::get_atom(std::wstring name)
{
	return m_atom_service.get_atom(name);
}

control_data* control_data_service::insert(
	control const *c, control_data::store s)
{
	control_data *result = s.get();
	m_object_map.insert(
		std::pair<control const*, control_data::store>(c, std::move(s)));

	return result;
}

control_data* control_data_service::insert(
	int32_t atom, control_data::store s)
{
	control_data *result = s.get();
	m_atom_map.insert(std::make_pair(atom, std::move(s)));

	return result;
}

control_data* control_data_service::insert(
	std::wstring name, control_data::store s)
{
	return insert(get_atom(name), std::move(s));
}

control_data* control_data_service::insert(
	std::type_index type, control_data::store s)
{
	control_data *result = s.get();

	m_class_map.insert(std::make_pair(type, std::move(s)));

	return result;
}

