// wordring/gui/window_service.h

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウィンドウ・サービスを提供

#ifndef WORDRING_WINDOW_SERVICE_H
#define WORDRING_WINDOW_SERVICE_H


#include <cstdint>
#include <memory>

#include <wordring/gui/detail/native_window_service.h>

namespace wordring
{
namespace gui
{

//class native_window_service;

class window_service
{
private:
	std::unique_ptr<detail::native_window_service> m_native_ptr;

public:
	int32_t m;

public:
	window_service();
	void run();
};









} // namespace gui
} // namespace wordring

#endif // WORDRING_WINDOW_SERVICE_H
