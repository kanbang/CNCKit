/**
 * @file    wordring/debug.cpp
 *
 * @brief   デバッグ実装ファイル
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

#include <wordring/debug.h>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

void wordring::debug::debug_memory_leak()
{
#ifdef WORDRING_D_DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif // WORDRING_D_DEBUG
}

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------


#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------
