/**
 * @file    wordring/gui/control.h
 *
 * @brief   GUIコントロール
 *
 * @details
 *          ボタン、スクロールバーなどのGUIコントロールです。
 *          controlクラスはインタフェースとして宣言されます。
 *          ただし、共通部分は一部実装されます。
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

#ifndef WORDRING_CONTROL_H
#define WORDRING_CONTROL_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>
#include <wordring/gui/canvas.h>

#include <cstdint>
#include <functional>

namespace wordring
{
namespace gui
{

class container; // 前方宣言
class form; // 前方宣言
class window;

class control
{
protected:
	container* m_parent;

	size_int m_size; /// コントロールの幅と高さ
	point_int m_position; /// 親コンテナ原点からの相対位置

public:
	control();

	// 
	virtual bool is_window() const;

	virtual window* find_window();
	virtual form* find_form();

	// 
	virtual void set_parent(container* parent);
	virtual container* get_parent();
	virtual container const* get_parent() const;
	
	// 
	virtual void repaint();
	virtual void repaint(point_int pt, size_int size);
	
	/// コントロールの大きさを設定する
	virtual void set_size(size_int size);
	virtual size_int get_size() const;

	virtual void set_position(point_int point);
	virtual point_int get_position() const;

	virtual void set_rect(rect_int rc);
	virtual rect_int get_rect() const;

protected:
	virtual point_int query_position_from_form() const;
	virtual point_int query_position_from_window() const;

	// マウス・メッセージ -----------------------------------------------------

	virtual void do_mouse_move(point_int pt);
	virtual void do_mouse_over(point_int pt);
	virtual void do_mouse_out(point_int pt);
	//virtual bool do_click();

	// キーボード -------------------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_create();
	virtual void do_destroy();
	virtual void do_paint(canvas& cv);
	virtual void do_size(size_int size);

public:
	// イベント・ハンドラ -----------------------------------------------------

	std::function<void(point_int)> on_mouse_move;
	std::function<void(point_int)> on_mouse_out;
	std::function<void(point_int)> on_mouse_over;
	std::function<bool()> onMouseDown;
	std::function<bool()> onMouseUp;
	//std::function<void(point_int, int32_t)> on_click;
	std::function<bool()> onDblClick;

	std::function<bool()> onKeyPress;
	std::function<bool()> onKeyDown;
	std::function<bool()> onKeyUp;

	std::function<void()> on_create;
	std::function<void()> on_destroy;
	std::function<void(canvas&)> on_paint;
	std::function<void(size_int)> on_size;
};



} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_H
