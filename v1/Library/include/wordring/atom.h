/**
 * @file    wordring/atom.h
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

#ifndef WORDRING_ATOM_H
#define WORDRING_ATOM_H

#include <wordring/wordring.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace wordring
{

class dummy;

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


} // namespace wordring

#endif // WORDRING_ATOM_H
