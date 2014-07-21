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
};



} // namespace graphics
} // namespace wordring

#endif // WORDRING_COLOR_H
