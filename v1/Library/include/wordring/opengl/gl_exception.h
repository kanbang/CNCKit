/**
 * @file    wordring/opengl/gl_exception.h
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
#ifndef WORDRING_GL_EXCEPTION_H
#define WORDRING_GL_EXCEPTION_H

#include <wordring/wordring.h>

#include <string>
#include <stdexcept>

#include <GL/glew.h>

namespace wordring
{
namespace opengl
{

class dummy;

class gl_error_category : public std::error_category
{
protected:

public:
	const char* name() const
	{
		return "OpenGL error";
	}
};


} // namespace opengl
} // namespace wordring

#endif // WORDRING_GL_EXCEPTION_H
