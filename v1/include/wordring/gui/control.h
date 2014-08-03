/**
 * @file    wordring/gui/control.h
 *
 * @brief   GUIコントロール
 *
 * @details
 *          ボタン、スクロールバーなどのGUIコントロールの基礎を宣言します。
 *          controlクラスはインタフェースとして宣言されます。
 *          共通の機能が実装されます。
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

#ifndef WORDRING_CONTROL_H
#define WORDRING_CONTROL_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>
#include <wordring/gui/canvas.h>

#include <cstdint>
#include <functional>

namespace wordring
{
namespace gui
{

class window_service; // 前方宣言
class window;         // 前方宣言
class root_window;    // 前方宣言
class container;      // 前方宣言
class root_container; // 前方宣言
struct message;       // 前方宣言

/**
 * @brief   GUIコントロール基本クラス
 *
 * @details 全てのGUIコントロールの祖先です。
 *          コントロールに必要な基本機能とインターフェースを備えています。
 */
class control
{
public:
	typedef std::unique_ptr<control> store;

protected:
	container* m_parent; ///< 親コンテナ

	size_int   m_size;   ///< コントロールの幅と高さ
	point_int  m_pt;     ///< 親コンテナ原点からの相対位置

	// 構築・破棄 -------------------------------------------------------------
public:
	/// コントロールを作成します
	control();

	/// コントロールを破棄します
	virtual ~control();

	/// ウィンドウ作成、初期描画等のタイミングに使います
	virtual void create(container *parent, rect_int rc);

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	/// コントロールがウィンドウの場合、trueを返します
	virtual bool is_window() const;

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;

	/**
	 * @brief   コントロールが配置されているウィンドウを検索します
	 * 
	 * @details コントロール自身がウィンドウの場合もあり得ます。
	 *          コントロール自身がウィンドウの場合は、このメンバをオーバーライド
	 *          してください。
	 */
	virtual window& find_window();

	/**
	 * @brief   一番近いコンテナを返します
	 *
	 * @details thisがコンテナの場合、thisを返します。
	 */
	virtual container* find_container();

	/**
	 * @brief   ルート・コンテナを検索します
	 */
	virtual root_container* find_root_container();

	/**
	 * @brief   ルート・ウィンドウを設定します
	 *
	 * @details
	 *          ルート・ウィンドウはデスクトップ直下のウィンドウです。
	 */
	virtual void set_root_window(root_window &rw);

	/**
	 * @brief   ルート・ウィンドウを検索します
	 *
	 * @details 
	 *          
	 */
	virtual root_window& find_root_window();

	/**
	 * @brief   スレッドのウィンドウ・サービスを検索します
	 *
	 * @return  ウィンドウ・サービス
	 */
	virtual window_service& find_service();

	// 表示 -------------------------------------------------------------------

	/// コントロールの表示状態を返します
	virtual bool is_visible() const;

	/// コントロールを表示します
	virtual void show();

	/// コントロールを非表示にします
	virtual void hide();

	// 親子関係 ---------------------------------------------------------------
protected:
	virtual void set_parent(container *parent);

public:
	/**
	 * @brief   親コンテナを取得します
	 *
	 * @details 
	 *          最上位のコンテナは親を持たないため、nullptrを返します。
	 */
	virtual container* get_parent();
	virtual container const* get_parent() const;

	/**
	 * @brief   cがコントロールの祖先か調べます
	 *
	 */
	bool is_ancestor(container *c) const;
	
	// 描画 -------------------------------------------------------------------

	/// コントロール全体を再描画します
	virtual void repaint();
	/// コントロールの指定部分を再描画します
	virtual void repaint(rect_int rc);
	
	// 大きさ・位置 -----------------------------------------------------------

	/// コントロールの大きさを設定する
	virtual void set_size(size_int size);

	/// コントロールの大きさを取得する
	virtual size_int get_size() const;

	/// コントロールの位置を設定する
	virtual void set_position(point_int pt);

	/// コントロールの位置を取得する
	virtual point_int get_position() const;

	/**
	 * @brief   コントロールの長方形を設定する
	 * 
	 * @details 実際の処理は、親コンテナのset_child_rect_internal()に委譲され、
	 *          その中からset_rect_internal()呼び出しによって設定されます。
	 *          親コンテナは、設定されているlayoutに処理を委譲する場合があります。
	 *
	 *          コントロールの長方形とウィンドウの長方形は明確に区別されて
	 *          います。
	 *          コントロールの長方形は、control.m_ptとcontrol.m_sizeで示され
	 *          ます。
	 *          ウィンドウの長方形はウィンドウ・システムが保持する値です。
	 *
	 *          ルート・コントロールは親を持たないため、特別な処理が必要です。
	 *          
	 *          ウィンドウを持つコントロールは特別な処理が必要です。
	 *
	 *          ウィンドウの変更後、do_size_w()が呼び出されます。
	 *          do_size_w()応答の結果として再レイアウトが行われると、それが更に
	 *          do_size_w()呼び出しを引き起こし、ループが起こります。
	 *          従って、do_size_w()はウィンドウの長方形とコントロールの長方形を
	 *          比較し、異なる場合のみ再レイアウトを引き起こさなければなりません。
	 *
	 *          do_size_w()は、レイアウトの結果のみではなく、ユーザーの操作に
	 *          よってウィンドウの大きさが変更された時も呼び出されます。
	 *          この場合、上記の比較アルゴリズムが有効に機能します。
	 *
	 *          コントロールの長方形を変更すると、結果として祖先と兄弟の変更を
	 *          引き起こします。
	 *          従って、実際に変更された長方形をユーザーに表示するのは、最も
	 *          祖先のset_child_rect_internal()です。
	 *
	 * @param   rc 長方形
	 */
	virtual void set_rect(rect_int rc);

public:
	/// コントロールの大きと位置さを取得する
	virtual rect_int get_rect() const;

	/// 推奨される大きさを取得する
	virtual size_int get_preferred_size() const;
	
	/// 最少の大きさを取得くする
	virtual size_int get_min_size() const;
	
	/// 最大の大きさを取得する
	virtual size_int get_max_size() const;

	/// フォームからの相対位置を取得する
	virtual point_int query_position_from_form() const;

	/// ウィンドウからの相対位置を取得する
	virtual point_int query_position_from_window() const;

	// 内部用: メッセージ・ハンドラ -------------------------------------------

	virtual void do_message_internal(message &m);

	virtual void do_create_internal();

	virtual void do_layout_internal(container &c);

	virtual void do_paint_internal(canvas &cv);

	/**
	 * @brief   大きさが変更された時に呼び出されます
	 *
	 * @details 大きさが変更された時に、メッセージの伝搬を行います。
	 *          コントロール内部専用です。
	 *
	 * @param   need_layout 再レイアウトが必要ない場合、falseに設定します。
	 *                      ループを防ぐために必要です。
	 */
	virtual void do_size_internal(size_int rc);

	// マウス・メッセージ -----------------------------------------------------

	virtual void do_mouse_move(point_int pt);
	virtual void do_mouse_over(point_int pt);
	virtual void do_mouse_out(point_int pt);
	//virtual bool do_click();

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_create();
	virtual void do_destroy();
	virtual void do_paint(canvas& cv);
	virtual void do_size(size_int size);

public:
	// イベント・ハンドラ -----------------------------------------------------
	/*
	std::function<void(point_int)> on_mouse_move;
	std::function<void(point_int)> on_mouse_out;
	std::function<void(point_int)> on_mouse_over;
	std::function<bool()> onMouseDown;
	std::function<bool()> onMouseUp;
	//std::function<void(point_int, int32_t)> on_click;
	std::function<bool()> onDblClick;

	std::function<bool()> onKeyPress;
	std::function<bool()> onKeyDown;
	std::function<bool()> onKeyUp;

	std::function<void()> on_create;
	std::function<void()> on_destroy;
	std::function<void(canvas&)> on_paint;
	std::function<void(size_int)> on_size;
	*/

	std::string strip_function_name(std::string s)
	{
		int i = s.find_last_of(':'); return s;
		return std::string(s.begin() + s.find_last_of(':'), s.end());
	}

	int32_t get_message_count();
};



} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_H
