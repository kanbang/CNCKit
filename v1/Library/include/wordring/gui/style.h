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

#include <memory>

#include <unordered_map>
#include <vector>

namespace wordring
{
namespace gui
{

class control;

struct property
{
	typedef std::unique_ptr<property> store;
};

class style
{
public:
	typedef std::unique_ptr<style> store;
	typedef std::unordered_map<int32_t, property::store> storage_type;
public:
	enum : int32_t
	{
		background_color = 1,
	};

protected:
	storage_type m_properties;
};

class style_service
{
public:
	typedef std::vector<style::store> storage_type;

	typedef std::unordered_map<control*, style*> id_map_type;
	typedef std::unordered_map<int32_t, style*> id_map_type;

private:
	storage_type m_styles;

public:

};

} // namespace gui
} // namespace wordring

#endif // WORDRING_STYLE_H
