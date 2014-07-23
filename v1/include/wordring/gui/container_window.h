/**
* @file    wordring/gui/container_window.h
*
* @brief
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

#ifndef WORDRING_CONTAINER_WINDOW_H
#define WORDRING_CONTAINER_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/window.h>

namespace wordring
{
namespace gui
{

/**
* @brief コンテナ・ウィンドウ
* @details
*    コンテナとして使う基底ウィンドウです。
*    ここから派生します。
*/
class container_window : public window
{
public:
	container_window();
	container_window(detail::native_window* nw);
	virtual ~container_window();
};

	


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_WINDOW_H
