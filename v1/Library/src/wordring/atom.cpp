/**
 * @file    wordring/atom.cpp
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

#include <wordring/wordring.h>

#include <wordring/atom.h>

#include <cstdint>
#include <string>
#include <cassert>

using namespace wordring;

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
