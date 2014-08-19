/**
 * @file    wordring/gui/shape_int.h
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

#ifndef WORDRING_SHAPE_INT_H
#define WORDRING_SHAPE_INT_H

#include <cstdint>
#include <algorithm>

namespace wordring
{
namespace gui
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

	void operator -=(point_int rhs) { x -= rhs.x, y -= rhs.y; }

	point_int operator +(point_int rhs)
	{
		return point_int(x + rhs.x, y + rhs.y);
	}

	point_int operator -(point_int rhs)
	{
		return point_int(x - rhs.x, y - rhs.y);
	}
};


/**
* @brief 形状を表す基本クラス
*/
struct shape_int
{

};

struct linesegment_int : shape_int
{

};

struct rect_int : shape_int
{
	point_int pt;
	size_int size;

	rect_int() { }
	rect_int(point_int pt_, size_int size_) : pt(pt_), size(size_) { }

	rect_int(int32_t x, int32_t y, int32_t cx, int32_t cy)
		: pt(x, y), size(cx, cy)
	{
	}

	int32_t top() const
	{
		return pt.y;
	}

	int32_t bottom() const
	{
		return pt.y + size.cy - 1;
	}

	int32_t left() const
	{
		return pt.x;
	}

	int32_t right() const
	{
		return pt.x + size.cx - 1;
	}

	bool operator ==(rect_int const &rhs) const
	{
		return pt == rhs.pt && size == rhs.size;
	}

	void operator +=(point_int pt_)
	{
		pt.x += pt_.x;
		pt.y += pt_.y;
	}

	rect_int operator +(rect_int const& rhs) const
	{
		int32_t x1, y1, x2, y2;
		x1 = std::min(pt.x, rhs.pt.x);
		y1 = std::min(pt.y, rhs.pt.y);
		x2 = std::max(pt.x + size.cx, rhs.pt.x + rhs.size.cx);
		y2 = std::max(pt.y + size.cy, rhs.pt.y + rhs.size.cy);

		return rect_int(point_int(x1, y1), size_int(x2 - x1, y2 - y1));
	}

	/// 重なる部分を返します
	rect_int operator &(rect_int rhs) const { return intersects(rhs); }

	bool including(point_int pt_)
	{
		return left() <= pt_.x && pt_.x <= right()
			&& top() <= pt_.y && pt_.y <= bottom();
	}

	/// 重なる部分を返します
	rect_int intersects(rect_int rhs) const
	{
		point_int pt1, pt2;
		/*
		pt1.x = std::max(pt.x, rhs.pt.x);
		pt1.y = std::max(pt.y, rhs.pt.y);
		pt2.x = std::min(pt.x + size.cx, rhs.pt.x + rhs.size.cx);
		pt2.y = std::min(pt.y + size.cy, rhs.pt.y + rhs.size.cy);
		*/

		pt1.x = std::max(left(), rhs.left());
		pt1.y = std::max(top(), rhs.top());
		pt2.x = std::min(right(), rhs.right());
		pt2.y = std::min(bottom(), rhs.bottom());

		int32_t
			cx = pt2.x - pt1.x + 1,
			cy = pt2.y - pt1.y + 1;

		return rect_int(
			pt1, (0 < cx && 0 < cy) ? size_int(cx, cy) : size_int());
	}
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_SHAPE_INT_H
