/**
 * @file    wordring/opengl/gl_window.cpp
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

#include <wordring/debug.h>

#include <wordring/opengl/gl_window.h>
//#include <wordring/opengl/detail/native_gl_window.h>
#include <wordring/opengl/detail/win32/win32_gl_window.h>

using namespace wordring::opengl;

gl_window::gl_window()// : container_window(new detail::native_gl_window_impl)
{

}

gl_window::~gl_window()
{

}

