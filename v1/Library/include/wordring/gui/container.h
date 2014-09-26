/**
 * @file    wordring/gui/container.h
 *
 * @brief   GUIコンテナ
 *
 * @details
 *          ボタンやスクロールバーなど、GUIのコントロールを載せるコンテナです。
 *          containerクラスはインターフェースとして宣言されます。
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

#ifndef WORDRING_CONTAINER_H
#define WORDRING_CONTAINER_H

#include <wordring/wordring.h>

#include <wordring/gui/control.h>
#include <wordring/gui/layout.h>

#include <list>
#include <vector>
#include <string>
#include <memory>

#include <cassert>

namespace wordring
{
namespace gui
{

class container : public control
{
public:
	typedef std::vector<control::store> storage_type;
	typedef storage_type::iterator iterator;
	typedef storage_type::const_iterator const_iterator;

protected:
	//storage_type m_storage;

	// 構築・破棄 -------------------------------------------------------------
protected:
	/// コンテナを構築します
	explicit container(rect_int rc);

	/**
	 * @brief   コンテナを構築します
	 *
	 * @param   l レイアウト
	 */
	container(rect_int rc, layout::store l);

public:
	/// コンテナを破棄します
	virtual ~container();

	/**
	 * @brief   コントロールを作成します
	 *
	 * @details
	 *          コントロールのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static control::store create(rect_int rc);

	/**
	 * @brief   コントロールを作成します
	 *
	 * @details
	 *          コントロールのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static control::store create(rect_int rc, layout::store l);

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;


	// 親子関係 ---------------------------------------------------------------


	// 描画 -------------------------------------------------------------------

	// 大きさ・位置 -----------------------------------------------------------

	// マウス・メッセージ -----------------------------------------------------

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

};

class test_container : public container
{
protected:
	color_rgb m_fg_color, m_bg_color;

protected:
	test_container(rect_int rc);

public:
	virtual ~test_container();

	static control::store create(rect_int rc);

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_H
