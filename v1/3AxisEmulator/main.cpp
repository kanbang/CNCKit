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

#include <wordring/opengl/gl_canvas.h>
#include <wordring/opengl/gl_context.h>

#include <Windows.h>

#include <GL/glew.h>
#include <gl/GL.h>
#include <GL/GLU.h>
//#include <cstdint>
#include <iostream>

int main()
{
	using namespace wordring::gui;
	using namespace wordring::opengl;

	wordring::debug::debug_memory_leak();

	window_service ws;

	form f;
	gl_context ctx;

		f.on_create = [&]()
		{
			ctx.assign(f, gl_context::flag::WINDOW, 24, 24);
			//ctx.make_current();
			//ctx.unmake_current();
		};

		f.on_size = [&](size_int size)
		{
			int i = 0;
		};

		f.on_paint = [&](canvas& cv)
		{
			size_int size = f.get_size();
			double dx = double(size.cx) / size.cy;
			ctx.make_current(cv);

			glViewport(0, 0, size.cx, size.cy);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-dx, dx, -1, 1, 0, 3);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
				1.0, 1.0, 1.0,
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBegin(GL_LINES);

			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 3.0f);

			//glColor3f(1.0f, 0.0f, 0.0f); // 赤
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 3.0f, 0.0f);

			//glColor3f(0.0f, 1.0f, 0.0f); // 緑
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(3.0f, 0.0f, 0.0f);
			glPopMatrix();

			// 座標線
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 0.0f); // 黄

			// xz
			glVertex3f(0.5f, 0.0f, 0.3f);
			glVertex3f(0.5f, 0.5f, 0.3f);
			// yz
			glVertex3f(0.0f, 0.5f, 0.3f);
			glVertex3f(0.5f, 0.5f, 0.3f);
			// xy
			glVertex3f(0.5f, 0.5f, 0.0f);
			glVertex3f(0.5f, 0.5f, 0.3f);


			glPopMatrix();

			glEnd();

			glFlush();

			GLUquadricObj* obj = gluNewQuadric();
			glPushMatrix();
			glTranslatef(0.5f, 0.5f, 0.3f);
			glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

			glColor3f(0.5f, 0.5f, 0.5f);
			gluQuadricDrawStyle(obj, GLU_FILL);
			gluQuadricNormals(obj, GLU_SMOOTH);
			gluQuadricTexture(obj, GLU_FALSE);
			gluQuadricOrientation(obj, GLU_OUTSIDE);
			gluCylinder(obj, 0.02, 0.02, 0.2, 8, 4);
			glPopMatrix();

			gluDeleteQuadric(obj);
			
			//glVertex2d(0.9, -0.9);
			//glVertex2d(0.9, 0.9);
			//glVertex2d(-0.9, 0.9);


			ctx.unmake_current(cv);
			cv.draw_string(L"テスト文字列", point_int(30, 30));
		};
		f.on_destroy = [&]() { ws.quit(); };

		f.set_parent(nullptr);

		f.set_size(size_int(640, 480));
		f.set_position(point_int(0, 0));
		f.set_title(std::wstring(L"3AxisEmulator"));

		//control* c = &f;
		//c->set_size(size_int(100, 100));

		f.show_window();

		rect_int rc = f.get_rect();
		ws.run();

	try
	{	}
	catch (std::runtime_error const& e)
	{
		std::cout << e.what() << std::endl;
	}



	return 0;
}
