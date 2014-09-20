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
	std::unique_ptr<layout> m_layout;
	storage_type m_children;

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

	/**
	 * @brief   コンテナの末尾に子コントロールを追加します
	 *
	 * @details
	 *          control::storeはコピーできません。
	 *          std::move()で所有権を移動させてください。
	 *
	 * @return  追加したコントロールへのポインタを返します。
	 */
	control* push_back(control::store s);

	/// レイアウトを設定します
	void set_layout(layout::store l);

	/// レイアウトを取得します
	layout* get_layout();

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;

	/**
	 * @brief   一番近いコンテナを返します
	 *
	 * @details thisがコンテナの場合、thisを返します。
	 */
	virtual container* find_container();

	// 親子関係 ---------------------------------------------------------------

	/**
	 * @brief   コンテナがcの先祖であるか調べます
	 *
	 * @return  コンテナがcの先祖である場合、trueを返します。
	 *          コンテナがcの先祖でない場合、falseを返します。
	 *          cがthisである場合、falseを返します。
	 */
	bool is_ancestor_of(control const *c) const;

	/// 子コントロールの配列を返します
	storage_type& get_children();

	/**
	 * @brief   [内部用] コントロールにウィンドウを取り付けます
	 *
	 * @details
	 *          親に追加される時、attach_parent()から呼び出されます。
	 *          再帰的に子へ伝搬しウィンドウを取り付けていきます。
	 *          この動作により、
	 */
	virtual void attach_window_internal();

	/**
	 * @brief   [内部用] コントロールからウィンドウを取り外します
	 *
	 * @details
	 *          親から取り外される時、detach_parent()から呼び出されます。
	 *          再帰的に子へ伝搬しウィンドウを取り外していきます。
	 */
	virtual void detach_window_internal();

//	iterator begin();

//	const_iterator begin() const;

//	iterator end();

//	const_iterator end() const;

	// 描画 -------------------------------------------------------------------

	// 大きさ・位置 -----------------------------------------------------------

	virtual void set_rect_internal(rect_int rc, bool notify, bool paint);

	// マウス・メッセージ -----------------------------------------------------

	virtual bool do_mouse_down_internal(mouse &m);

	/*
	 * @brief   [内部用] マウスの移動で呼び出されます
	 *
	 * @details
	 *          containerはこのメンバを実装しています。
	 *          その中で、メッセージの配送を処理しています。
	 */
	virtual void do_mouse_move_internal(mouse &m);

	virtual bool do_mouse_up_internal(mouse &m);

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	/// [内部用] 再描画要求で呼び出されます
	virtual void do_paint_internal(canvas& cv);

};

class test_container : public container
{
protected:
	rgb_color m_fg_color, m_bg_color;

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
