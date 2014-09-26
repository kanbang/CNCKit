/**
 * @file    wordring/gui/container.cpp
 *
 * @brief   GUIコンテナの実装ファイル
 *
 * @details
 *          フォームなどのGUIコンテナの基礎を定義します。
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

#include <wordring/gui/window_service.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>

#include <cassert>
#include <memory>

using namespace wordring::gui;

// 構築・破棄 -----------------------------------------------------------------

container::container(rect_int rc) : control(rc, flow_layout::create())
{
}

container::container(rect_int rc, layout::store l)
	: control(rc, std::move(l))
{
}

container::~container()
{
}

control::store container::create(rect_int rc)
{
	return control::store(new container(rc));
}

control::store container::create(rect_int rc, layout::store l)
{
	return control::store(new container(rc, std::move(l)));
}

// 情報 -----------------------------------------------------------------------

wchar_t const* container::get_control_name() const
{
	return L"wordring::gui::container";
}

bool container::is_container() const
{
	return true;
}

// 親子関係 -------------------------------------------------------------------

// 描画 -----------------------------------------------------------------------

// 大きさ・位置 ---------------------------------------------------------------

// マウス・メッセージ ---------------------------------------------------------

// キーボード・メッセージ -----------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

// test_container -------------------------------------------------------------

test_container::test_container(rect_int rc) : container(rc)
{

}

test_container::~test_container()
{

}

control::store test_container::create(rect_int rc)
{
	return control::store(new test_container(rc));
}

void test_container::do_paint(canvas &cv)
{
	rect_int rc(point_int(0, 0), get_size());
	cv->fill_rect(rc, color_rgb(0xA0, 0xF0, 0xF0));
}
