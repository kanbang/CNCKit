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

style::style(type tp, state st) : m_type(tp), m_state(st)
{

}

// control_style --------------------------------------------------------------

control_style::control_style(style::state st)
	: style            (style::type::control, st)
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
	
	, color            (color_rgb(0xFF, 0xFF, 0xFF, 0xFF))
	, foreground_color (color_rgb(0x80, 0x80, 0x80, 0xFF))
	, background_color (color_rgb(0x20, 0x20, 0x20, 0xFF))
	, border_color     (color_rgb(0x80, 0x80, 0x80, 0xFF))

	, line_height      (16)
{

}

style::store control_style::create(style::state st)
{
	return style::store(new control_style(st));
}

// text_style -----------------------------------------------------------------




