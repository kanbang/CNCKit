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

	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	std::cout << sizeof(window);

	window_service ws;

	container_window w, w2;
	button_window b;
	form f;

	w.on_create = [&](){


		w2.create(NULL);

		w2.set_parent(&w);
		w2.close();


	};

	w2.on_create = [&](){
		b.create(&w2);
		//b.set_parent(&w2);
		b.set_size(size_int(100, 160));
		b.on_click = [](){
			std::cout << "click!" << std::endl;
		};

	};
	w.create(NULL);
	w.set_size(size_int(500, 500));
	w.set_position(point_int(500, 10));

	w.get_position();



	ws.run();

	//std::cout << sizeof(int32_t);
	//new int;

	return 0;
}
