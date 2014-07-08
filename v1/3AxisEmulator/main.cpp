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

#include <crtdbg.h>


#include <Windows.h>
#include <gl/GL.h>

#include <cstdint>
#include <iostream>

#include <wordring/gui/window.h>
#include <wordring/gui/window_service.h>
#include <wordring/gui/container.h>


int main()
{
	using namespace wordring::gui;
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif // _DEBUG

	std::cout << sizeof(window);

	window_service ws;

	//container_window w, w2;
	//button_window b;
	form f;




	ws.run();

	//std::cout << sizeof(int32_t);
	//new int;

	return 0;
}
