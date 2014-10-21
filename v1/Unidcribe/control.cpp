#include "control.h"

// text -----------------------------------------------------------------------

void text::set_string(std::wstring s)
{
	m_string = s;
	m_storage.clear();
}

wchar_t const* text::get_sdata() const
{
	return m_string.data();
}

void text::insert_string(uint32_t ipoint, std::wstring s)
{
	m_string.insert(ipoint, s);
}

void text::erase_string(uint32_t ipoint, uint32_t slength)
{
	m_string.erase(ipoint, slength);
}

uint32_t text::get_slength() const
{
	return m_string.length();
}

void text::clear()
{
	m_storage.clear();
}

void text::push_back(item const &i)
{
	m_storage.push_back(i);
}

void text::assign(const_iterator it1, const_iterator it2)
{
	m_storage.clear();
	m_storage.insert(begin(), it1, it2);
}

text::iterator text::begin()
{
	return m_storage.begin();
}

text::const_iterator text::begin() const
{
	return m_storage.begin();
}

text::iterator text::end()
{
	return m_storage.end();
}

text::const_iterator text::end() const
{
	return m_storage.end();
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

		render_item ri(vsi[i].a, const_cast<wchar_t*>(pch) + i1, nlen);
		ri.break_();
		m_output(ri);
	}
}

// font_binder ----------------------------------------------------------------

font_binder::font_binder(font::store f) : m_base_font(f)
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
		if (type == item::string)
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
	output(render_item(item::line_end));
	m_axpx = 0;
	m_aypx += m_dy;
	m_dy = 0;
}

void line_breaker::output(render_item &&ri)
{
	m_dy = std::max(m_dy, ri.get_height()); // 行高さ
	
	uint32_t cch = ri.get_slength();
	uint32_t cct = ri.get_ctlength();

	ri.set_position(m_axpx, m_aypx);
	ri.set_ch_offset(m_ach);
	ri.set_ct_offset(m_act);

	m_output(ri);

	m_ach += cch;
	m_act += cct;
}

// text_render ----------------------------------------------------------------

text_render::text_render(SCRIPT_DIGITSUBSTITUTE *sds, text *txt)
	: m_text(txt)
	, m_sds(sds)
	, m_control(nullptr)
	, m_width(0)
	, m_height(0)
	, m_nct(0)
{
}

void text_render::set_control(control *c)
{
	m_control = c;
}

void text_render::parse(HDC hdc)
{
	m_storage.clear();

	item_breaker ib(m_sds);
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

void text_render::set_size(HDC hdc, uint32_t cx, uint32_t cy)
{
	m_width = cx;
	m_height = cy;
	parse(hdc);

	point pt = find_pt_by_nct(m_nct);

	// ウィンドウを最小化→復元で失敗する
	// おそらく、最小化でキャレット非表示、復元でキャレット表示の必要がある
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	//assert(ret != 0);
}

uint32_t text_render::get_slength() const
{
	return m_text->get_slength();
}

uint32_t text_render::get_clength() const
{
	uint32_t result = 0;
	for (render_item const &ri : m_storage)
	{
		result += ri.get_clength();
	}
	return result;
}

uint32_t text_render::get_ctlength() const
{
	uint32_t nct = 0;
	for (render_item const &ri : m_storage) nct += ri.get_ctlength();
	return nct;
}

// キャレット -------------------------------------------------------------

void text_render::set_caret_position(uint32_t ict)
{
	m_nct = ict;
	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
}

void text_render::move_caret_left()
{
	if (0 < m_nct) --m_nct;

	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
}


void text_render::move_caret_right()
{
	if (m_nct < get_ctlength()) ++m_nct;
	else
	{
		// コントロールの終わりに達しているので、親に通知する
	}
	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
}

void text_render::move_caret_up()
{

	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
}

void text_render::move_caret_down()
{

	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
}

void text_render::move_caret_home()
{
	m_nct = 0;
	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
}
void text_render::move_caret_end()
{
	m_nct = get_ctlength();
	point pt = find_pt_by_nct(m_nct);
	BOOL ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
}

text_render::iterator text_render::find_item_from_ctlength(uint32_t nct)
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


text_render::iterator text_render::find_item_from_slength(uint32_t slength)
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

uint32_t text_render::query_slength_from_ctlength(uint32_t nct) const
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

uint32_t text_render::query_ctlength_from_slength(uint32_t slength) const
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

uint32_t text_render::get_nct(uint32_t xct, uint32_t yct) const
{
	uint32_t result = 0;
	uint32_t ayct = 0;
	for (render_item const &ri : m_storage)
	{
		if (ri.get_type() == item::line_end) ++ayct;
		if (ayct == yct) break;
		result += ri.get_ctlength();
	}
	return result + xct;
}

point text_render::find_pt_by_nct(uint32_t nct) const
{
	point pt(0, 0);
	uint32_t act = 0; // キャレット前進位置
	uint32_t dy = 0;  // 現在の行高さ
	for (render_item const &ri : m_storage)
	{
		if (act == nct) break;

		uint32_t cct = ri.get_ctlength(); // 注目描画アイテムのキャレット数

		// 改行を検出
		if(ri.get_type() == item::line_end)
		{
			pt.m_y += dy; // 行の高さを加算
			dy = 0;       // 行の高さをクリア
			pt.m_x = 0;   // x位置をクリア
			act += cct;   // 改行は0キャレット
			//assert(cct == 1);
			continue;
		}

		dy = std::max(dy, ri.get_height());

		// 描画アイテムのキャレット数を足しても前進位置があふれない場合
		if (act + cct <= nct)
		{
			act += cct; // キャレット位置を前進
			pt.m_x += ri.get_width(); // x位置を加算
			continue;
		}

		pt.m_x += ri.find_xpt_from_nct(nct - act);
		break;
	}
	return pt;
}

// xy位置からキャレット数を返す
uint32_t text_render::find_nct_by_pt(uint32_t xpt, uint32_t ypt) const
{
	uint32_t nct = 0;
	uint32_t dy = 0; // 行の高さ


	return 0;
}

uint32_t text_render::find_max_yct() const
{
	uint32_t result = 0;
	for (render_item const &ri : m_storage)
	{
		if (ri.get_type() == item::line_end) ++result;
	}
	return result;
}

uint32_t text_render::find_max_xct(uint32_t yct) const
{
	assert(yct <= find_max_yct());
	uint32_t result = 0;
	const_iterator it1 = begin() + find_index_by_yct(yct), it2 = end();
	while (it1 != it2)
	{
		if (it1->get_type() == item::line_end) break;
		result += it1->get_ctlength();
		++it1;
	}
	return result;
}

uint32_t text_render::find_index_by_yct(uint32_t yct) const
{
	assert(yct <= find_max_yct());
	uint32_t result = 0;
	uint32_t act = 0;
	for (render_item const &ri : m_storage)
	{
		if (act == yct) break;
		if (ri.get_type() == item::line_end) ++act;
		++result;
	}
	return result;
}

uint32_t text_render::find_xpt_from_xct(uint32_t xct, uint32_t yct) const
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

uint32_t text_render::find_xct_from_xpt(uint32_t xpt, uint32_t yct) const
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

uint32_t text_render::find_ypt_from_yct(uint32_t yct) const
{
	uint32_t result = 0;
	uint32_t dy = 0;
	uint32_t act = 0;
	for (render_item const &ri : m_storage)
	{
		if(act == yct) break;
		if (ri.get_type() == item::line_end)
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

uint32_t text_render::find_yct_from_ypt(uint32_t ypt) const
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

		if (ri.get_type() == item::line_end)
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

void text_render::paint(HDC hdc)
{
	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t dy = 0;
	for (render_item &ri : m_storage)
	{
		if (ri.get_type() == item::line_end)
		{
			y += dy;
			dy = 0;
			x = 0;
			continue;
		}
		ri.paint(hdc, x, y);
		x += ri.get_width();
		dy = std::max(dy, ri.get_height());
	}
}

void text_render::paint_under_line(HDC hdc, uint32_t ich, uint32_t nch)
{
	for (render_item &ri : m_storage)
	{
		ri.paint_under_line_if(hdc, ich, nch);
	}
}

void text_render::invalidate(HWND hwnd)
{
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.bottom = m_height;
	rc.right = m_width;
	::InvalidateRect(hwnd, &rc, TRUE);
}

void text_render::clear()
{
	m_text->clear();
	m_storage.clear();
}

void text_render::set_string(HDC hdc, std::wstring s)
{
	m_text->set_string(s);
	m_storage.clear();
}

void text_render::insert_string(HDC hdc, uint32_t ipoint, std::wstring s)
{
	m_text->insert_string(ipoint, s);
}

void text_render::erase_string(uint32_t ipoint, uint32_t slength)
{
	m_text->erase_string(ipoint, slength);
}

void text_render::assign(text::const_iterator it1, text::const_iterator it2)
{
	m_storage.insert(begin(), it1, it2);
}

void text_render::set_base_font(font::store f)
{
	m_base_font = f;
}

font::store text_render::get_base_font()
{
	return m_base_font;
}

text_render::iterator text_render::begin()
{
	return m_storage.begin();
}

text_render::const_iterator text_render::begin() const
{
	return m_storage.begin();
}

text_render::iterator text_render::end()
{
	return m_storage.end();
}

text_render::const_iterator text_render::end() const
{
	return m_storage.end();
}

void text_render::set_focus(HWND hwnd)
{
	int32_t h = m_base_font->m_logfont.lfHeight;
	text_render::iterator it = find_item_from_ctlength(m_nct);
	if (it != end()) h = it->get_height();

	BOOL ret = ::CreateCaret(hwnd, (HBITMAP)1, 2, h);
	assert(ret != 0);
	point pt = find_pt_by_nct(m_nct);
	ret = ::SetCaretPos(pt.m_x, pt.m_y);
	assert(ret != 0);
	ret = ::ShowCaret(hwnd);
	assert(ret != 0);
}

void text_render::kill_focus(HWND hwnd)
{
	BOOL ret = ::HideCaret(hwnd);
	assert(ret != 0);
	ret = ::DestroyCaret();
	assert(ret != 0);
}

void text_render::start_composition(HWND hwnd)
{
	iterator it = find_item_from_ctlength(m_nct);
	font::store f = (it != end()) ? it->get_font() : m_base_font;

	m_ime = std::make_unique<ime>(this, f);
	uint32_t ipoint = query_slength_from_ctlength(m_nct);
	m_ime->set_insert_point(ipoint);
}

void text_render::end_composition(HWND hwnd)
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

void text_render::process_composition(HWND hwnd, WPARAM wParam, LPARAM lParam)
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
		point pt = find_pt_by_nct(ict);
		m_ime->set_candidate_position(himc, pt.m_x, pt.m_y);
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

// control --------------------------------------------------------------------

control::control(SCRIPT_DIGITSUBSTITUTE *sds)
	: m_render(sds, &m_text)
{
	m_render.set_control(this);
}

control::~control()
{

}

text_render* control::get_render()
{
	return &m_render;
}

void control::set_string(HDC hdc, std::wstring s)
{
	m_render.set_string(hdc, s);
}


void control::set_base_font(font::store f)
{
	m_base_font = f;
	m_render.set_base_font(f);
}

void control::on_char(WPARAM wparam)
{
	//uint32_t nct = m_render.get_nct(m_xct, m_yct);
}

void control::on_l_button_down(uint32_t xpt, uint32_t ypt)
{
	//m_yct = m_render.find_yct_from_ypt(ypt);
	//m_xct = m_render.find_xct_from_xpt(xpt, m_yct);

	//xpt = m_render.find_xpt_from_xct(m_xct, m_yct);
	//ypt = m_render.find_ypt_from_yct(m_yct);
	//BOOL ret = ::SetCaretPos(xpt, ypt);
	//assert(ret != 0);
}




