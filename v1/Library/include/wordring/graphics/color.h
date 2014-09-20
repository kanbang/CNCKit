/**
 * @file    wordring/graphics/color.h
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

#ifndef WORDRING_COLOR_H
#define WORDRING_COLOR_H

#include <wordring/wordring.h>

namespace wordring
{
namespace graphics
{

struct rgb_color
{
	unsigned char r, g, b, a;

	rgb_color() : r(0), g(0), b(0), a(0) { }
	rgb_color(
		unsigned char r_,
		unsigned char g_,
		unsigned char b_,
		unsigned char a_ = 255) : r(r_), g(g_), b(b_), a(a_) { }
	rgb_color(uint32_t rgba)
		: r((rgba & 0xFF000000) >> 24)
		, g((rgba & 0xFF0000) >> 16)
		, b((rgba & 0xFF00) >> 8)
		, a(rgba & 0xFF) { }

	operator uint32_t() const
	{
		return (r << 24) + (g << 16) + (b << 8) + a;
	}
};

} // namespace graphics

namespace gui
{

typedef wordring::graphics::rgb_color rgb_color;

} // namespace gui

} // namespace wordring

#endif // WORDRING_COLOR_H
