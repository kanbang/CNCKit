/**
* @file    wordring/gui/container.cpp
*
* @brief   GUIコンテナの実装ファイル
*
* @details
*          フォームなどのGUIコンテナです。
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

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>

#include <cassert>
#include <memory>


using namespace wordring::gui;

void container::add(store&& child)
{
	control* c = child.get();
	m_children.push_back(std::move(child));
	c->set_parent(this);
	c->repaint();
}



