/**
 * @file    wordring/detail/win32/exception.cpp
 *
 * @brief   例外実装ファイル
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

#include <wordring/wordring.h>

#ifdef WORDRING_OS_WIN

#include <wordring/exception.h>

#include <string>
#include <cassert>

#include <Windows.h>

using namespace wordring;

std::string exception_category::message(int ev) const
{
	char* buf;

	::FormatMessageA(
		  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
		, NULL
		, (DWORD)ev
		, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) // デフォルト ユーザー言語 
		, (LPSTR)&buf
		, 0
		, NULL);
	std::string result(buf);

	HLOCAL p = ::LocalFree((LPVOID)buf);
	assert(p == NULL);
	if (p != NULL) { ::exit((int)p); }

	return result;
}

exception_category exception_category::g_exception_category;

exception_category const& wordring::get_exception_category()
{
	return exception_category::g_exception_category;
}

void wordring::check_assertion(bool btrue)
{
	if (!btrue)
	{
		throw std::system_error(
			::GetLastError(), wordring::get_exception_category());
	}
}

#endif // WRODRING_OS_WIN
