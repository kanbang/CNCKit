/**
 * @file    wordring/gui/window_control.h
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
#ifndef WORDRING_WINDOW_CONTROL_H
#define WORDRING_WINDOW_CONTROL_H

#include <wordring/debug.h>

#include <wordring/gui/control.h>
#include <wordring/gui/control_window.h>

namespace wordring
{
namespace gui
{

class dummy;

class window_control : public control, public control_window
{
public:
	virtual void set_parent(container* c);

protected:
	virtual void do_create();
	virtual void do_destroy();
	virtual void do_paint(canvas& cv);
	virtual void do_size(size_int size);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_CONTROL_H
