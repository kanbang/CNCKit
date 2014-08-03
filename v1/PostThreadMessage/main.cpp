#include <Windows.h>
#include <iostream>


int main()
{
	MSG msg;
	BOOL result;

	result = ::PostThreadMessage(GetCurrentThreadId(), WM_APP + 1, 0, 0);
	result = ::PostThreadMessage(GetCurrentThreadId(), WM_APP + 1, 0, 0);
	result = ::PostThreadMessage(GetCurrentThreadId(), WM_APP + 1, 0, 0);

	while ((result = ::GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (result == -1) { break; }

		result = ::PostThreadMessage(GetCurrentThreadId(), WM_APP + 1, 0, 0);

		if (msg.message == WM_APP + 1)
		{
			std::cout << "tick" << ::GetCurrentThreadId() << std::endl;
		}
	}
}
