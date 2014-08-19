/**
 * @file    wordring/gui/window_container_tmpl.h
 *
 * @brief   コンテナ用のウィンドウ実装
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

#ifndef WORDRING_WINDOW_CONTAINER_TMPL_H
#define WORDRING_WINDOW_CONTAINER_TMPL_H

#include <wordring/debug.h>

#include <wordring/gui/shape_int.h>

#include <wordring/gui/window_control_tmpl.h>

#include <wordring/gui/window.h>
#include <wordring/gui/detail/native_window.h>

#include <wordring/gui/layout.h>

namespace wordring
{
namespace gui
{
	
class dummy;

/**
 * @brief   ウィンドウを持つコンテナを作成するためのテンプレート・クラス
 *
 * @details ウィンドウの特性とコンテナの特性を合成できます。
 *          ウィンドウを持つすべてのコンテナは、このクラスを継承するべきです。
 *          コントロールの場合、window_control_tmplを継承してください。
 *          window_container_tmplは、window_control_tmplを継承しています。
 *
 * @param   T1 基本コンテナ
 * @param   T2 基本ウィンドウ
 */
template <typename T1, typename T2>
class window_container_tmpl : public window_control_tmpl<T1, T2>
{
public:
	typedef window_control_tmpl<T1, T2> base_type;
	typedef typename T1 container_type;
	typedef typename T2 window_type;

	// 構築・破棄 -------------------------------------------------------------
protected:
	window_container_tmpl(
		rect_int rc, layout::store l, detail::native_window::store nw)
			: base_type(rc, std::move(l), std::move(nw))
	{
	}

public:
	virtual ~window_container_tmpl()
	{
	}

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const
	{
		return "window_container_tmpl<T1, T2>";
	}
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_CONTAINER_TMPL_H
