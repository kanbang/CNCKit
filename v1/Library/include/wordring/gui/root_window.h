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

#ifndef WORDRING_ROOT_WINDOW_CONTAINER_H
#define WORDRING_ROOT_WINDOW_CONTAINER_H

#include <wordring/gui/window_container_tmpl.h>

#include <wordring/gui/container.h>

#include <wordring/gui/detail/native_window.h>

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
 *          メッセージの圧縮や調停を行う能力があります。
 */
class root_container : public window_container_tmpl<container, window>
{
public:
	typedef window_container_tmpl<container, window> base_type;
	typedef std::unique_ptr<root_container> store;

protected:
	root_window *m_root_window;
	std::deque<container*> m_layout_requests; ///< レイアウト要求のキュー

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

	virtual void create(root_window *rw);

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual char const* get_control_name() const;

	/// ルート・コンテナを検索します
	virtual root_container* find_root_container();

	/// ルート・ウィンドウを設定します
	virtual void set_root_window(root_window *rw);

	/// ルート・ウィンドウを検索します
	virtual root_window& find_root_window();

	/// スレッドのウィンドウ・サービスを検索します
	virtual window_service& find_service();

	// レイアウト調停 ---------------------------------------------------------

	/// レイアウトを要求します
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
protected:
	window_service &m_service;
	root_container::store m_client; ///< クライアント領域

	// 構築・破棄 -------------------------------------------------------------
public:
	/**
	 * @brief   ルート・ウィンドウを構築します
	 *
	 * @param   ws ウィンドウ・サービス
	 * @param   rc ウィンドウの長方形
	 */
	root_window(window_service &ws, rect_int rc);

	/**
	 * @brief   ルート・ウィンドウを構築します
	 *
	 * @param   ws ウィンドウ・サービス
	 * @param   rc ウィンドウの長方形
	 * @param   nw 環境依存のウィンドウ実装
	 */
	root_window(
		window_service &ws, rect_int rc, detail::native_window::store nw);

	control* assign(control::store s);

	// 情報 -------------------------------------------------------------------

	/// スレッドのウィンドウ・サービスを返します
	window_service& get_service() { return m_service; }

	// 表示 -------------------------------------------------------------------

	/// ウィンドウを表示します
	void show();

	/// ウィンドウを非表示にします
	void hide();

	// 大きさ・位置 -----------------------------------------------------------

	/// ウィンドウの大きさを設定する
	void set_rect(rect_int rc);

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_destroy_window()
	{
		m_service.quit();
	}

	virtual void do_size_window(size_int size);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_ROOT_WINDOW_CONTAINER_H
