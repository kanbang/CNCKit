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
#include <wordring/gui/window_container_tmpl.h>

#include <wordring/opengl/gl_service.h>

#include <atomic>
#include <cassert>

namespace wordring
{
namespace gui
{

class cnc_3axis_emulator_control : public window_control_tmpl <control, window>
{
public:
	typedef wordring::opengl::gl_service gl_service;

protected:
	gl_service &m_service;

protected:
	cnc_3axis_emulator_control(gl_service &sv);

public:
	virtual ~cnc_3axis_emulator_control();

	static control::store create(gl_service &sv);

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	virtual void do_paint(gui::canvas &cv);
	virtual void do_paint_internal(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CNC_3AXIS_EMULATOR_CONTROL_H
