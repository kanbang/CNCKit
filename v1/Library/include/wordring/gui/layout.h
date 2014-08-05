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

#include <cstdint>
#include <memory>

namespace wordring
{
namespace gui
{

class container;

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
	 * @brief   コンテナ内のコントロールをレイアウトします
	 *
	 * @details
	 *          このメンバは、コンテナからコンテナ自身を引数に呼び出されます。
	 *
	 *          コンテナのレイアウト変更は、さらに親コンテナのレイアウト変更を
	 *          引き起こす場合があります。
	 *
	 *          container::perform_layout()の説明を読んでください。
	 */
	virtual void perform_layout(container* c);

	/**
	 * @brief   レイアウトを作成します
	 *
	 * @details 
	 *          メモリー管理のために存在します。
	 *          レイアウトの作成は必ずレイアウト自身で行います。
	 *          寿命の管理、アロケータの統一を実現します。
	 */
	static layout::store create();
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

	virtual void perform_layout(container* c);

	static layout::store create();
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

	virtual void perform_layout(container* c);

	static layout::store create();
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_LAYOUT_H
