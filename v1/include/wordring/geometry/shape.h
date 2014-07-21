/**
 * @file    wordring/geometry/shape.h
 *
 * @brief   形状を表すクラス群
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

#ifndef WORDRING_SHAPE_H
#define WORDRING_SHAPE_H

#include <wordring/debug.h>

#include <cstdint>

namespace wordring
{
namespace geometry
{

struct size_int
{
	int32_t w, h;

	size_int(int32_t width, int32_t height) : w(width), h(height) { }
};

struct point_int
{
	int32_t x, y;

	point_int(int32_t x_, int32_t y_) : x(x_), y(y_) { }
};


/**
 * @brief 形状を表す基本クラス
 */
struct shape
{

};

struct linesegment_int : shape
{
	
};


} // namespace geometry

namespace gui
{

typedef wordring::geometry::size_int size_int;
typedef wordring::geometry::point_int point_int;

typedef wordring::geometry::linesegment_int line_int;

} // namespace gui

} // namespace wordring

#endif // WORDRING_SHAPE_H
