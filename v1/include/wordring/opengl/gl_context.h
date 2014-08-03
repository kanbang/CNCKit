/**
 * @file    wordring/opengl/gl_context.h
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

#ifndef WORDRING_GL_CONTEXT_H
#define WORDRING_GL_CONTEXT_H

#include <wordring/gui/canvas.h>

#include <wordring/gui/window.h>
#include <wordring/opengl/detail/native_gl_context.h>

#include <memory>

namespace wordring
{
namespace opengl
{

class dummy;

class gl_context
{
public:
	enum flag
	{
		WINDOW, MEMORY,
	};

protected:
	std::unique_ptr<detail::native_gl_context> m_native;

public:
	gl_context();
	gl_context(wordring::gui::window& w, int flg, int depth, int bpp);
	gl_context(wordring::gui::canvas& cv, int flg, int depth, int bpp);
	virtual ~gl_context();

	/**
	 * ウィンドウと互換性を持つコンテキストを作成します
	 * 
	 * 引数に出来るだけ近いコンテキストが作成されます。
	 * 
	 * @param w ウィンドウ
	 * @param flg 描画対象を表すフラグ（WINDOWあるいはMEMORY）
	 * @param depth zバッファの深度（16、24、32等）
	 * @param bpp ピクセルあたりのビット数（24、32等）
	 */
	virtual void assign(wordring::gui::window& w, int flg, int depth, int bpp);

	/**
	 * キャンバスと互換性を持つコンテキストを作成します
	 *
	 * 引数に出来るだけ近いコンテキストが作成されます。
	 *
	 * @param cv キャンバス
	 * @param flg 描画対象を表すフラグ（WINDOWあるいはMEMORY）
	 * @param depth zバッファの深度（16、24、32等）
	 * @param bpp ピクセルあたりのビット数（24、32等）
	 */
	virtual void assign(wordring::gui::canvas& cv, int flg, int depth, int bpp);

	/**
	 * スレッドのカレント・コンテキストに選択します
	 * 
	 * カレント・コンテキストに選択されている間、OpenGLの描画はこのコンテキスト
	 * の設定されたキャンバスに対して行われます。
	 * 
	 * @param cv 描画対象となるcanvas
	 */
	virtual void make_current(wordring::gui::canvas& cv);
	/**
	 * スレッドのカレント・コンテキストから外します
	 */
	virtual void unmake_current(wordring::gui::canvas& cv);
};

} // namespace opengl
} // namespace wordring

#endif // WORDRING_GL_CONTEXT_H
