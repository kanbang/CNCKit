// wordring/gui/detail/win32/win32_window_service.h

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウィンドウ・サービス

#ifndef WORDRING_WIN32_WINDOW_SERVICE_H
#define WORDRING_WIN32_WINDOW_SERVICE_H

#include <wordring/gui/detail/native_window_service.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class native_window_service_impl : public native_window_service
{
private:
public:
	virtual void run();
};







} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_SERVICE_H
