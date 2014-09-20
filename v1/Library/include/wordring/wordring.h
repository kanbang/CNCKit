/**
 * @file    wordring/wordring.h
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

#ifndef WORDRING_WORDRING_H
#define WORDRING_WORDRING_H

// OS -------------------------------------------------------------------------

#ifndef WORDRING_OS
#define WORDRING_OS

#ifdef _WIN32

#define WORDRING_OS_WIN

#ifdef _WIN64
#define WORDRING_OS_WIN64
#else // _WIN64
#define WORDRING_OS_WIN32
#endif // _WIN64

#endif // _WIN32

#ifdef __linux__

#define WORDRING_OS_LINUX

#ifdef __x86_64__
#define WORDRING_OS_LINUX_64
#else // __x86_64__
#define WORDRING_OS_LINUX_32
#endif // __x86_64__

#endif // __linux__

#endif // WORDRING_OS

// WS -------------------------------------------------------------------------

#ifndef WORDRING_WS
#define WORDRING_WS

#ifdef WORDRING_OS_WIN
#define WORDRING_WS_WIN
#endif // WORDRING_OS_WIN

#ifdef XlibSpecificationRelease
#define WORDRING_WS_X11
#endif // XlibSpecificationRelease 

#endif // WORDRING_WS

// Compiler -------------------------------------------------------------------

#ifdef _MSC_VER
#define WORDRING_TLS __declspec(thread)

#else
#ifdef __GNUC__
#define WORDRING_TLS __thread

#endif // __GNUC__

#endif // _MSC_VER





namespace wordring
{
} // namespace wordring


#include <wordring/debug.h>

#endif // WORDRING_WORDRING_H
