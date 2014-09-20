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

#ifdef _WIN32 // Windows ------------------------------------------------------

#define WORDRING_OS_WIN

#ifdef _WIN64 // Windows 64bit ------------------------------------------------

#define WORDRING_OS_WIN64

#else // Windows 32bit --------------------------------------------------------

#define WORDRING_OS_WIN32

#endif // _WIN64

#else
#ifdef __linux__ // Linux -----------------------------------------------------

#define WORDRING_OS_LINUX

#ifdef __x86_64__ // Linux 64bit ----------------------------------------------

#define WORDRING_OS_LINUX_64

#else // __x86_64__ // Linux 32bit --------------------------------------------

#define WORDRING_OS_LINUX_32

#endif // __x86_64__
#endif // __linux__
#endif // _WIN32

#endif // WORDRING_OS

// ウィンドウ・システム -------------------------------------------------------

#ifndef WORDRING_WS
#define WORDRING_WS

#ifdef WORDRING_OS_WIN
#define WORDRING_WS_WIN
#endif // WORDRING_OS_WIN

#ifdef XlibSpecificationRelease
#define WORDRING_WS_X11
#endif // XlibSpecificationRelease 

#endif // WORDRING_WS

// コンパイラ -----------------------------------------------------------------

#ifndef WORDRING_CC
#define WORDRING_CC

#ifdef _MSC_VER // VC ---------------------------------------------------------

#define WORDRING_CC_VC

#else
#ifdef __GNUC__ // gcc --------------------------------------------------------

#define WORDRING_CC_GCC

#endif // __GNUC__
#endif // _MSC_VER

#endif // WORDRING_CC

// TLS ------------------------------------------------------------------------

#ifndef WORDRING_TLS

#ifdef WORDRING_CC_VC // VC ---------------------------------------------------

#define WORDRING_TLS __declspec(thread)

#else
#ifdef WORDRING_CC_GCC // gcc -------------------------------------------------

#define WORDRING_TLS __thread

#endif // WORDRING_CC_GCC
#endif // WORDRING_CC_VC

#endif // WORDRING_TLS

// その他 ---------------------------------------------------------------------

#ifdef WORDRING_OS_WIN // Windows ---------------------------------------------

#define NOMINMAX // min maxマクロを抑制する

#ifdef _DEBUG 

#define WORDRING_D_DEBUG

#endif // _DEBUG

#else
#ifdef WORDRING_OS_LINUX // Linux ---------------------------------------------


#endif // WORDRING_OS_LINUX
#endif // WORDRING_OS_WIN -----------------------------------------------------

#include <wordring/debug.h>

#endif // WORDRING_WORDRING_H
