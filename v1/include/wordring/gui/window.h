// wordring/gui/window.h

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウィンドウ

#ifndef WORDRING_WINDOW_H
#define WORDRING_WINDOW_H

#include <memory>

#include <wordring/gui/detail/native_window.h>

namespace wordring
{
namespace gui
{

// 
class window
{
private:
	std::unique_ptr<detail::native_window> m_native_ptr;

public:
	window();
};






} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_H
