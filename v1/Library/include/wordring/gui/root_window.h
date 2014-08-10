/**
 * @file    wordring/gui/root_window.h
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

#ifndef WORDRING_ROOT_WINDOW_H
#define WORDRING_ROOT_WINDOW_H

#include <wordring/gui/window_container_tmpl.h>

#include <wordring/gui/container.h>

#include <wordring/gui/detail/native_window.h>

#include <deque>

namespace wordring
{
namespace gui
{

class root_window; // 前方宣言

/**
 * @brief   最祖先のコンテナです
 *
 * @details 
 *          デスクトップ直下のウィンドウのクライアント領域に配置されるコンテナ
 *          です。
 *          レイアウト要求はネストするため、OSに処理を返さずにこのクラスで処理
 *          します。
 *          この処理は特にLinuxでの処理の重さを改善する狙いがあります。
 *          レイアウト要求の圧縮や調停を行う能力があります。
 */
class root_container : public window_container_tmpl<container, window>
{
public:
	typedef window_container_tmpl<container, window> base_type;
	typedef std::unique_ptr<root_container> store;

	typedef std::deque<container*> layout_storage_type;

protected:
	root_window *m_root_window;
	layout_storage_type m_layout_requests; ///< レイアウト要求のキュー

	// 構築・破棄 -------------------------------------------------------------
protected:
	/// コントロールを作成します
	root_container();

public:
	/// コントロールを破棄します
	virtual ~root_container();

	/**
	 * @brief   コントロールを作成します
	 *
	 * @details
	 *          コントロールのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static store create();

	/// 親となるルート・ウィンドウを取り付けます
	virtual void attach_root_window(root_window *parent);

	/// 親となるルート・ウィンドウを取り外します
	virtual void detach_root_window();

	/// ルート・コンテナに親コンテナは無く、attach_root_windowを使用します
	virtual void attach_parent(container *parent);

	/// ルート・コンテナに親コンテナは無く、detach_root_windowを使用します
	virtual void detach_parent();

	/// 再帰的にウィンドウを取り付けます
	virtual void attach_window();

	/**
	 * @brief   キューからcに関連付けられたレイアウト要求を削除します
	 *
	 * @details
	 *          このメンバは、取り外されようとしているコンテナから呼び出され
	 *          ます。
	 */
	virtual void remove_layout_requests(container *c);

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	/// ルート・コンテナを検索します
	virtual root_container* find_root_container();

	/// ルート・ウィンドウを設定します
	virtual void set_root_window(root_window *rw);

	/// ルート・ウィンドウを検索します
	virtual root_window* find_root_window();

	/// スレッドのウィンドウ・サービスを検索します
	virtual window_service* find_service();

	// レイアウト調停 ---------------------------------------------------------

	/// cのレイアウトを要求します
	virtual void request_layout(container *c);
};

/**
 * @brief   ルート・ウィンドウの基本クラスです
 *
 * @details
 *          デスクトップ直下のウィンドウです。
 *          この実装は、タイトルバーを持ちます。
 *          タイトルバーを持たないなど他の特徴を持つデスクトップ直下の
 *          ウィンドウを作るには、このクラスから派生して、相応の特徴を持つ
 *          native_windowをコンストラクタで設定してください。
 */
class root_window : public window
{
public:
	typedef std::unique_ptr<root_window> store;

protected:
	window_service *m_service;
	root_container::store m_client; ///< クライアント領域

	point_int m_pt; ///< 位置
	size_int m_size; ///< 大きさ

	// 構築・破棄 -------------------------------------------------------------
protected:
	/**
	 * @brief   ルート・ウィンドウを構築します
	 *
	 * @param   ws ウィンドウ・サービス
	 * @param   rc ウィンドウの長方形
	 */
	root_window(rect_int rc);

	/**
	 * @brief   ルート・ウィンドウを構築します
	 *
	 * @param   ws ウィンドウ・サービス
	 * @param   rc ウィンドウの長方形
	 * @param   nw 環境依存のウィンドウ実装
	 */
	root_window(rect_int rc, detail::native_window::store nw);

public:
	/// クライアントを破棄して終了します
	virtual ~root_window();

	/**
	 * @brief   ルート・ウィンドウを作成します
	 *
	 * @details
	 *          オブジェクトのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static store create(rect_int rc);

	void attach_parent(window_service *ws);

	void detach_parent();

	void set_client(root_container::store s);

	container* get_client();

	/**
	 * @brief   クライアント領域にコントロールを割り当てます
	 */
	control* assign(control::store s);

	// 情報 -------------------------------------------------------------------

	void set_service(window_service *ws);

	/// スレッドのウィンドウ・サービスを返します
	window_service* get_service();

	// 表示 -------------------------------------------------------------------

	/// ウィンドウを表示します
	void show();

	/// ウィンドウを非表示にします
	void hide();

	// 大きさ・位置 -----------------------------------------------------------

	/// ウィンドウの大きさを設定する
	void set_rect(rect_int rc);

	rect_int get_rect() const;

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_destroy_window();

	virtual void do_size_window(size_int size);

	virtual void do_paint_window(canvas &cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_ROOT_WINDOW_H
