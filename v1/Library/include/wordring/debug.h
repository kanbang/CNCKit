/**
 * @file    wordring/debug.h
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

#ifndef WORDRING_DEBUG_H
#define WORDRING_DEBUG_H

#ifdef WORDRING_D_DEBUG // Debug ----------------------------------------------

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------


#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

#endif // WORDRING_D_DEBUG ----------------------------------------------------

namespace wordring
{
namespace debug
{

void debug_memory_leak();

} // namespace debug
} // namespace wordring

#endif // WORDRING_DEBUG_H
