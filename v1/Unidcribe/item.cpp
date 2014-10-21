#include "item.h"





// rect -----------------------------------------------------------------------

rect::rect()
	: m_x(0)
	, m_y(0)
	, m_cx(0)
	, m_cy(0)
{

}

rect::rect(uint32_t x, uint32_t y, uint32_t cx, uint32_t cy)
	: m_x(x)
	, m_y(y)
	, m_cx(cx)
	, m_cy(cy)
{

}

// font -----------------------------------------------------------------------

font::font() : m_cache(nullptr), m_hfont(nullptr), m_color(0), m_height(0)
{

}

font::font(uint32_t height, std::wstring face)
	: m_cache(nullptr), m_hfont(nullptr), m_color(0), m_height(height)
{
	m_hfont = create(height, face);
	// Meiryo は、内部レディングの分サイズがおかしい
	// 内部レディングを測る
	HDC hdc = ::GetDC(NULL);
	::SetMapMode(hdc, MM_TEXT);
	assert(hdc);
	HGDIOBJ old = ::SelectObject(hdc, m_hfont);
	TEXTMETRIC tm;
	BOOL ret = ::GetTextMetrics(hdc, &tm);
	assert(ret != 0);
	::SelectObject(hdc, old);
	// 内部レディングが0以外の場合、フォントを消して再作成する
	if (tm.tmInternalLeading != 0)
	{
		// (フォントの高さに対する内部レディングの割合) + 1
		float dh = float(tm.tmInternalLeading) / tm.tmHeight + 1;

		BOOL ret = ::DeleteObject(m_hfont);
		assert(ret != 0);
		m_hfont = create(LONG(height * dh), face);
	}
	::ReleaseDC(NULL, hdc);
}

font::~font()
{
	if (m_cache) ::ScriptFreeCache(&m_cache);
	if (m_hfont) ::DeleteObject(m_hfont);
}

HFONT font::create(uint32_t height, std::wstring face)
{
	assert(face.length() < LF_FACESIZE);

	m_logfont.lfHeight = height;
	m_logfont.lfWidth = 0;
	m_logfont.lfEscapement = 0;
	m_logfont.lfOrientation = 0;
	m_logfont.lfWeight = FW_REGULAR;
	m_logfont.lfItalic = FALSE;
	m_logfont.lfUnderline = FALSE;
	m_logfont.lfStrikeOut = FALSE;
	m_logfont.lfCharSet = DEFAULT_CHARSET;
	m_logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logfont.lfQuality = DEFAULT_QUALITY;
	m_logfont.lfPitchAndFamily = FF_DONTCARE;
	std::copy(face.begin(), face.end(), m_logfont.lfFaceName);
	m_logfont.lfFaceName[face.length()] = 0;
	/*
	HFONT hfont = ::CreateFontW(
		height, 0, 0, 0,
		FW_REGULAR, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FF_DONTCARE, face.c_str());
		*/
	HFONT hfont = CreateFontIndirectW(&m_logfont);
	assert(hfont);
	return hfont;
}

// font_span ------------------------------------------------------------------

font_span::font_span(font::store f, uint32_t l) : m_font(f), m_length(l)
{

}

// item -----------------------------------------------------------------------

item::item(uint32_t type)
	: m_analysis({ 0 })
	, m_data(nullptr)
	, m_type(type)
	, m_slength(0)
{

}

item::item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength)
	: m_analysis(sa)
	, m_data(data)
	, m_type(string)
	, m_slength(slength)
{

}

item::item(item const &rhs)
	: m_analysis(rhs.m_analysis)
	, m_data(rhs.m_data)
	, m_type(rhs.m_type)
	, m_slength(rhs.m_slength)
{

}

uint32_t item::get_type() const
{
	return m_type;
}

bool item::is_empty() const
{
	return m_slength == 0;
}

SCRIPT_ANALYSIS* item::get_analysis()
{
	return &m_analysis;
}

SCRIPT_ANALYSIS const* item::get_analysis() const
{
	return &m_analysis;
}

void* item::get_data()
{
	return m_data;
}

uint32_t item::get_slength() const
{
	//assert(m_type == string);
	return m_slength;
}

item item::trim_front_by_slength(uint32_t slength)
{
	assert(m_type == string);
	wchar_t *p = m_data;
	m_data = static_cast<wchar_t*>(m_data) + slength;
	m_slength -= slength;
	return item(m_analysis, p, slength);
}

// render_item ----------------------------------------------------------------

render_item::render_item(uint32_t type)
	: item(type)
	, m_abc({ 0 })
	, m_xpt(0)
	, m_ypt(0)
	, m_line_height(0)
	, m_ch_offset(0)
	, m_ct_offset(0)
{

}

render_item::render_item(item const &rhs)
	: item(rhs)
	, m_abc({ 0 })
	, m_xpt(0)
	, m_ypt(0)
	, m_line_height(0)
	, m_ch_offset(0)
	, m_ct_offset(0)
{
	m_log_cluster.resize(get_slength());
}

render_item::render_item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength)
	: item(sa, data, slength)
	, m_abc({ 0 })
	, m_xpt(0)
	, m_ypt(0)
	, m_line_height(0)
	, m_ch_offset(0)
	, m_ct_offset(0)
{
	m_log_cluster.resize(slength);
}

render_item::render_item(render_item &&rhs)
	: item(rhs)
	, m_glyphs(std::forward<std::vector<WORD> >(rhs.m_glyphs))
	, m_advances(std::forward<std::vector<int> >(rhs.m_advances))
	, m_justifies(std::forward<std::vector<int> >(rhs.m_justifies))
	, m_goffsets(std::forward<std::vector<GOFFSET> >(rhs.m_goffsets))
	, m_log_cluster(std::forward<std::vector<WORD> >(rhs.m_log_cluster))
	, m_abc(rhs.m_abc)
	, m_visattr(std::forward<std::vector<SCRIPT_VISATTR> >(rhs.m_visattr))
	, m_logattr(std::forward<std::vector<SCRIPT_LOGATTR> >(rhs.m_logattr))
	, m_font(rhs.m_font)
	, m_xpt(rhs.m_xpt)
	, m_ypt(rhs.m_ypt)
	, m_line_height(rhs.m_line_height)
	, m_ch_offset(rhs.m_ch_offset)
	, m_ct_offset(rhs.m_ct_offset)
{
	rhs.m_abc = { 0 };
	rhs.m_xpt = 0;
	rhs.m_ypt = 0;
	rhs.m_line_height = 0;
	rhs.m_ch_offset = 0;
	rhs.m_ct_offset = 0;
}

void render_item::set_position(uint32_t xpt, uint32_t ypt)
{
	m_xpt = xpt;
	m_ypt = ypt;
}

void render_item::set_line_height(uint32_t cy)
{
	m_line_height = cy;
}

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
	case item::string: return get_clength();
	case item::line_end: return 0;
	case item::image:  return 1;
	}
	assert(false); // 他のタイプを追加した場合、この関数もエントリを追加する
	return 0;
}

uint32_t render_item::get_height() const
{
	uint32_t height = 0;
	if (m_font) height = m_font->m_logfont.lfHeight;
	return height;
}

uint32_t render_item::get_width() const
{
	uint32_t b = m_abc.abcB;
	if (0xFFFFFFF0 < b)
	{
		b = m_font->m_height * 4;
	}
	assert(0 <= m_abc.abcA + b + m_abc.abcC);
	return m_abc.abcA + b + m_abc.abcC;
}

void render_item::set_font(font::store f)
{
	m_font = f;
}

render_item render_item::trim_front_by_slength(HDC hdc, uint32_t slength)
{
	assert(0 < slength && slength < get_slength());

	render_item result = item::trim_front_by_slength(slength);
	result.set_font(get_font());
	result.shape(hdc);
	result.place(hdc);

	return result;
}

// キャレット移動数から文字数を返す
uint32_t render_item::query_slength_from_ctlength(uint32_t nct) const
{
	assert(get_type() != item::line_end);

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
	assert(get_type() != item::line_end);
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
	assert(get_type() != item::line_end);

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
	assert(get_type() != item::line_end);
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

void render_item::break_()
{
	m_logattr.resize(get_slength());
	LRESULT ret = ::ScriptBreak(
		static_cast<wchar_t*>(get_data()), get_slength(),
		get_analysis(),
		m_logattr.data()); // [out]
	assert(ret == 0);
}

void render_item::shape(HDC hdc)
{
	assert(m_font);

	HRESULT ret = 0;
	::SelectObject(hdc, m_font->m_hfont);
	// バッファが足りるまで大きさを増やしてリトライ
	// バッファの大きさについては、ScriptShape()の解説に説明がある
	size_t nchar = get_slength();
	for (size_t i = 0; i < 10; i++)
	{
		set_glength(size_t((nchar * 1.5f + 16) * (1.0f + i / 5.0f)));
		int nresult = 0;

		ret = ::ScriptShape(
			hdc, &(m_font->m_cache),
			static_cast<wchar_t*>(get_data()), get_slength(),
			get_glength(),
			get_analysis(),
			m_glyphs.data(),
			m_log_cluster.data(),
			m_visattr.data(),
			&nresult);
		set_glength(nresult);

		if (ret == USP_E_SCRIPT_NOT_IN_FONT)
		{
			uint32_t i = 0;
		}
		if (ret == 0) break;
	}
}

void render_item::place(HDC hdc)
{
	LRESULT ret = ::ScriptPlace(
		hdc, &(m_font->m_cache),
		m_glyphs.data(), get_glength(),
		m_visattr.data(),
		get_analysis(),
		m_advances.data(), // [out]
		m_goffsets.data(), // [out]
		&m_abc);           // [out]
	assert(ret == 0);
}

// グリフ間隔の調整によって表示幅をdxにする
void render_item::justify(uint32_t dx)
{
	HRESULT ret = ::ScriptJustify(
		m_visattr.data(),
		m_advances.data(),
		get_glength(),
		dx,
		2,
		m_justifies.data());
	assert(ret == 0);
}

void render_item::paint(HDC hdc, int32_t x, int32_t y)
{
	::SelectObject(hdc, m_font->m_hfont);

	HRESULT ret = ::ScriptTextOut(
		hdc,
		&m_font->m_cache,
		m_xpt, m_ypt,
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

	::MoveToEx(hdc, xpt1, m_ypt + get_height(), NULL);
	::LineTo(hdc, xpt2, m_ypt);

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

/*
item_storage::range_type item_storage::get_line(uint32_t n)
{
	return range_type();
}

item_storage::const_range_type item_storage::get_line(uint32_t n) const
{
	return range_type();
}
*/








