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

#include <string>

using namespace wordring::gui;

form::form()
{
	create(nullptr);
}

form::~form()
{
}

form* form::get_form()
{
	return this;
}

window* form::get_window()
{
	//todo
	return static_cast<window*>(this);
}

void form::set_title(std::string title)
{
	m_native_window->set_text(title);
}

void form::set_title(std::wstring title)
{
	m_native_window->set_text(title);
}

void form::set_size(size_int size)
{
	window::set_size(size);
}

size_int form::get_size() const
{
	return window::get_size();
}

void form::set_position(point_int point)
{
	window::set_position(point);
}

point_int form::get_position() const
{
	return window::get_position();
}

/*
bool form::on_click()
{
	return true;
}
*/