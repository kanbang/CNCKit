/**
 * @file    cnc_3axis_emulator_control.h
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

#ifndef WORDRING_CNC_3AXIS_EMULATOR_CONTROL_H
#define WORDRING_CNC_3AXIS_EMULATOR_CONTROL_H

#include <wordring/debug.h>

#include <wordring/gui/control.h>
//#include <wordring/gui/window_control_tmpl.h>
#include <wordring/gui/window_control.h>

#include <wordring/opengl/gl_context.h>

#include <atomic>
#include <cassert>

#include <GL/glew.h>
#include <gl/GL.h>
#include <GL/GLU.h>

namespace wordring
{
namespace gui
{

class cnc_3axis_emulator_control : public window_control
{
public:
	typedef wordring::opengl::gl_context gl_context;

protected:
	gl_context &m_service;

protected:
	cnc_3axis_emulator_control(rect_int rc, gl_context &sv);

public:
	virtual ~cnc_3axis_emulator_control();

	static control::store create(rect_int rc, gl_context &sv);

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	virtual void do_paint(gui::canvas &cv);
	virtual void do_paint_internal(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CNC_3AXIS_EMULATOR_CONTROL_H
