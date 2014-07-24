/**
 * @file    wordring/exception.h
 *
 * @brief   環境に依存する例外を抽象化します
 *
 * @details
 *          このファイル内のクラスは環境依存のオブジェクトを外部に見せないので
 *          pimplになっていません。
 *          detail/<環境名>/以下に実装ファイルを置いてください。
 *          【例】src/wordring/detail/win32/exception.cpp
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
#ifndef WORDRING_EXCEPTION_H
#define WORDRING_EXCEPTION_H

#include <system_error>

namespace wordring
{

class dummy;


class exception_category : public std::error_category
{
public:
	virtual char const* name() const { return "wordring error"; }
	virtual std::string message(int ev) const;
	// virtual error_condition default_error_condition(int ev) const noexcept;
	// virtual bool equivalent(int code, const error_condition& condition) const noexcept;
	// virtual bool equivalent(const error_code& code, int condition) const noexcept;


	static exception_category g_exception_category;
};

exception_category const& get_exception_category();

void check_assertion(bool btrue);

} // namespace wordring

#endif // WORDRING_EXCEPTION_H
