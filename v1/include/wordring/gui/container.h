/**
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

class form : public container
{
protected:
	std::unique_ptr<window> m_window; /// 内部でウィンドウを使用します

public:
	form();

	virtual void set_size(size_int size); /// コントロールの大きさを設定する
	virtual size_int get_size() const; /// コントロールの大きさを取得する

	virtual void set_position(point_int point);
	virtual point_int get_position() const;
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_H
