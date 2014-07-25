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

#include <algorithm>
#include <cstdint>

namespace wordring
{
namespace geometry
{

struct size_int
{
	int32_t cx, cy;

	size_int() : cx(0), cy(0) { }
	size_int(int32_t width, int32_t height) : cx(width), cy(height) { }

	bool operator ==(size_int const& rhs) const
	{
		return cx == rhs.cx && cy == rhs.cy;
	}
};

struct point_int
{
	int32_t x, y;

	point_int() : x(0), y(0) { }
	point_int(int32_t x_, int32_t y_) : x(x_), y(y_) { }

	bool operator ==(point_int const& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	void operator +=(point_int rhs) { x += rhs.x; y += rhs.y; }
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

struct rect_int : shape
{
	point_int pt;
	size_int size;

	rect_int() { }
	rect_int(point_int pt_, size_int size_) : pt(pt_), size(size_) { }

	rect_int operator +(rect_int const& rhs) const
	{
		int32_t x1, y1, x2, y2;
		x1 = std::min(pt.x, rhs.pt.x);
		y1 = std::min(pt.y, rhs.pt.y);
		x2 = std::max(pt.x + size.cx, rhs.pt.x + rhs.size.cx);
		y2 = std::max(pt.y + size.cy, rhs.pt.y + rhs.size.cy);

		return rect_int(point_int(x1, y1), size_int(x2 - x1, y2 - y1));
	}
};

} // namespace geometry

namespace gui
{

typedef wordring::geometry::size_int size_int;
typedef wordring::geometry::point_int point_int;

typedef wordring::geometry::linesegment_int line_int;
typedef wordring::geometry::rect_int rect_int;

} // namespace gui

} // namespace wordring

#endif // WORDRING_SHAPE_H
