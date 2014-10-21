#include "ime.h"
#include "control.h"


ime::ime(text_render *render, font::store  f)
	: m_render(render)
	, m_font(f)
{

}

ime::~ime()
{
}

void ime::set_insert_point(uint32_t ipoint)
{
	m_insert_point = ipoint;
}

uint32_t ime::get_insert_point() const
{
	return m_insert_point;
}

uint32_t ime::get_slength() const
{
	return m_slength;
}


uint32_t ime::get_cursol_position(HIMC himc) const
{
	LONG ret = ImmGetCompositionString(himc, GCS_CURSORPOS, NULL, 0);
	assert(ret != IMM_ERROR_NODATA && ret != IMM_ERROR_GENERAL);
	return  ret & 0xFFFF;
}

uint32_t ime::get_delta(HIMC himc) const
{
	LONG ret = ImmGetCompositionString(himc, GCS_DELTASTART, NULL, 0);
	assert(ret != IMM_ERROR_NODATA && ret != IMM_ERROR_GENERAL);
	return  ret & 0xFFFF;
}

void ime::set_candidate_position(HIMC himc, uint32_t x, uint32_t y)
{
	LPLOGFONT f = &(m_font->m_logfont);
	BOOL ret = ImmSetCompositionFont(himc, f);
	assert(ret != 0);

	CANDIDATEFORM cf = { 0 };
	cf.ptCurrentPos.x = x;
	cf.ptCurrentPos.y = y;
	cf.dwStyle = CFS_CANDIDATEPOS;  // 候補文字ウィンドウの位置指定をする

	if (himc != NULL)
	{
		// 候補文字ウィンドウの位置調整を行う
		ImmSetCandidateWindow(himc, &cf);
	}
}

std::wstring ime::get_composition_string(HIMC himc)
{
	LONG nbyte = ::ImmGetCompositionString(himc, GCS_COMPSTR, NULL, 0);
	assert(nbyte != IMM_ERROR_NODATA && nbyte != IMM_ERROR_GENERAL);

	uint32_t nbuf = nbyte / sizeof(wchar_t);

	// バッファを確保し、文字列を得る
	std::unique_ptr<wchar_t[]> buf = std::make_unique<wchar_t[]>(nbuf);
	LONG ret = ::ImmGetCompositionString(himc, GCS_COMPSTR, buf.get(), nbyte);
	assert(ret != IMM_ERROR_NODATA && ret != IMM_ERROR_GENERAL);

	m_slength = nbuf;

	return std::wstring(buf.get(), nbuf);
}

void ime::get_attribute(HIMC himc)
{
	LONG nbyte = ::ImmGetCompositionString(himc, GCS_COMPATTR, NULL, 0);
	assert(nbyte != IMM_ERROR_NODATA && nbyte != IMM_ERROR_GENERAL);

	m_attributes.resize(nbyte);
	LONG ret = ::ImmGetCompositionString(himc, GCS_COMPATTR, m_attributes.data(), nbyte);
	assert(ret != IMM_ERROR_NODATA && ret != IMM_ERROR_GENERAL);

	// ATTR_INPUT               選択されていない変換されていない文字
	// ATTR_TARGET_CONVERTED    選択されている変換されている文字
	// ATTR_CONVERTED           選択されていない変換されている文字
	// ATTR_TARGET_NOTCONVERTED 選択されている変換されていない文字
	// ATTR_INPUT_ERROR         無効な文字
}


void ime::get_clause(HIMC himc)
{
	LONG nbyte = ::ImmGetCompositionString(himc, GCS_COMPCLAUSE, NULL, 0);
	assert(nbyte != IMM_ERROR_NODATA && nbyte != IMM_ERROR_GENERAL);

	m_clauses.resize(nbyte / sizeof(uint32_t));
	LONG ret = ::ImmGetCompositionString(himc, GCS_COMPCLAUSE, m_clauses.data(), nbyte);
	assert(ret != IMM_ERROR_NODATA && ret != IMM_ERROR_GENERAL);
}

std::wstring ime::get_result_string(HIMC himc)
{
	// バッファのバイト数を調査
	LONG nch = ::ImmGetCompositionString(himc, GCS_RESULTSTR, NULL, 0);
	assert(nch != IMM_ERROR_NODATA && nch != IMM_ERROR_GENERAL);
	uint32_t nbuf = nch / sizeof(wchar_t);

	// バッファを確保し、文字列を得る
	std::unique_ptr<wchar_t[]> buf = std::make_unique<wchar_t[]>(nbuf);
	LONG ret = ::ImmGetCompositionString(himc, GCS_RESULTSTR, buf.get(), nch);
	assert(nch != IMM_ERROR_NODATA && nch != IMM_ERROR_GENERAL);

	m_slength = nbuf;

	return std::wstring(buf.get(), nbuf);
}




















