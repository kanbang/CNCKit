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

#include <wordring/wordring.h>

#include <wordring/gui/window_container.h>
#include <wordring/gui/container.h>

#include <wordring/gui/detail/native_window.h>

#include <deque>

namespace wordring
{
namespace gui
{

class root_window; // 前方宣言

/**
 * @brief   ルート・ウィンドウのクライアント領域に使われるコンテナです
 *
 * @details 
 *          デスクトップ直下のウィンドウのクライアント領域に配置されるコンテナ
 *          です。
 *          レイアウト要求はネストするため、OSに処理を返さずにこのクラスで処理
 *          します。
 *          この処理は特にLinuxでの処理の重さを改善する狙いがあります。
 *          レイアウト要求の圧縮や調停を行う能力があります。
 */
class root_container : public window_container
{
public:
	typedef std::unique_ptr<root_container> store;

protected:
	root_window *m_root_window;

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
	virtual void attach_root_window_internal(root_window *parent);

	/// 親となるルート・ウィンドウを取り外します
	virtual void detach_root_window_internal();

	/// 再帰的にウィンドウを取り付けます
	//virtual void attach_window_internal();

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	/// ルート・ウィンドウを検索します
	virtual root_window* find_root_window();

	/// スレッドのウィンドウ・サービスを検索します
	virtual window_service* find_service();

	// 大きさ・位置 -----------------------------------------------------------

	void set_rect(rect_int rc);
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
	 * @param   rc ウィンドウの長方形
	 */
	root_window(rect_int rc);

	/**
	 * @brief   ルート・ウィンドウを構築します
	 *
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

	void attach_service_internal(window_service *ws);

	void detach_service_internal();

	void set_client(root_container::store s);

	container* get_client();

	/**
	 * @brief   クライアント領域にコントロールを割り当てます
	 */
	control* assign(control::store s);

	// 情報 -------------------------------------------------------------------

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

	// その他 -----------------------------------------------------------------

	std::wstring get_title_text() const;

	void set_title_text(std::wstring s);

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_destroy_window();

	virtual void do_size_window(size_int size);

	virtual void do_paint_window(canvas &cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_ROOT_WINDOW_H
