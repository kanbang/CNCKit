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
public:
	/// コンテナを構築します
	container();

	explicit container(layout *l);

	/// コンテナを破棄します
	virtual ~container();

	template <typename T>
	T* assign(T* pT)
	{
		m_children.push_back(store(pT));
		pT->T::create(this, pT->T::get_rect());
		return pT;
	}

	/// レイアウトを設定します
	void set_layout(std::unique_ptr<layout>&& l);

	// 情報 -------------------------------------------------------------------

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	/**
	 * @brief   一番近いコンテナを返します
	 *
	 * @details thisがコンテナの場合、thisを返します。
	 */
	virtual container* find_container();

	bool including(control *c) const;

	// 親子関係 ---------------------------------------------------------------

	/// 子コントロールの配列を返します
	storage_type& get_children();

//	iterator begin();

//	const_iterator begin() const;

//	iterator end();

//	const_iterator end() const;

	// 描画 -------------------------------------------------------------------

	virtual void do_paint_internal(canvas& cv);

	/**
	 * @brief   大きさが変更された時に呼び出されます
	 *
	 * @details 大きさが変更された時に、メッセージの伝搬を行います。
	 *          コントロール内部専用です。
	 *
	 * @param   need_layout 再レイアウトが必要ない場合、falseに設定します。
	 *                      ループを防ぐために必要です。
	 */

	// 大きさ・位置 -----------------------------------------------------------

	/// 子コントロールの再配置を行います
	virtual void perform_layout();
	
	/// レイアウトを要求します
	void request_layout();

	// テンプレート用のスタブです
	//virtual void request_layout(container *c);

	virtual container* perform_layout_internal(container *c);

	virtual void do_size_internal(size_int size);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_H
