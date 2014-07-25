/**
 * @file    wordring/gui/window_control.cpp
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

#include <wordring/gui/window_control.h>

using namespace wordring::gui;


void window_control::do_create()
{
	if (on_create) { on_create(); }
}

void window_control::do_destroy()
{
	if (on_destroy) { on_destroy(); }
}

void window_control::do_paint(canvas& cv)
{
	if (on_paint) { on_paint(cv); }
}

void window_control::do_size(size_int size)
{
	if (on_size) { on_size(size); }
}




