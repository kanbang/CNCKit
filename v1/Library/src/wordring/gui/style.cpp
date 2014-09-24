/**
 * @file    wordring/gui/style.cpp
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

#include <wordring/gui/control.h>
#include <wordring/gui/style.h>
#include <wordring/gui/window_service.h>

#include <string>
#include <algorithm>
#include <cassert>

using namespace wordring::gui;

// style ----------------------------------------------------------------------

// control_style --------------------------------------------------------------

layout_style::layout_style()
	: style            (style::type::layout, style::state::normal)
	, min_width        (0)
	, max_width        (0)
	, min_height       (0)
	, max_height       (0)

	, margin_left      (0)
	, margin_right     (0)
	, margin_top       (0)
	, margin_bottom    (0)

	, padding_left     (0)
	, padding_right    (0)
	, padding_top      (0)
	, padding_bottom   (0)

	, border_width     (0)
	, border_style     (0)
{

}

style::store layout_style::create()
{
	return style::store(new layout_style());
}

// text_style -----------------------------------------------------------------




