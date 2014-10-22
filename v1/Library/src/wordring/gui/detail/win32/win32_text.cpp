/**
 * @file    wordring/gui/detail/win32/win32_text.cpp
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

#include <wordring/wordring.h>

#include <wordring/gui/control.h>
#include <wordring/gui/window_service.h>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_text.h>
#include <wordring/gui/detail/win32/win32_font.h>

#include <cassert>

using namespace wordring::gui;
using namespace wordring::gui::detail;

// native_text_service_impl --------------------------------------------------

SCRIPT_DIGITSUBSTITUTE* native_text_service_impl::get_digitsubstitute()
{
	return &m_digitsubstitute;
}

// text_item -----------------------------------------------------------------

text_item::text_item(uint32_t type)
	: m_analysis({ 0 })
	, m_data(nullptr)
	, m_type(type)
	, m_slength(0)
{

}

text_item::text_item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength)
	: m_analysis(sa)
	, m_data(data)
	, m_type(string)
	, m_slength(slength)
{

}

text_item::text_item(text_item const &rhs)
	: m_analysis(rhs.m_analysis)
	, m_data(rhs.m_data)
	, m_type(rhs.m_type)
	, m_slength(rhs.m_slength)
{

}

uint32_t text_item::get_type() const
{
	return m_type;
}

bool text_item::is_empty() const
{
	return m_slength == 0;
}

SCRIPT_ANALYSIS* text_item::get_analysis()
{
	return &m_analysis;
}

SCRIPT_ANALYSIS const* text_item::get_analysis() const
{
	return &m_analysis;
}

wchar_t* text_item::get_data()
{
	return m_data;
}

control* text_item::get_control()
{
	return m_control;
}

uint32_t text_item::get_slength() const
{
	//assert(m_type == string);
	return m_slength;
}

text_item text_item::trim_front_by_slength(uint32_t slength)
{
	assert(m_type == string);
	wchar_t *tmp = m_data;
	m_data += slength;
	m_slength -= slength;
	return text_item(m_analysis, tmp, slength);
}


// render_item ----------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

render_item::render_item(uint32_t type)
	: text_item(type)
	, m_abc({ 0 })
	, m_pt(0, 0)
	, m_line_height(0)
	, m_ch_offset(0)
	, m_ct_offset(0)
{

}

render_item::render_item(text_item const &rhs)
	: text_item(rhs)
	, m_abc({ 0 })
	, m_pt(0, 0)
	, m_line_height(0)
	, m_ch_offset(0)
	, m_ct_offset(0)
{
	m_log_cluster.resize(get_slength());
}

render_item::render_item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength)
	: text_item(sa, data, slength)
	, m_abc({ 0 })
	, m_pt(0, 0)
	, m_line_height(0)
	, m_ch_offset(0)
	, m_ct_offset(0)
{
	m_log_cluster.resize(slength);
}

render_item::render_item(render_item &&rhs)
	: text_item(rhs)
	, m_glyphs(std::forward<std::vector<WORD> >(rhs.m_glyphs))
	, m_advances(std::forward<std::vector<int> >(rhs.m_advances))
	, m_justifies(std::forward<std::vector<int> >(rhs.m_justifies))
	, m_goffsets(std::forward<std::vector<GOFFSET> >(rhs.m_goffsets))
	, m_log_cluster(std::forward<std::vector<WORD> >(rhs.m_log_cluster))
	, m_abc(rhs.m_abc)
	, m_visattr(std::forward<std::vector<SCRIPT_VISATTR> >(rhs.m_visattr))
	, m_logattr(std::forward<std::vector<SCRIPT_LOGATTR> >(rhs.m_logattr))
	, m_font(rhs.m_font)
	, m_pt(rhs.m_pt)
	, m_line_height(rhs.m_line_height)
	, m_ch_offset(rhs.m_ch_offset)
	, m_ct_offset(rhs.m_ct_offset)
{
	rhs.m_abc = { 0 };
	rhs.m_pt = { 0, 0 };
	rhs.m_line_height = 0;
	rhs.m_ch_offset = 0;
	rhs.m_ct_offset = 0;
}

// フォント -------------------------------------------------------------------

void render_item::set_font(native_font_impl *f)
{
	m_font = f;
}

native_font_impl* render_item::get_font()
{
	return m_font;
}

// 大きさ・位置 ---------------------------------------------------------------

void render_item::set_position(point_int pt)
{
	m_pt = pt;
}

uint32_t render_item::get_height() const
{
	uint32_t height = 0;
	if (m_font) height = m_font->get_height();
	return height;
}

uint32_t render_item::get_width() const
{
	uint32_t b = m_abc.abcB;
	if (0xFFFFFFF0 < b)
	{
		b = m_font->get_height() * 4;
	}
	assert(0 <= m_abc.abcA + b + m_abc.abcC);
	return m_abc.abcA + b + m_abc.abcC;
}

void render_item::set_line_height(uint32_t cy)
{
	m_line_height = cy;
}

// 文字数・クラスタ数・キャレット移動数・グリフ数 -----------------------------

void render_item::set_ch_offset(uint32_t ich)
{
	m_ch_offset = ich;
}

uint32_t render_item::get_ch_offset() const
{
	return m_ch_offset;
}

void render_item::set_ct_offset(uint32_t ict)
{
	m_ct_offset = ict;
}

uint32_t render_item::get_ct_offset() const
{
	return m_ct_offset;
}

void render_item::set_glength(size_t nglyph)
{
	m_glyphs.resize(nglyph);
	m_advances.resize(nglyph);
	m_justifies.resize(nglyph);
	m_goffsets.resize(nglyph);

	m_visattr.resize(nglyph);
}

// グリフ数を返す
uint32_t render_item::get_glength() const
{
	return m_glyphs.size();
}

// クラスタ数を返す
uint32_t render_item::get_clength() const
{
	uint32_t result = 0;
	uint32_t xcp = get_slength();
	uint32_t iclust = 0xFFFFFFFF;
	for (uint32_t i = 0; i < xcp; i++)
	{
		if (iclust != m_log_cluster[i])
		{
			iclust = m_log_cluster[i];
			++result;
		}
	}

	return result;
}

uint32_t render_item::get_ctlength() const
{
	// 画像などはキャレットが一つ移動するので、1を返す
	switch (get_type())
	{
	case text_item::string:   return get_clength();
	case text_item::line_end: return 0;
	case text_item::image:    return 1;
	}
	assert(false); // 他のタイプを追加した場合、この関数もエントリを追加する
	return 0;
}

// キャレット移動数から文字数を返す
uint32_t render_item::query_slength_from_ctlength(uint32_t nct) const
{
	assert(get_type() != text_item::line_end);

	uint32_t ich = 0;
	uint32_t maxch = get_slength();
	uint32_t act = 0;

	uint32_t iclust = 0xFFFFFFFF;

	while (act < nct)
	{
		uint32_t cclust = m_log_cluster[ich];
		if (iclust != cclust)
		{
			iclust = cclust;
			++act;
		}
		++ich;
	}
	return ich;
}

// 文字数からキャレット数を返す
uint32_t render_item::query_ctlength_from_slength(uint32_t slength) const
{
	assert(get_type() != text_item::line_end);
	assert(slength <= get_slength());

	uint32_t ict = 0;
	uint32_t iclust = 0xFFFFFFFF;

	for (uint32_t i = 0; i < slength; i++)
	{
		uint32_t cclust = m_log_cluster[i];
		if (iclust != cclust)
		{
			iclust = cclust;
			++ict;
		}
	}

	return ict;
}

uint32_t render_item::query_softbreak_slength_by_minimum() const
{
	assert(get_type() != text_item::line_end);

	uint32_t ich = 0;
	uint32_t maxch = get_slength();

	uint32_t iclust = 0xFFFFFFFF;

	for (; ich < maxch; ich++)
	{
		uint32_t cclust = m_log_cluster[ich];
		if (iclust != cclust)
		{
			iclust = cclust;
			if (m_logattr[ich].fSoftBreak) break;
		}
	}
	return (ich == maxch) ? 0 : ich;
}

// 幅cx以内で分割できる最大文字長を返す
uint32_t render_item::query_softbreak_slength_by_width(uint32_t cx) const
{
	assert(get_type() != text_item::line_end);
	assert(cx < get_width());

	uint32_t ich = 0; // 現在文字インデックス
	uint32_t maxch = get_slength();

	uint32_t iclust = 0xFFFFFFFF; // 現在クラスタ

	uint32_t dx = 0; // 現在幅

	uint32_t ilastbreak = 0;

	for (; ich < maxch; ich++)
	{
		uint32_t cclust = m_log_cluster[ich];
		if (iclust != cclust) // 次のクラスタに移動した
		{
			iclust = cclust;
			if (m_logattr[ich].fSoftBreak) ilastbreak = ich;
			dx += m_advances[iclust];
			if (cx < dx) break;
			if (cx == dx) break;
		}
	}

	return ilastbreak; // 最後に分割できたインデックスは、文字数と一致する
}

// 操作 -----------------------------------------------------------------------

render_item render_item::trim_front_by_slength(HDC hdc, uint32_t slength)
{
	assert(0 < slength && slength < get_slength());

	render_item result = text_item::trim_front_by_slength(slength);
	result.set_font(get_font());
	result.shape(hdc);
	result.place(hdc);

	return result;
}

LRESULT render_item::break_()
{
	m_logattr.resize(get_slength());
	LRESULT ret = ::ScriptBreak(
		get_data(), get_slength(),
		get_analysis(),
		m_logattr.data()); // [out]
	assert(ret == 0);

	return ret;
}

HRESULT render_item::shape(HDC hdc)
{
	assert(m_font);

	HRESULT ret = 0;
	::SelectObject(hdc, m_font->get_handle());
	// バッファが足りるまで大きさを増やしてリトライ
	// バッファの大きさについては、ScriptShape()の解説に説明がある
	size_t nchar = get_slength();
	for (size_t i = 0; i < 10; i++)
	{
		set_glength(size_t((nchar * 1.5f + 16) * (1.0f + i / 5.0f)));
		int nresult = 0;

		ret = ::ScriptShape(
			hdc, m_font->get_cache(),
			get_data(), get_slength(),
			get_glength(),
			get_analysis(),
			m_glyphs.data(),
			m_log_cluster.data(),
			m_visattr.data(),
			&nresult);
		set_glength(nresult);

		if (ret == USP_E_SCRIPT_NOT_IN_FONT)
		{
			break;
		}
		if (ret == 0) break;
	}
	return ret;
}

LRESULT render_item::place(HDC hdc)
{
	assert(m_font);

	LRESULT ret = ::ScriptPlace(
		hdc, m_font->get_cache(),
		m_glyphs.data(), get_glength(),
		m_visattr.data(),
		get_analysis(),
		m_advances.data(), // [out]
		m_goffsets.data(), // [out]
		&m_abc);           // [out]
	assert(ret == 0);

	return ret;
}

// グリフ間隔の調整によって表示幅をdxにする
HRESULT render_item::justify(uint32_t dx)
{
	HRESULT ret = ::ScriptJustify(
		m_visattr.data(),
		m_advances.data(),
		get_glength(),
		dx,
		2,
		m_justifies.data());
	assert(ret == 0);

	return ret;
}

HRESULT render_item::paint(HDC hdc)
{
	assert(m_font);

	::SelectObject(hdc, m_font->get_handle());

	HRESULT ret = ::ScriptTextOut(
		hdc,
		m_font->get_cache(),
		m_pt.x, m_pt.y,
		0,
		NULL,
		get_analysis(),
		NULL, 0,
		m_glyphs.data(),
		get_glength(),
		m_advances.data(),
		NULL,//m_justifies.data(),
		m_goffsets.data());
	if (ret)
	{
		int i = 0;
	}
	assert(ret == 0);

	return ret;
}

void render_item::paint_under_line_if(HDC hdc, uint32_t offset, uint32_t length)
{
	uint32_t ch1 = std::max(offset, (uint32_t)m_ch_offset);
	uint32_t ch2 = std::min(offset + length, (uint32_t)m_ch_offset + get_slength());
	if (ch2 <= ch1) return;

	ch1 -= m_ch_offset;
	ch2 -= m_ch_offset;

	uint32_t xpt1 = find_xpt_from_nct(ch1);
	uint32_t xpt2 = find_xpt_from_nct(ch2);

	::MoveToEx(hdc, xpt1, m_pt.y + get_height(), NULL);
	::LineTo(hdc, xpt2, m_pt.y);

}

// x位置までの文字数を検索する
uint32_t render_item::find_xcp_from_xpt(uint32_t x) const
{
	int icp;
	int itrailing;
	HRESULT ret = ::ScriptXtoCP(
		x,
		get_slength(),
		get_glength(),
		m_log_cluster.data(),
		m_visattr.data(),
		m_advances.data(),
		get_analysis(),
		&icp,
		&itrailing);

	return icp + itrailing;
}

// x位置までのキャレット数を検索する
uint32_t render_item::find_xct_from_xpt(uint32_t x) const
{
	uint32_t result = 0;
	uint32_t xcp = find_xcp_from_xpt(x);
	uint32_t iclust = 0xFFFFFFFF;
	for (uint32_t i = 0; i < xcp; i++)
	{
		if (iclust != m_log_cluster[i])
		{
			iclust = m_log_cluster[i];
			++result;
		}
	}

	return get_analysis()->fRTL ? get_clength() - result : result;
}



// アイテムの先頭からのキャレット位置xctのxピクセル位置を検索する
uint32_t render_item::find_xpt_from_nct(uint32_t nct) const
{
	uint32_t result = 0;

	uint32_t iclust = 0xFFFFFFFF;
	uint32_t axct = 0;
	for (uint32_t i = 0; i < get_slength(); i++)
	{
		if (axct == nct) break;
		uint32_t j = get_analysis()->fRTL ? get_slength() - i - 1 : i;
		if (iclust != m_log_cluster[j])
		{
			iclust = m_log_cluster[j];
			result += m_advances[iclust];
			++axct;
		}
	}
	return result;
}





// native_text_impl -----------------------------------------------------------

void native_text_impl::set_string(std::wstring s)
{
	m_string = s;
	m_storage.clear();
}

wchar_t const* native_text_impl::get_sdata() const
{
	return m_string.data();
}

void native_text_impl::insert_string(uint32_t ipoint, std::wstring s)
{
	m_string.insert(ipoint, s);
}

void native_text_impl::erase_string(uint32_t ipoint, uint32_t slength)
{
	m_string.erase(ipoint, slength);
}

uint32_t native_text_impl::get_slength() const
{
	return m_string.length();
}

void native_text_impl::clear()
{
	m_storage.clear();
}

void native_text_impl::push_back(text_item const &ti)
{
	m_storage.push_back(ti);
}

void native_text_impl::assign(const_iterator it1, const_iterator it2)
{
	m_storage.clear();
	m_storage.insert(begin(), it1, it2);
}

native_text_impl::iterator native_text_impl::begin()
{
	return m_storage.begin();
}

native_text_impl::const_iterator native_text_impl::begin() const
{
	return m_storage.begin();
}

native_text_impl::iterator native_text_impl::end()
{
	return m_storage.end();
}

native_text_impl::const_iterator native_text_impl::end() const
{
	return m_storage.end();
}


// native_text_render_impl ----------------------------------------------------

native_text_render_impl::native_text_render_impl()
	: m_text(nullptr)
	, m_digitsubstitute(nullptr)
	, m_control(nullptr)
	, m_width(0)
	, m_height(0)
	, m_nct(0)
{

}

void native_text_render_impl::set_control(control *c)
{
	m_control = c;

	if (m_digitsubstitute == nullptr)
	{
		text_service &ts = c->find_service()->get_text_service();
		m_digitsubstitute = static_cast<native_text_service_impl*>(
			ts.get_native())->get_digitsubstitute();
	}
}

void native_text_render_impl::parse(HDC hdc)
{
	m_storage.clear();

	item_breaker ib(m_digitsubstitute);
	font_binder  fb(m_base_font);
	image_binder imgb;
	line_breaker lb(hdc, m_width);

	ib.set_output_function([&](render_item ri){ fb(ri); });
	fb.set_output_function([&](render_item ri){ imgb(ri); });
	imgb.set_output_function([&](render_item ri){ lb(ri); });
	lb.set_output_function([&](render_item ri){
		m_storage.push_back(ri);
	});

	ib(m_text->get_sdata(), m_text->get_slength());
}

void native_text_render_impl::set_size(HDC hdc, uint32_t cx, uint32_t cy)
{
	m_width = cx;
	m_height = cy;
	parse(hdc);

	point_int pt = find_pt_by_nct(m_nct);

	// ウィンドウを最小化→復元で失敗する
	// おそらく、最小化でキャレット非表示、復元でキャレット表示の必要がある
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	//assert(ret != 0);
}

uint32_t native_text_render_impl::get_slength() const
{
	return m_text->get_slength();
}

uint32_t native_text_render_impl::get_clength() const
{
	uint32_t result = 0;
	for (render_item const &ri : m_storage)
	{
		result += ri.get_clength();
	}
	return result;
}

uint32_t native_text_render_impl::get_ctlength() const
{
	uint32_t nct = 0;
	for (render_item const &ri : m_storage) nct += ri.get_ctlength();
	return nct;
}

// キャレット -------------------------------------------------------------

void native_text_render_impl::set_caret_position(uint32_t ict)
{
	m_nct = ict;
	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
}

void native_text_render_impl::move_caret_left()
{
	if (0 < m_nct) --m_nct;

	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
}


void native_text_render_impl::move_caret_right()
{
	if (m_nct < get_ctlength()) ++m_nct;
	else
	{
		// コントロールの終わりに達しているので、親に通知する
	}
	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
}

void native_text_render_impl::move_caret_up()
{

	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
}

void native_text_render_impl::move_caret_down()
{

	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
}

void native_text_render_impl::move_caret_home()
{
	m_nct = 0;
	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
}
void native_text_render_impl::move_caret_end()
{
	m_nct = get_ctlength();
	point_int pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
}

native_text_render_impl::iterator
native_text_render_impl::find_item_from_ctlength(uint32_t nct)
{
	uint32_t act = 0; // キャレットの前進位置
	iterator it1 = begin(), it2 = end();
	while (it1 != it2)
	{
		act += it1->get_ctlength();
		if (nct <= act) break;
		++it1;
	}
	return it1;
}


native_text_render_impl::iterator
native_text_render_impl::find_item_from_slength(uint32_t slength)
{
	uint32_t ach = 0;
	iterator it1 = begin(), it2 = end();
	while (it1 != it2)
	{
		ach += it1->get_slength();
		if (slength <= ach) break;
		++it1;
	}
	return it1;
}

uint32_t native_text_render_impl::query_slength_from_ctlength(uint32_t nct) const
{
	uint32_t nch = 0;
	uint32_t act = 0;
	for (render_item const &ri : m_storage)
	{
		uint32_t cct = ri.get_ctlength();
		if (act + cct < nct)
		{
			act += cct;
			nch += ri.get_slength();
			continue;
		}
		else if (act + cct == nct)
		{
			nch += ri.get_slength();
			break;
		}
		nch += ri.query_slength_from_ctlength(nct - act);
		break;
	}
	return nch;
}

uint32_t native_text_render_impl::query_ctlength_from_slength(uint32_t slength) const
{
	uint32_t ict = 0;
	uint32_t ach = 0;
	for (render_item const &ri : m_storage)
	{
		uint32_t cch = ri.get_slength();
		uint32_t cct = ri.get_ctlength();
		if (ach + cch < slength)
		{
			ach += cch;
			ict += cct;
			continue;
		}
		else if (ach + cch == slength)
		{
			ict += cct;
			break;
		}
		ict += ri.query_ctlength_from_slength(slength - ach);
		break;
	}
	return ict;
}

uint32_t native_text_render_impl::get_nct(uint32_t xct, uint32_t yct) const
{
	uint32_t result = 0;
	uint32_t ayct = 0;
	for (render_item const &ri : m_storage)
	{
		if (ri.get_type() == text_item::line_end) ++ayct;
		if (ayct == yct) break;
		result += ri.get_ctlength();
	}
	return result + xct;
}

point_int native_text_render_impl::find_pt_by_nct(uint32_t nct) const
{
	point_int pt(0, 0);
	uint32_t act = 0; // キャレット前進位置
	uint32_t dy = 0;  // 現在の行高さ
	for (render_item const &ri : m_storage)
	{
		if (act == nct) break;

		uint32_t cct = ri.get_ctlength(); // 注目描画アイテムのキャレット数

		// 改行を検出
		if (ri.get_type() == text_item::line_end)
		{
			pt.y += dy; // 行の高さを加算
			dy = 0;       // 行の高さをクリア
			pt.x = 0;   // x位置をクリア
			act += cct;   // 改行は0キャレット
			//assert(cct == 1);
			continue;
		}

		dy = std::max(dy, ri.get_height());

		// 描画アイテムのキャレット数を足しても前進位置があふれない場合
		if (act + cct <= nct)
		{
			act += cct; // キャレット位置を前進
			pt.x += ri.get_width(); // x位置を加算
			continue;
		}

		pt.x += ri.find_xpt_from_nct(nct - act);
		break;
	}
	return pt;
}

// xy位置からキャレット数を返す
uint32_t native_text_render_impl::find_nct_by_pt(uint32_t xpt, uint32_t ypt) const
{
	uint32_t nct = 0;
	uint32_t dy = 0; // 行の高さ


	return 0;
}

uint32_t native_text_render_impl::find_max_yct() const
{
	uint32_t result = 0;
	for (render_item const &ri : m_storage)
	{
		if (ri.get_type() == text_item::line_end) ++result;
	}
	return result;
}

uint32_t native_text_render_impl::find_max_xct(uint32_t yct) const
{
	assert(yct <= find_max_yct());
	uint32_t result = 0;
	const_iterator it1 = begin() + find_index_by_yct(yct), it2 = end();
	while (it1 != it2)
	{
		if (it1->get_type() == text_item::line_end) break;
		result += it1->get_ctlength();
		++it1;
	}
	return result;
}

uint32_t native_text_render_impl::find_index_by_yct(uint32_t yct) const
{
	assert(yct <= find_max_yct());
	uint32_t result = 0;
	uint32_t act = 0;
	for (render_item const &ri : m_storage)
	{
		if (act == yct) break;
		if (ri.get_type() == text_item::line_end) ++act;
		++result;
	}
	return result;
}

uint32_t native_text_render_impl::find_xpt_from_xct(uint32_t xct, uint32_t yct) const
{
	uint32_t result = 0;
	uint32_t nct = 0;
	const_iterator it1 = begin() + find_index_by_yct(yct), it2 = end();
	while (it1 != it2)
	{
		uint32_t act = it1->get_ctlength();
		if (nct + act <= xct)
		{
			nct += act;
			result += it1->get_width();
			if (nct == xct) break;
		}
		else
		{
			//result += it1->find_xpt_from_xct(xct -  nct);
			break;
		}
		++it1;
	}
	return result;
}

uint32_t native_text_render_impl::find_xct_from_xpt(uint32_t xpt, uint32_t yct) const
{
	uint32_t result = 0;
	uint32_t npt = 0;
	const_iterator it1 = begin() + find_index_by_yct(yct), it2 = end();
	while (it1 != it2)
	{
		uint32_t apt = it1->get_width();
		if (npt + apt <= xpt)
		{
			npt += apt;
			result += it1->get_ctlength();
			if (npt == xpt) break;
		}
		else
		{
			result += it1->find_xct_from_xpt(xpt - npt);
			break;
		}
		++it1;
	}
	return result;
}

uint32_t native_text_render_impl::find_ypt_from_yct(uint32_t yct) const
{
	uint32_t result = 0;
	uint32_t dy = 0;
	uint32_t act = 0;
	for (render_item const &ri : m_storage)
	{
		if (act == yct) break;
		if (ri.get_type() == text_item::line_end)
		{
			result += dy;
			dy = 0;
			++act;
		}
		else
		{
			dy = std::max(dy, ri.get_height());
		}
	}
	return result;
}

uint32_t native_text_render_impl::find_yct_from_ypt(uint32_t ypt) const
{
	uint32_t result = 0;
	uint32_t dy = 0;
	uint32_t apt = 0;
	for (render_item const &ri : m_storage)
	{
		if (ypt <= apt)
		{
			--result;
			break;
		}

		if (ri.get_type() == text_item::line_end)
		{
			apt += dy;
			dy = 0;
			++result;
		}
		else
		{
			dy = std::max(dy, ri.get_height());
		}
	}
	return result;
}

void native_text_render_impl::paint(HDC hdc)
{
	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t dy = 0;
	for (render_item &ri : m_storage)
	{
		if (ri.get_type() == text_item::line_end)
		{
			y += dy;
			dy = 0;
			x = 0;
			continue;
		}
		ri.paint(hdc);
		x += ri.get_width();
		dy = std::max(dy, ri.get_height());
	}
}

void native_text_render_impl::paint_under_line(HDC hdc, uint32_t ich, uint32_t nch)
{
	for (render_item &ri : m_storage)
	{
		ri.paint_under_line_if(hdc, ich, nch);
	}
}

void native_text_render_impl::invalidate(HWND hwnd)
{
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.bottom = m_height;
	rc.right = m_width;
	::InvalidateRect(hwnd, &rc, TRUE);
}

void native_text_render_impl::clear()
{
	m_text->clear();
	m_storage.clear();
}

void native_text_render_impl::set_string(HDC hdc, std::wstring s)
{
	m_text->set_string(s);
	m_storage.clear();
}

void native_text_render_impl::insert_string(HDC hdc, uint32_t ipoint, std::wstring s)
{
	m_text->insert_string(ipoint, s);
}

void native_text_render_impl::erase_string(uint32_t ipoint, uint32_t slength)
{
	m_text->erase_string(ipoint, slength);
}

void native_text_render_impl::assign(
	native_text_impl::const_iterator it1, native_text_impl::const_iterator it2)
{
	m_storage.insert(begin(), it1, it2);
}

void native_text_render_impl::set_base_font(native_font_impl *f)
{
	m_base_font = f;
}

native_font_impl* native_text_render_impl::get_base_font()
{
	return m_base_font;
}

native_text_render_impl::iterator native_text_render_impl::begin()
{
	return m_storage.begin();
}

native_text_render_impl::const_iterator native_text_render_impl::begin() const
{
	return m_storage.begin();
}

native_text_render_impl::iterator native_text_render_impl::end()
{
	return m_storage.end();
}

native_text_render_impl::const_iterator native_text_render_impl::end() const
{
	return m_storage.end();
}

void native_text_render_impl::set_focus(HWND hwnd)
{
	int32_t h = m_base_font->get_height();
	iterator it = find_item_from_ctlength(m_nct);
	if (it != end()) h = it->get_height();

	BOOL ret = ::CreateCaret(hwnd, (HBITMAP)1, 2, h);
	assert(ret != 0);
	point_int pt = find_pt_by_nct(m_nct);
	ret = ::SetCaretPos(pt.x, pt.y);
	assert(ret != 0);
	ret = ::ShowCaret(hwnd);
	assert(ret != 0);
}

void native_text_render_impl::kill_focus(HWND hwnd)
{
	BOOL ret = ::HideCaret(hwnd);
	assert(ret != 0);
	ret = ::DestroyCaret();
	assert(ret != 0);
}

void native_text_render_impl::start_composition(HWND hwnd)
{
	iterator it = find_item_from_ctlength(m_nct);
	native_font_impl *f = (it != end()) ? it->get_font() : m_base_font;

	m_ime = std::make_unique<native_ime_impl>(this, f);
	uint32_t ipoint = query_slength_from_ctlength(m_nct);
	m_ime->set_insert_point(ipoint);
}

void native_text_render_impl::end_composition(HWND hwnd)
{
	HDC hdc = ::GetDC(hwnd);
	HIMC himc = ::ImmGetContext(hwnd);

	// 旧文字列消去
	uint32_t ipoint = m_ime->get_insert_point();
	uint32_t slength = m_ime->get_slength();
	erase_string(ipoint, slength);
	// 新文字列追加
	std::wstring s = m_ime->get_result_string(himc);
	insert_string(hdc, ipoint, s);

	parse(hdc);

	// カーソル位置
	uint32_t cch = ipoint + m_ime->get_slength();
	uint32_t ict = query_ctlength_from_slength(cch);
	//set_caret_position(ict);

	invalidate(hwnd);

	m_ime = nullptr;
	::ImmReleaseContext(hwnd, himc);
	::ReleaseDC(hwnd, hdc);
}

void native_text_render_impl::process_composition(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HIMC himc = ::ImmGetContext(hwnd);
	HDC hdc = ::GetDC(hwnd);

	if ((lParam & GCS_COMPSTR) != 0)
	{
		// 旧文字列消去
		uint32_t ipoint = m_ime->get_insert_point();
		uint32_t slength = m_ime->get_slength();
		erase_string(ipoint, slength);

		// 新文字列追加
		std::wstring s = m_ime->get_composition_string(himc);
		insert_string(hdc, ipoint, s);

		m_ime->get_attribute(himc);
		// 解析して表示
		parse(hdc);
		invalidate(hwnd);

		// カーソル位置
		uint32_t cch = ipoint + m_ime->get_slength();
		uint32_t ict = query_ctlength_from_slength(cch);
		//set_caret_position(ict);
	}

	if ((lParam & GCS_COMPCLAUSE) != 0)
	{
		m_ime->get_clause(himc);
	}

	if ((lParam & GCS_CURSORPOS) != 0)
	{
		uint32_t ich = m_ime->get_insert_point();
		uint32_t icursol = m_ime->get_cursol_position(himc);

		uint32_t ict = query_ctlength_from_slength(ich + icursol);
		set_caret_position(ict);

		// 候補リスト移動
		point_int pt = find_pt_by_nct(ict);
		m_ime->set_candidate_position(himc, pt.x, pt.y);
	}

	if ((lParam & GCS_DELTASTART) != 0)
	{
		uint32_t ich = m_ime->get_insert_point();
		uint32_t delta = m_ime->get_delta(himc);
		uint32_t ict = query_ctlength_from_slength(ich + delta);
	}

	::ReleaseDC(hwnd, hdc);
	::ImmReleaseContext(hwnd, himc);
}











// item_breaker ---------------------------------------------------------------

item_breaker::item_breaker(SCRIPT_DIGITSUBSTITUTE *sds)
	: m_sds(sds)
{

}

void item_breaker::set_output_function(output_function output)
{
	m_output = output;
}

void item_breaker::operator()(wchar_t const *pch, uint32_t slength)
{
	assert(slength);

	SCRIPT_CONTROL sc = { 0 }, *psc = nullptr;
	SCRIPT_STATE   ss = { 0 }, *pss = nullptr;

	bool complex = ::ScriptIsComplex(
		pch, slength,
		(m_sds->DigitSubstitute != SCRIPT_DIGITSUBSTITUTE_NONE)
		| SIC_COMPLEX | SIC_NEUTRAL) == S_OK;
	if (complex)
	{
		LRESULT ret = ::ScriptApplyDigitSubstitution(m_sds, &sc, &ss);
		assert(ret == S_OK);
		psc = &sc;
		pss = &ss;
	}

	int                      nmax = 1024;
	std::vector<SCRIPT_ITEM> vsi;

	while (nmax <= 100 * 1024)
	{
		int nret = 0;
		vsi.resize(nmax);
		HRESULT ret = ::ScriptItemize(
			pch, slength,
			nmax, psc, pss, vsi.data(), &nret);
		assert(ret == 0 || ret == E_OUTOFMEMORY);
		if (ret == 0)
		{
			vsi.resize(nret + 1);
			break;
		}
		nmax += 1024;
	}

	for (size_t i = 0; i < vsi.size() - 1; i++)
	{
		int i1 = vsi[i].iCharPos;
		int i2 = vsi[i + 1].iCharPos;
		int nlen = i2 - i1;

		render_item ri(vsi[i].a, const_cast<wchar_t*>(pch)+i1, nlen);
		ri.break_();
		m_output(ri);
	}
}

// font_binder ----------------------------------------------------------------

font_binder::font_binder(native_font_impl *f) : m_base_font(f)
{

}

void font_binder::set_output_function(output_function output)
{
	m_output = output;
}

void font_binder::operator()(render_item ri)
{
	ri.set_font(m_base_font);
	m_output(ri);
}

// image_binder ---------------------------------------------------------------

void image_binder::set_output_function(output_function output)
{
	m_output = output;
}

void image_binder::operator()(render_item ri)
{
	m_output(ri);
}

// line_breaker ---------------------------------------------------------------

line_breaker::line_breaker(HDC hdc, uint32_t width)
	: m_hdc(hdc)
	, m_width(width)
	, m_axpx(0)
	, m_aypx(0)
	, m_dy(0)
	, m_ach(0)
	, m_act(0)
{

}

void line_breaker::set_output_function(output_function output)
{
	m_output = output;
}

void line_breaker::operator()(render_item ri)
{
	assert(0 < m_width);

	uint32_t type = ri.get_type();

	while (!ri.is_empty())
	{
		if (type == text_item::string)
		{
			ri.shape(m_hdc);
			ri.place(m_hdc);
		}
		uint32_t item_width = ri.get_width();
		uint32_t remaining_width = m_width - m_axpx;

		// 行内に追加できる場合
		if (item_width <= remaining_width)
		{
			output(std::move(ri));

			m_axpx += item_width;

			// 改行する場合
			if (item_width == remaining_width) break_line();
			return;
		}

		uint32_t nsb = ri.query_softbreak_slength_by_width(remaining_width);
		if (nsb) // 残り幅以内で分割できる場合
		{
			render_item front = ri.trim_front_by_slength(m_hdc, nsb);
			output(std::move(front));
			break_line();
			continue;
		}

		// ここに来たということは、改行しないと対処できない
		break_line();

		if (item_width <= m_width) continue; // 改行すれば分割しなくて済む場合

		nsb = ri.query_softbreak_slength_by_width(m_width);
		if (nsb) // 改行すれば分割できる場合
		{
			render_item front = ri.trim_front_by_slength(m_hdc, nsb);
			output(std::move(front));
			break_line();
			continue;
		}

		nsb = ri.query_softbreak_slength_by_minimum();
		if (nsb) // 改行してもはみ出すが分割できる場合
		{
			render_item front = ri.trim_front_by_slength(m_hdc, nsb);
			output(std::move(front));
			break_line();
			continue;
		}

		// 分割できない場合
		output(std::move(ri));
		break_line();
		return;
	}
}

void line_breaker::break_line()
{
	output(render_item(text_item::line_end));
	m_axpx = 0;
	m_aypx += m_dy;
	m_dy = 0;
}

void line_breaker::output(render_item &&ri)
{
	m_dy = std::max(m_dy, ri.get_height()); // 行高さ

	uint32_t cch = ri.get_slength();
	uint32_t cct = ri.get_ctlength();

	ri.set_position(point_int(m_axpx, m_aypx));
	ri.set_ch_offset(m_ach);
	ri.set_ct_offset(m_act);

	m_output(ri);

	m_ach += cch;
	m_act += cct;
}




















#endif // WORDRING_WS_WIN -----------------------------------------------------
