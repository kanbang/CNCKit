/**
 * @file    wordring/gui/control.h
 *
 * @brief   GUIコントロール
 *
 * @details
 *          ボタン、スクロールバーなどのGUIコントロールです。
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
#ifndef WORDRING_CONTROL_H
#define WORDRING_CONTROL_H

#include <wordring/geometry/shape.h>
//#include <wordring/gui/window_service.h>

#include <cstdint>
#include <functional>

namespace wordring
{
namespace gui
{


class container; // 前方宣言
class form; // 前方宣言

class control
{
private:
	container* m_parent;

	//size_int m_size; /// コントロールの幅と高さ
	//point_int m_position; /// 親コンテナ原点との相対位置

public:
	/// イベント・ハンドラ
	std::function<bool()> onCreate;
	/// イベント・ハンドラ
	std::function<bool()> onDestroy;
	/// イベント・ハンドラ
	std::function<bool()> onClick;
	/// イベント・ハンドラ
	std::function<bool()> onDblClick;
	/// イベント・ハンドラ
	std::function<bool()> onMouseMove;
	/// イベント・ハンドラ
	std::function<bool()> onMouseOut;
	/// イベント・ハンドラ
	std::function<bool()> onMouseOver;
	/// イベント・ハンドラ
	std::function<bool()> onMouseDown;
	/// イベント・ハンドラ
	std::function<bool()> onMouseUp;
	/// イベント・ハンドラ
	std::function<bool()> onKeyPress;
	/// イベント・ハンドラ
	std::function<bool()> onKeyDown;
	/// イベント・ハンドラ
	std::function<bool()> onKeyUp;

public:
	control();
	//virtual void set_parent(container* parent);
	virtual container* get_parent();
	virtual form* get_form();
	
	//virtual window_service* get_service() const;

	/**
	 * @brief コントロールの大きさを設定する
	 *
	 * 
	 * 
	 */
	virtual void set_size(size_int size) = 0;
	virtual size_int get_size() const = 0;

	virtual void set_position(point_int point) = 0;
	virtual point_int get_position() const = 0;

	virtual bool on_click()
	{
		if (onClick) { return onClick(); }
		return false;
	}
};



} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_H
