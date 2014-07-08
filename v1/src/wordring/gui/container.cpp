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

using namespace wordring::gui;

form::form() : m_window(new window)
{
}

void form::set_size(size_int size)
{
	m_window->set_size(size);

}

size_int form::get_size() const
{
	return m_window->get_size();
}

void form::set_position(point_int point)
{
	m_window->set_position(point);
}

point_int form::get_position() const
{
	return m_window->get_position();
}