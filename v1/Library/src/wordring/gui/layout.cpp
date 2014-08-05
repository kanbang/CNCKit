/**
 * @file    wordring/gui/layout.cpp
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

#include <wordring/gui/layout.h>

#include <wordring/gui/container.h>

#include <algorithm>

using namespace wordring::gui;

// layout ---------------------------------------------------------------------

layout::layout()
{

}

layout::~layout()
{

}

void layout::perform_layout(container* c)
{
}

layout::store layout::create()
{
	return layout::store(new layout);
}

// full_layout ----------------------------------------------------------------

full_layout::full_layout()
{

}

full_layout::~full_layout()
{

}

void full_layout::perform_layout(container* c)
{
	size_int size = c->get_size();
	container::storage_type& children = c->get_children();

	for (container::store& store : children)
	{
		control* child = store.get();
		child->set_rect(rect_int(point_int(0, 0), size));
	}
}

layout::store full_layout::create()
{
	return layout::store(new full_layout);
}

// flow_layout ----------------------------------------------------------------

flow_layout::flow_layout()
{

}

flow_layout::~flow_layout()
{

}

void flow_layout::perform_layout(container* c)
{
	int32_t
		cx = c->get_size().cx,
		cy = c->get_size().cy;

	int32_t x = 0, dx = 0, y = 0, dy = 0;

	container::storage_type& children = c->get_children();
	for (container::store& store : children)
	{
		size_int size = store->get_size();

		x += dx;
		dx = size.cx;

		if (cx < x + dx) // y送り
		{
			x = 0;
			y += dy;
			dy = 0;
		}

		dy = std::max(dy, size.cy);
		
		store->set_rect(rect_int(point_int(x, y), size));
	}
}

layout::store flow_layout::create()
{
	return layout::store(new flow_layout);
}




