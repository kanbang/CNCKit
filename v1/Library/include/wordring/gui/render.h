/**
 * @file    wordring/gui/render.h
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

#include <wordring/wordring.h>

#include <memory>

#ifndef WORDRING_RENDER_H
#define WORDRING_RENDER_H

namespace wordring
{
namespace gui
{

class dummy;

/**
 * @brief   コントロールの描画を行います
 *
 * @details
 *          コントロールの描画は、コントロールの機能と分離できます。
 *          例えばボタンは、バツ印、矢印、文字列、画像などを表示しますが、
 *          押されたらハンドラを実行するというボタン本来の機能性と、描画内容は
 *          関係がありません。
 *          表示内容に合わせてクラスを作るのは無駄なので、描画部分だけ分離
 *          します。
 */
class render
{
public:
	typedef std::shared_ptr<render> store;

};



} // namespace gui
} // namespace wordring

#endif // WORDRING_RENDER_H
