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

#endif // WORDRING_WORDRING_H
