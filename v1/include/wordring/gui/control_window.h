/**
 * @file    wordring/gui/control_window.h
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

#ifndef WORDRING_CONTROL_WINDOW_H
#define WORDRING_CONTROL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/window.h>

namespace wordring
{
namespace gui
{

/**
* @brief コントロール・ウィンドウ
* @details
*    コントロールとして使う基底ウィンドウです。
*    ここから派生します。
*/
class control_window : public window
{
public:
	control_window();
	control_window(detail::native_window* nw);
	virtual ~control_window();
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_WINDOW_H
