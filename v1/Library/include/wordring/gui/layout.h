/**
 * @file    wordring/gui/layout.h
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

#ifndef WORDRING_LAYOUT_H
#define WORDRING_LAYOUT_H

#include <wordring/wordring.h>

#include <wordring/gui/shape_int.h>
#include <wordring/gui/canvas.h>

#include <cstdint>
#include <memory>

namespace wordring
{
namespace gui
{

class control;
class container;

class layout_context
{
public:
protected:
	canvas m_canvas;

public:
	layout_context(){}
};

/// 何もしないレイアウトです
class layout
{
public:
	typedef std::unique_ptr<layout> store;


protected:
	layout();

public:
	virtual ~layout();

	/**
	 * @brief   レイアウトを作成します
	 *
	 * @details 
	 *          メモリー管理のために存在します。
	 *          レイアウトの作成は必ずレイアウト自身で行います。
	 *          寿命の管理、アロケータの統一を実現します。
	 */
	static layout::store create();

	/**
	 * @brief   コンテナ内のコントロールをレイアウトします
	 *
	 * @details
	 *          このメンバは、コンテナからコンテナ自身を引数に呼び出されます。
	 *
	 *          コンテナのレイアウトは、さらに親コンテナのレイアウトを
	 *          引き起こす場合があります。
	 */
	virtual void perform_layout(container* c);

	/**
	 * @brief   子コントロールの位置が変更されたとき呼び出されます
	 *
	 * @param   child 原因となった子コントロール
	 *
	 * @param   old 以前の位置
	 */
	virtual void do_child_position(control *child, point_int old);

	/**
	 * @brief   子コントロールの長方形が変更されたとき呼び出されます
	 *
	 * @param   child 原因となった子コントロール
	 *
	 * @param   old 以前の長方形
	 */
	virtual void do_child_rect(control *child, rect_int old);

	/**
	 * @brief   子コントロールの大きさが変更されたとき呼び出されます
	 *
	 * @param   child 原因となった子コントロール
	 *
	 * @param   old 以前の大きさ
	 */
	virtual void do_child_size(control *child, size_int old);
};

/**
 * @brief   文章の配置のようなレイアウトを行います
 *
 * @details 
 *          x方向に子コントロールを順番に配置します。
 *          右端に達するとy方向に下がって、左端から配置を再開します。
 */
class flow_layout : public layout
{
protected:
	flow_layout();

public:
	virtual ~flow_layout();

	static layout::store create();

	virtual void perform_layout(container* c);
};

/**
 * @brief   コンテナ全面にコントロールを拡大して配置します
 *
 * @details
 *          複数のコントロールがある場合、重なって配置されます。
 */
class full_layout : public layout
{
protected:
	full_layout();

public:
	virtual ~full_layout();

	static layout::store create();

	virtual void perform_layout(container* c);

};

class vertical_layout : public layout
{
protected:
	vertical_layout();

public:
	virtual ~vertical_layout();

	static layout::store create();

	virtual void perform_layout(container* c);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_LAYOUT_H
