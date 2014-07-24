/**
 * @file    wordring/gui/container.h
 *
 * @brief   GUIコンテナ
 *
 * @details
 *          ボタンやスクロールバーなど、GUIのコントロールを載せるコンテナです。
 *          containerクラスはインターフェースとして宣言されます。
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

#include <wordring/debug.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container_window.h>

#include <vector>
#include <string>

namespace wordring
{
namespace gui
{

class container : public control
{
private:
	//std::vector<control*> m_children;

public:
	//void add(control* child);
	//void remove(control* child);
};

class form : public container, public container_window
{
protected:
public:
	form();
	virtual ~form();

	virtual void set_title(std::string title);
	virtual void set_title(std::wstring title);

	virtual void set_parent(container* c);

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

	virtual void do_create()
	{
		if (on_create) { on_create(); }
	}

	virtual void do_destroy()
	{
		if (on_destroy) { on_destroy(); }
	}

	virtual void do_paint(canvas& cv)
	{
		if (on_paint) { on_paint(cv); }
	}
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_H
