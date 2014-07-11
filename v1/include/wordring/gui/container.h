﻿/**
 * @file    wordring/gui/container.h
 *
 * @brief   GUIコンテナ
 *
 * @details
 *          ボタンやスクロールバーなど、GUIのコントロールを載せるコンテナです。
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

#ifndef WORDRING_CONTAINER_H
#define WORDRING_CONTAINER_H

#include <wordring/gui/control.h>
#include <wordring/gui/window.h>

#include <vector>

namespace wordring
{
namespace gui
{

class container : public control
{
private:
	std::vector<control*> m_children;

public:
	//void add(control* child);
	//void remove(control* child);
};

class form : public container, public window
{
protected:
public:
	form(){}

	bool onCreate(){ return true; }

	/// 子コントロールから呼び出され、自身を返します
	virtual form* get_form();
	/// 関連付けられたウィンドウを返します
	virtual window* get_window();
	/// コントロールの大きさを設定する
	virtual void set_size(size_int size);
	/// コントロールの大きさを取得する
	virtual size_int get_size() const;

	virtual void set_position(point_int point);
	virtual point_int get_position() const;

	//virtual bool on_click();

	virtual void on_paint(canvas& cv)
	{
		if (onPaint) { onPaint(cv); }
	}
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_H
