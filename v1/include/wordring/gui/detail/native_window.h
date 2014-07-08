﻿/**
 * @file    wordring/gui/detail/native_window.h
 *
 * @brief   ウィンドウのpimplインターフェース
 *
 * @details
 *          このファイルには、pimplの公開されるインターフェースを宣言します。
 *          window関連クラスの実装に使われます。
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

#ifndef WORDRING_NATIVE_WINDOW_H
#define WORDRING_NATIVE_WINDOW_H

//#include <wordring/gui/window.h>
//#include <wordring/gui/window_service.h>
#include <wordring/geometry/shape.h>

namespace wordring
{
namespace gui
{

class window; // 前方宣言

namespace detail
{

// 
class native_window
{
private:
protected:
	wordring::gui::window* m_window;

public:
	native_window() { }

	void set_window(wordring::gui::window* w) { m_window = w; }
	wordring::gui::window* get_window() { return m_window; }

	virtual void create(wordring::gui::window* parent) = 0;
	virtual void close() = 0;
	virtual void destroy() = 0;

	virtual native_window* get_parent() = 0;
	virtual void set_parent(native_window* parent) = 0;

	virtual void set_size(size_int size) = 0;
	virtual size_int get_size() const = 0;

	virtual void set_position(point_int point) = 0;
	virtual point_int get_position() const = 0;


	//virtual void set_parent() = 0;
};


class native_control_window : public native_window
{

};

class native_container_window : public native_window
{

};

class native_button_window : public native_window
{

};




} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_WINDOW_H