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

#include <wordring/debug.h>

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
	typedef std::unique_ptr<control> store;
	typedef std::vector<store> storage_type;
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

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;

	/**
	 * @brief   一番近いコンテナを返します
	 *
	 * @details thisがコンテナの場合、thisを返します。
	 */
	virtual container* find_container();

	// 親子関係 ---------------------------------------------------------------

	/// コンテナがcの先祖であるか調べます
	bool is_ancestor_of(control *c) const;

	/// 子コントロールの配列を返します
	storage_type& get_children();

	virtual void attach_window();

	virtual void detach_window();

//	iterator begin();

//	const_iterator begin() const;

//	iterator end();

//	const_iterator end() const;

	// 描画 -------------------------------------------------------------------

	// 大きさ・位置 -----------------------------------------------------------

	/// 子コントロールの再配置を行います
	virtual void perform_layout();
	
	/// レイアウトを要求します
	void request_layout();


	// マウス・メッセージ -----------------------------------------------------

	/*
	 * @brief   マウスの移動で呼び出されます
	 *
	 * @details
	 *          マウスの移動に反応するには、このメンバをオーバーライドします。
	 *          戻り値でfalseを返すと、親コンテナのdo_mouse_move()に権利が
	 *          移ります。
	 *
	 * @return  メッセージを処理した場合trueを返します。
	 */
	virtual bool do_mouse_move(point_int pt) { return false; }

	/*
	 * @brief   内部用: マウスの移動で呼び出されます
	 *
	 * @details
	 *          containerはこのメンバを実装しています。
	 *          その中で、メッセージの配送を処理しています。
	 *
	 * @return  do_mouse_move()の戻り値をそのまま返します。
	 */
	virtual bool do_mouse_move_internal(point_int pt);

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	/// 内部用: 再描画要求で呼び出されます
	virtual void do_paint_internal(canvas& cv);

};

class test_container : public container
{
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
