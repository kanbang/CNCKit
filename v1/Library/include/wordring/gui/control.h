/**
 * @file    wordring/gui/control.h
 *
 * @brief   GUIコントロール
 *
 * @details
 *          ボタン、スクロールバーなどのGUIコントロールの基礎を定義します。
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

#include <wordring/wordring.h>

#include <wordring/gui/style.h>
#include <wordring/gui/shape_int.h>
#include <wordring/gui/canvas.h>
#include <wordring/gui/message.h>
#include <wordring/gui/mouse.h>

#include <wordring/gui/layout.h>
#include <wordring/gui/render.h>

#include <cstdint>
#include <memory>
#include <functional>
#include <iterator>

namespace wordring
{
namespace gui
{

class window_service; // 前方宣言
class window;         // 前方宣言
//class render;         // 前方宣言

//class root_window;    // 前方宣言
//class container;      // 前方宣言
//class root_container; // 前方宣言

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

	typedef std::vector<control::store> storage_type;
	typedef storage_type::iterator iterator;
	typedef storage_type::const_iterator const_iterator;
	typedef storage_type::reverse_iterator reverse_iterator;
	typedef storage_type::const_reverse_iterator const_reverse_iterator;

public:
	/**
	 * @brief   子から親へ辿るイテレータです
	 *
	 * @details
	 *          コントロール階層の子から親へ辿ります。
	 *          インクリメントすると現在指しているコントロールの親を指すように
	 *          なります。
	 *          前進イテレータなので、デクリメントできません。
	 */
	class ancestor_iterator
		: public std::iterator < std::forward_iterator_tag, control* >
	{
	private:
		control *m_current;

	public:
		/// end()相当のイテレータを作ります
		ancestor_iterator();

		/**
		 * @brief   begin()相当のイテレータを作ります
		 *
		 * @param   c 初期状態でポイントするコントロール
		 */
		ancestor_iterator(control *c);

		control* operator *();

		control const* operator *() const;

		control* operator ->();

		control const* operator ->() const;

		ancestor_iterator operator ++(int);

		ancestor_iterator& operator ++();

		bool operator != (ancestor_iterator const& rhs) const;

		bool operator == (ancestor_iterator const& rhs) const;
	};

	/**
	 * @brief   親から子へセレクタにマッチするコントロールを辿るイテレータです
	 *
	 * @details
	 *          コントロールの階層は、ツリー状になっています。
	 *          子から親へ辿るのは一つの道しかありませんが、親から子へは複数の
	 *          道があります。
	 *          このイテレータは、選択する条件を付けることでどの子を選ぶか決定
	 *          します。
	 *          イテレータをインクリメントすると、現在ポイントしている
	 *          コントロールの子を末尾から先頭に向けて検索します。
	 *          兄弟の末尾から先頭に向けて検索し、最初に条件に合った子をポイント
	 *          します。
	 */
	class descendant_reverse_iterator
		: public std::iterator<std::forward_iterator_tag, control*>
	{
	public:
		typedef std::function<bool(control*)> selector_function;

	private:
		selector_function  m_selector;
		control           *m_current;

	public:
		/// rend()相当のイテレータを作ります
		descendant_reverse_iterator();

		/**
		 * @brief   firstをポイントする、rbegin()相当のイテレータを作ります
		 *
		 * @param   first イテレータが最初に指すコントロール
		 *
		 * @param   fn
		 *          bool fn(control *c)という形式の関数オブジェクトを指定します。
		 *          この関数は、cが選択条件に合う場合、trueを返してください。
		 */
		descendant_reverse_iterator(control *first, selector_function fn);

		control* operator *();

		control const* operator *() const;

		control* operator ->();

		control const* operator ->() const;

		descendant_reverse_iterator operator ++(int);

		descendant_reverse_iterator& operator ++();

		bool operator != (descendant_reverse_iterator const& rhs) const;

		bool operator == (descendant_reverse_iterator const& rhs) const;
	};

	struct state
	{
		bool focus : 1;
		bool hover : 1;
	};

protected:
	control  *m_parent; ///< 親コンテナ
	rect_int  m_rc;     ///< コントロールの長方形
	state     m_state;  ///< コントロールの状態

	style::store  m_style;  ///< 共有されるスタイル
	layout::store m_layout; ///< レイアウト
	render::store m_render; ///< 描画

	storage_type  m_storage; ///< 子コントロール

	// 構築・破棄 -------------------------------------------------------------
protected:
	/// コントロールを作成します
	explicit control(rect_int rc);

	control(rect_int rc, layout::store l);

public:
	/// コントロールを破棄します
	virtual ~control();

	/**
	 * @brief   コントロールを作成します
	 *
	 * @details 
	 *          オブジェクトのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static store create(rect_int rc);

	// 親子関係 ---------------------------------------------------------------

	/**
	 * @brief   コンテナがcの先祖であるか調べます
	 *
	 * @return  コンテナがcの先祖である場合、trueを返します。
	 *          コンテナがcの先祖でない場合、falseを返します。
	 *          cがthisである場合、falseを返します。
	 */
	bool is_ancestor_of(control const *c) const;

	/// 位置ptを含む最子孫のコントロールを検索します
	control* find_descendant_from_point(point_int pt);

	/**
	 * @brief   [内部用]親コンテナを取り付けます
	 *
	 * @details 
	 *          親コンテナから呼び出されます。
	 *          root_containerには親コンテナが無いため、呼び出されません。
	 *          代わりに、attach_root_window_internalが呼び出されます。
	 */
	void attach_parent_internal(control *parent);

	/// [内部用]親コンテナを取り外します
	void detach_parent_internal();

	/**
	 * @brief   [内部用]ウィンドウを取り付けます
	 *
	 * @details 
	 *          ウィンドウを持つコントロールは、このメンバが呼び出される
	 *          タイミングでウィンドウを生成します。
	 *          ウィンドウを持たないコントロールは何もしません。
	 *          コンテナは、自身のウィンドウを処理した後、子のatatch_window()を
	 *          順に呼び出します。
	 * @param
	 *          pw 親ウィンドウ
	 */
	void attach_window_internal(window *pw);

	/**
	 * @brief   [内部用]ウィンドウを取り外します
	 *
	 * @details
	 *          ウィンドウを持つコントロールは、このメンバが呼び出される
	 *          タイミングでウィンドウを破棄します。
	 *          ウィンドウを持たないコントロールは何もしません。
	 *          コンテナは、子のdetach_window()を順に呼び出した後、自身の
	 *          ウィンドウを処理します。
	 */
	void detach_window_internal();

	/**
	 * @brief   親コンテナを取得します
	 *
	 * @details
	 *          最上位のコンテナは親を持たないため、nullptrを返します。
	 */
	control* get_parent();

	/**
	 * @brief   親コンテナを取得します
	 *
	 * @details
	 *          最上位のコンテナは親を持たないため、nullptrを返します。
	 */
	control const* get_parent() const;

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

	/// 子コントロール配列の最初をポイントするイテレータを返します
	iterator begin();

	/// 子コントロール配列の末尾の次をポイントするイテレータを返します
	iterator end();

	/// 子コントロール配列の最初をポイントするイテレータを返します
	const_iterator begin() const;

	/// 子コントロール配列の末尾の次をポイントするイテレータを返します
	const_iterator end() const;

	/// 子コントロール配列の末尾をポイントする逆イテレータを返します
	reverse_iterator rbegin();

	/// 子コントロール配列の最初の前をポイントする逆イテレータを返します
	reverse_iterator rend();

	/// 子コントロール配列の末尾をポイントする逆イテレータを返します
	const_reverse_iterator rbegin() const;

	/// 子コントロール配列の最初の前をポイントする逆イテレータを返します
	const_reverse_iterator rend() const;

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	/// コントロールがウィンドウの場合、trueを返します
	virtual bool is_window() const;

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;

	/**
	 * @brief   コントロールが配置されているウィンドウを検索します
	 * 
	 * @details
	 *          コントロール自身がウィンドウの場合もあり得ます。
	 */
	window* find_window();

	/**
	 * @brief   コントロールをウィンドウに変換します
	 *
	 * @details
	 *          コントロール自身がウィンドウの場合は、このメンバをオーバーライド
	 *          してください。
	 *          クロス・キャストを避けるため仮想関数で実装します。
	 * @return
	 *          thisをwindow*に変換して返します。
	 *          windowを継承していないコントロールは、nullptrを返します。
	 */
	virtual window* to_window();

	/**
	 * @brief   スレッドのウィンドウ・サービスを検索します
	 *
	 * @return  ウィンドウ・サービス
	 */
	virtual window_service* find_service();

	// 表示 -------------------------------------------------------------------

	/// コントロールの表示状態を返します
	virtual bool is_visible() const;

	/// コントロールを表示します
	void show();

	/// コントロールを非表示にします
	void hide();

	// 描画 -------------------------------------------------------------------

	/// コントロール全体を再描画します
	virtual void repaint();

	/// コントロールの指定部分を再描画します
	virtual void repaint(rect_int rc);

	// 大きさ・位置 -----------------------------------------------------------

	/// コントロールの位置を設定する
	void set_position(point_int pt);

	/// コントロールの位置を取得する
	point_int get_position() const;

	/**
	 * @brief   コントロールの長方形を設定する
	 * 
	 * @details 
	 *          コントロールの長方形を変更すると、結果として祖先の変更を引き
	 *          起こす場合があります。
	 *          祖先の変更は、結果として子孫の変更を引き起こします。
	 *          従って、無限ループが起こる可能性があるため、このメンバを
	 *          オーバーライドする場合、ループ・ガードが必要です。
	 *          
	 *          長方形の変更によってdo_size()メッセージ・ハンドラが引き起こされ
	 *          その中から更に長方形の変更が引き起こされる場合があります。
	 *          無駄な変更を避けるため、レイアウトの実施はウィンドウ・サービス
	 *          に委譲します。
	 *          委譲は親コンテナのrequest_layout()呼び出しによって適切に
	 *          取り扱われます。
	 *
	 * @param   rc 長方形
	 */
	void set_rect(rect_int rc);

	/**
	 * @brief   [内部用] コントロールの長方形を設定する
	 *
	 * @details
	 *          コントロールの長方形を設定します。
	 *          notifyがtrueの場合、親のレイアウトに通知します。
	 *          親のレイアウトがset_rect_internalを呼び出す場合、notifyをfalse
	 *          にして呼び出します。
	 *          これをしない場合、呼び出しがループします。
	 *
	 * @param   rc 長方形
	 *
	 * @param   notify trueの場合、親のレイアウトへ通知します
	 *
	 * @param   paint trueの場合、再描画します
	 */
	void set_rect_internal(rect_int rc, bool notify, bool paint);

	/// コントロールの長方形を取得する
	rect_int get_rect() const;

	/// コントロールの大きさを設定する
	void set_size(size_int size);

	/// コントロールの大きさを取得する
	size_int get_size() const;

	/// 推奨される大きさを取得する
	virtual size_int get_preferred_size() const;
	
	/// 最少の大きさを取得する
	virtual size_int get_min_size() const;
	
	/// 最大の大きさを取得する
	virtual size_int get_max_size() const;

	/// ウィンドウからの相対位置を取得する
	point_int query_offset_from_window() const;
	
	/// ウィンドウからの相対長方形を取得する
	rect_int query_rect_from_window() const;

	/// cからコントロールまでのオフセットを取得する
	point_int query_offset_from(control *c) const;

	virtual bool hit_test(point_int pt) const;

	// スタイル ---------------------------------------------------------------

	control_style* get_style();

	control_style const* find_style(style::state st) const
	{
		control_style const* result = nullptr;
		if (m_style)
		{
			result = static_cast<control_style const *>(
				m_style->find(style::type::control, st));
		}
		return result;
	}

	void set_style(style::store s);

	// レイアウト -------------------------------------------------------------

	/// レイアウトを設定します
	void set_layout(layout::store l);

	/// レイアウトを取得します
	layout* get_layout();

	// タイマー ---------------------------------------------------------------

	/// msミリ秒後に発火するタイマーを設定します
	virtual void set_timer(int32_t ms);

	// ライブラリ定義のメッセージ・ハンドラ -----------------------------------

	/// ライブラリが実装するメッセージ機構のハンドラです
	virtual void do_message_internal(message &m);

	// マウス・メッセージ -----------------------------------------------------

	/// マウス・ボタンが押されたとき呼び出されます
	virtual bool do_mouse_down(mouse &m);

	/// [内部用] マウス・ボタンが押されたとき呼び出されます
	bool do_mouse_down_internal(mouse &m);

	/*
	 * @brief   マウスの移動で呼び出されます
	 *
	 * @details
	 *          マウスの移動に反応するには、このメンバをオーバーライドします。
	 */
	virtual void do_mouse_move(mouse &m);

	/*
	 * @brief   [内部用] マウスの移動で呼び出されます
	 *
	 * @details
	 *          containerはこのメンバを実装しています。
	 *          その中で、メッセージの配送を処理しています。
	 */
	void do_mouse_move_internal(mouse &m);

	/// マウス・ポインタがコントロールに入ったとき呼び出されます
	virtual void do_mouse_over(mouse &m);

	/// マウス・ポインタがコントロールから出たとき呼び出されます
	virtual void do_mouse_out();

	/// マウス・ボタンが離されたとき呼び出されます
	virtual bool do_mouse_up(mouse &m);

	// [内部用] マウス・ボタンが離されたとき呼び出されます
	bool do_mouse_up_internal(mouse &m);

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_create();

	virtual void do_create_internal();

	virtual void do_destroy();

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);

	/// [内部用]再描画要求で呼び出されます
	void do_paint_internal(canvas &cv);

	virtual void do_size(size_int size);
};

class test_control : public control
{
protected:
	color_rgb m_fg_color, m_bg_color;
	int32_t m_id;

protected:
	test_control(rect_int rc, int32_t id);

public:
	virtual ~test_control();

	static control::store create(rect_int rc, int32_t id);

	virtual bool do_mouse_down(mouse &m);

	virtual void do_mouse_over(mouse &m);

	virtual void do_mouse_out();

	virtual bool do_mouse_up(mouse &m);

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_H
