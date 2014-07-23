/**
 * @file    3AxisEmulator/main.cpp
 *
 * @brief   3軸CNCエミュレータ
 *
 * @details
 *          CNCソフトウェア開発用のツールとして仮想的なCNCマシンとして
 *          エミュレータを準備します。
 *          開発の段階が進むにつれて、シミュレータとしての機能が追加されると
 *          良いと思います。
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#include <wordring/debug.h>

#include <wordring/gui/window.h>
#include <wordring/gui/window_service.h>
#include <wordring/gui/container.h>

#include <Windows.h>
#include <gl/GL.h>

//#include <cstdint>
//#include <iostream>

int main()
{
	using namespace wordring::gui;

	wordring::debug::debug_memory_leak();

	window_service ws;

	form f;
	f.on_paint = [](canvas& cv)
	{
		cv.draw_string(L"テスト文字列", point_int(30, 30));
	};
	f.on_destroy = [&]() { ws.quit(); };

	f.set_size(size_int(500, 500));
	f.set_position(point_int(0, 0));
	f.set_title(std::wstring(L"3AxisEmulator"));

	f.show();

	ws.run();

	return 0;
}
