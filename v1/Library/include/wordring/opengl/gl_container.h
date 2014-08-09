/**
 * @file    wordring/opengl/gl_container.h
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

#ifndef WORDRING_GL_CONTAINER_H
#define WORDRING_GL_CONTAINER_H

#include <wordring/debug.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/window_container_tmpl.h>

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

class dummy;

/**
 * @brief   OpenGLを描画できるコンテナです
 *
 * @details 
 *          コントロールではなくコンテナです。
 *
 *
 */
class gl_container : public window_container_tmpl <container, window>
{
public:
	typedef window_container_tmpl<container, window> base_type;
	typedef wordring::opengl::gl_context gl_context;

protected:
	gl_context m_context;

	// 構築・破棄 -------------------------------------------------------------
protected:
	gl_container();

public:
	virtual ~gl_container();

	/**
	 * @brief   コントロールを作成します
	 *
	 * @details
	 *          コントロールのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static control::store create();

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	// 一般メッセージ ---------------------------------------------------------

	/// 内部用: 再描画要求で呼び出されます
	virtual void do_paint_internal(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_GL_CONTAINER_H
