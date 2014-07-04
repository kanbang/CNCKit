// wordring/gui/detail/native_window_service.h

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウィンドウ・サービス

#ifndef WORDRING_NATIVE_WINDOW_SERVICE_H
#define WORDRING_NATIVE_WINDOW_SERVICE_H

#include <cstdint>

namespace wordring
{
namespace gui
{
namespace detail
{

class native_window_service
{
public:
	virtual void run() = 0;
};





} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_WINDOW_SERVICE_H
