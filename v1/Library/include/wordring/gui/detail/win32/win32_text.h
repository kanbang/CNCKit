/**
 * @file    wordring/gui/detail/win32/win32_text.h
 *
 * @brief   
 *
 * @details
 *          フォントが変更された場合、レンダーは再構築されなければならない
 *          ため、フォントはポインタのみ保持しています。
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

#include <wordring/gui/shape_int.h>

#include <wordring/gui/detail/native_text.h>

#include <wordring/gui/detail/win32/win32_font.h>
#include <wordring/gui/detail/win32/win32_ime.h>

#include <Windows.h>
#include <usp10.h>

#ifndef WORDRING_WIN32_TEXT_H
#define WORDRING_WIN32_TEXT_H

namespace wordring
{
namespace gui
{

class control;

namespace detail
{

class dummy;

class native_text_service_impl : public native_text_service
{
private:
	SCRIPT_DIGITSUBSTITUTE m_digitsubstitute;

public:
	SCRIPT_DIGITSUBSTITUTE* get_digitsubstitute();
};

/**
 * @brief   保存用に文字列範囲を記録するクラス
 *
 * @details
 *          シェーピングエンジンを切り替えずに描画できる文字列の範囲を記録する
 *          クラスです。
 *          国際化文字列は様々な言語を含むので、使用するシェーピングエンジンで
 *          文字列を切り分けます。
 *          このクラスは、文字列に対して、どの範囲がどのシェーピングエンジンに
 *          対応するか記録します。
 *
 *          インラインにイメージなどを配置できるよう、コントロールへのポインタ
 *          を共用で保存できます。
 *          保存しているデータを識別するためにm_typeメンバを持ちます。
 *
 *          m_typeメンバにline_endが設定されている場合、改行アイテムを意味
 *          します。
 */
class text_item
{
public:
	enum : uint16_t
	{
		none = 0, line_end, string, image,
	};

private:
	SCRIPT_ANALYSIS m_analysis; ///< 言語特性

	union
	{
		control *m_control; ///< インライン画像を保持する場合のポインタ
		wchar_t *m_data;    ///< 文字列範囲を保存する場合の先頭ポインタ
	};
	uint32_t m_type    : 8;  ///< 保持するデータの形式
	uint32_t m_slength : 24; ///< 文字列長

public:
	// 構築・破棄 -------------------------------------------------------------

	/// 空のアイテムを作ります
	text_item(uint32_t type = none);

	text_item(SCRIPT_ANALYSIS sa, wchar_t *pch, uint32_t slength);

	text_item(text_item const &ri);

	// 大きさ -----------------------------------------------------------------

	uint32_t get_slength() const;

	// 情報 -------------------------------------------------------------------

	uint32_t get_type() const;

	bool is_empty() const;

	// 取得・設定 -------------------------------------------------------------

	SCRIPT_ANALYSIS* get_analysis(); // shape,placeで必要

	SCRIPT_ANALYSIS const* get_analysis() const;

	control* get_control();

	wchar_t* get_data();

	// 操作 -------------------------------------------------------------------

	/// 先頭からslength文字、取り出す
	text_item trim_front_by_slength(uint32_t slength);
};

/**
 * @brief   描画・配置用の機能を持つテキストアイテム
 *
 * @details
 *          コントロールにマウスが載ったとき、コントロールの大きさが変わる、
 *          文字列が追加される、スタイルが変更されるなどで、文字列の再配置が
 *          必要なとき、キーボードフォーカスが当たったとき、描画する時、
 *          text_itemを元に、追加の情報を復元します。
 *
 *          必要になるまでtext_itemの状態で保存し、グリフの情報が必要になった
 *          時点でrender_itemに復元することでメモリーを節約します。
 */
class render_item : public text_item
{
protected:
	std::vector<WORD>    m_glyphs;    // グリフに対して一つの要素
	std::vector<int>     m_advances;  // グリフに対して一つの要素
	std::vector<int>     m_justifies; // グリフに対して一つの要素
	std::vector<GOFFSET> m_goffsets;  // グリフに対して一つの要素

	std::vector<WORD>    m_log_cluster; // キャラクタに対して一つの要素

	ABC m_abc; // アイテムに対して一つ

	std::vector<SCRIPT_VISATTR> m_visattr; // グリフに対して一つの要素
	std::vector<SCRIPT_LOGATTR> m_logattr; // キャラクタに対して一つの要素

	native_font_impl *m_font; ///< フォント

	point_int m_pt;         ///< レンダー内での位置
	uint16_t m_line_height; ///< 行高さ

	uint16_t m_ch_offset; ///< レンダーの先頭からの文字オフセット
	uint16_t m_ct_offset; ///< レンダーの先頭からのキャレットオフセット

public:
	// 構築・破棄 -------------------------------------------------------------

	render_item(uint32_t type = text_item::none);

	// アイテム列から復元するとき使う
	render_item(text_item const &rhs);

	// 文字列から分割する時使う
	render_item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength);

	// コンテナに挿入するとき使う
	render_item(render_item &&rhs);

	// フォント ---------------------------------------------------------------

	void set_font(native_font_impl *f);

	native_font_impl* get_font();

	// 大きさ・位置 -----------------------------------------------------------

	void set_position(point_int pt);

	uint32_t get_height() const;

	uint32_t get_width() const;

	void set_line_height(uint32_t cy);

	// 文字数・クラスタ数・キャレット移動数・グリフ数 -------------------------

	void set_ch_offset(uint32_t ich);

	uint32_t get_ch_offset() const;

	/// キャレット移動数でのオフセットを設定する
	void set_ct_offset(uint32_t ict);

	/// キャレット移動数でのオフセットを取得する
	uint32_t get_ct_offset() const;

	void set_glength(size_t nglyph);

	/// グリフ数を返す
	uint32_t get_glength() const;

	/// クラスタ数を返す
	uint32_t get_clength() const;

	/// キャレット移動数を返す
	uint32_t get_ctlength() const;

	// キャレット移動数から文字数を返す
	uint32_t query_slength_from_ctlength(uint32_t ctlength) const;

	// 文字数からキャレット数を返す
	uint32_t query_ctlength_from_slength(uint32_t slength) const;

	// 分割できる最小文字数を返す
	// イメージは0を返す
	// 分割できない場合は0を返す
	uint32_t query_softbreak_slength_by_minimum() const;

	// 幅cx以内で分割できる最大文字長を返す
	// イメージは0を返す
	// 分割できない場合は0を返す
	uint32_t query_softbreak_slength_by_width(uint32_t cx) const;

	// 操作 -------------------------------------------------------------------

	/// 
	render_item trim_front_by_slength(HDC hdc, uint32_t slength);

	/// スクリプトの分割可能位置、改行可能位置等を計算し設定します
	LRESULT break_();

	/**
	 * @brief   スクリプトの形状を計算し設定します
	 *
	 * @details
	 *          グリフがキャッシュされている場合、HDCは必要ありません。
	 *
	 * @return   USP_E_SCRIPT_NOT_IN_FONTが返る場合、フォントフォールバックが
	 *           必要です。
	 *           E_OUTOFMEMORYが返る場合、メモリー確保のアルゴリズム変更が
	 *           必要です。
	 *           E_PENDINGが返る場合、HDCを引数にセットして再度呼び出して
	 *           ください。
	 */
	HRESULT shape(HDC hdc = nullptr);

	/**
	 * @brief   スクリプトの配置を計算し設定します
	 *
	 * @details
	 *          グリフがキャッシュされている場合、HDCは必要ありません。
	 *
	 * @return   
	 *           E_PENDINGが返る場合、HDCを引数にセットして再度呼び出して
	 *           ください。
	 */
	LRESULT place(HDC hdc = nullptr);

	// グリフ間隔の調整によって表示幅をdxにする
	HRESULT justify(uint32_t dx);

	HRESULT paint(HDC hdc);

	void paint_under_line_if(HDC hdc, uint32_t offset, uint32_t length);

	// x位置までの文字数を検索する
	uint32_t find_xcp_from_xpt(uint32_t x) const;

	// x位置までのキャレット数を検索する
	uint32_t find_xct_from_xpt(uint32_t x) const;

	// アイテムの先頭からのキャレット位置xctのxピクセル位置を検索する
	uint32_t find_xpt_from_nct(uint32_t nct) const;
};

/**
 * @brief   国際化文字列のパラグラフに相当するデータを保持するクラス
 *
 * @details
 *          コントロールの文字列データとしてパラグラフを保持するためのクラス
 *          です。
 *          コントロールの大きさが変わった時点で、改行位置とシェーピング
 *          エンジンを確定し、保持します。
 *          そのためのデータ保存用クラスです。
 *          実際の描画や計算は、text_renderクラスにセットして行います。
 */
class native_text_impl : public native_text
{
	friend class text_render;

public:
	typedef std::vector<text_item> storage_type;

	typedef storage_type::iterator       iterator;
	typedef storage_type::const_iterator const_iterator;

protected:
	std::wstring           m_string;
	std::vector<text_item> m_storage;

public:
	virtual void set_string(std::wstring s);

	void insert_string(uint32_t ich, std::wstring s);

	void erase_string(uint32_t ich, uint32_t slength);

	wchar_t const* get_sdata() const;

	uint32_t get_slength() const;

	void clear();

	void push_back(text_item const &ti);

	void assign(const_iterator it1, const_iterator it2);

	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;
};

/**
 * @brief   国際化文字列の描画と配置に使うクラスです
 *
 * @details
 *          *
 */
class native_text_render_impl : public native_text_render
{
public:
	typedef std::vector<render_item> storage_type;

	typedef storage_type::iterator       iterator;
	typedef storage_type::const_iterator const_iterator;

protected:
	/// 代替数字のためのキャッシュへのポインタ
	SCRIPT_DIGITSUBSTITUTE *m_digitsubstitute;

	control          *m_control; ///< 親コントロールへのポインタ

	native_text_impl *m_text; ///< 国際化文字列を保持するオブジェクトへのポインタ

	storage_type  m_storage;

	native_font_impl *m_base_font;

	uint32_t m_width;
	uint32_t m_height;

	uint32_t m_nct;

	native_ime_impl::store m_ime;

public:

	virtual void set_text(native_text *nt) {}

	native_text_render_impl();

	void set_control(control *c);

	void parse(HDC hdc);

	void set_size(HDC hdc, uint32_t cx, uint32_t cy);

	// 文字数を返す
	uint32_t get_slength() const;

	// クラスタ数を返す
	uint32_t get_clength() const;

	// キャレット数を返す
	uint32_t get_ctlength() const;

	// キャレット -------------------------------------------------------------

	void set_caret_position(uint32_t ict);

	void move_caret_left();

	void move_caret_right();

	void move_caret_up();

	void move_caret_down();

	void move_caret_home();

	void move_caret_end();

	iterator find_item_from_ctlength(uint32_t nct);

	iterator find_item_from_slength(uint32_t slength);

	// キャレット数から文字数を返す
	uint32_t query_slength_from_ctlength(uint32_t nct) const;

	// 文字数からキャレット数を返す
	uint32_t query_ctlength_from_slength(uint32_t slength) const;

	uint32_t get_nct(uint32_t xct, uint32_t yct) const;

	// キャレット数からxy位置を検索
	point_int find_pt_by_nct(uint32_t nct) const;

	// xy位置からキャレット数を返す
	uint32_t find_nct_by_pt(uint32_t xpt, uint32_t ypt) const;

	uint32_t find_max_yct() const;

	uint32_t find_max_xct(uint32_t yct) const;

	uint32_t find_index_by_yct(uint32_t yct) const;

	uint32_t find_xpt_from_xct(uint32_t xct, uint32_t yct) const;

	uint32_t find_xct_from_xpt(uint32_t xpt, uint32_t yct) const;

	uint32_t find_ypt_from_yct(uint32_t yct) const;

	uint32_t find_yct_from_ypt(uint32_t ypt) const;

	void paint(HDC hdc);

	// ich 文字列での開始位置
	// nch 文字数
	void paint_under_line(HDC hdc, uint32_t ich, uint32_t nch);

	void invalidate(HWND hwnd);

	void clear();

	void set_string(HDC hdc, std::wstring s);

	// ipoint 文字列の位置
	void insert_string(HDC hdc, uint32_t ipoint, std::wstring s);

	// ipoint 文字列の位置
	void erase_string(uint32_t ipoint, uint32_t slength);

	void assign(
		native_text_impl::const_iterator it1,
		native_text_impl::const_iterator it2);

	void set_base_font(native_font_impl *f);

	native_font_impl* get_base_font();

	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;

	void set_focus(HWND hwnd);

	void kill_focus(HWND hwnd);

	void start_composition(HWND hwnd);

	void end_composition(HWND hwnd);

	void process_composition(HWND hwnd, WPARAM wParam, LPARAM lParam);
};












class item_breaker
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	SCRIPT_DIGITSUBSTITUTE *m_sds;

	output_function m_output;

public:
	item_breaker(SCRIPT_DIGITSUBSTITUTE *sds);

	void set_output_function(output_function output);

	void operator()(wchar_t const *pch, uint32_t slength);
};

class font_binder
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	native_font_impl *m_base_font;

	output_function m_output;

public:
	font_binder(native_font_impl *f);

	void set_output_function(output_function output);

	void operator()(render_item ri);
};

class image_binder
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	std::vector<text_item> m_storage;

	output_function m_output;

public:
	//image_binder();

	void set_output_function(output_function output);

	void operator()(render_item ri);
};

class line_breaker
{
public:
	typedef std::function<void(render_item)> output_function;

protected:
	output_function m_output;

	HDC m_hdc;

	uint32_t m_width; // 最大幅

	uint32_t m_axpx; // 前進x位置
	uint32_t m_aypx; // 前進y位置

	uint32_t m_dy; // 行高さ検出用

	uint32_t m_ach; // 前進文字数
	uint32_t m_act; // 前進キャレット数

public:
	line_breaker(HDC hdc, uint32_t width);

	void set_output_function(output_function output);

	void operator()(render_item ri);

	void break_line();

	void output(render_item &&ri);
};


} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_TEXT_H
