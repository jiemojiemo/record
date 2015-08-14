#include "opengl_part.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <list>
using std::list;

GLuint g_width = 800;				//窗口的宽度
GLuint g_height = 600;				//窗口的高度
bool g_dataReady =false;			//音频数据是否准备好给opengl绘制
list<SWaves> g_waveList;			//存放一堆音波数据

int OpenGLDisplay( void* data )
{
	int argc = 1;
	char *argv[] = {"Display"};

	//init
	glutInit( &argc, argv );
	//设置显示模式
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	//设置窗口大小
	glutInitWindowSize( g_width, g_height );
	//窗口起始位置
	glutInitWindowPosition( 100, 100 );
	//创建窗口
	glutCreateWindow( "Display" );
	//设置全局的回调函数（每时每刻都在回调）
	glutIdleFunc( IdelFunction );
	//设置回调函数
	glutDisplayFunc( DisplayFunction );
	//设置窗口变化函数
	glutReshapeFunc( ReshapeFunction );
	//进行一些初始化
	InitializeGraphics();

	//进入opengl时间循环
	glutMainLoop();

	return 0;
}

int AudioPlay( void* data )
{

	return 0;
}




//******************************************************
//				OpenGL Part
//******************************************************


/*******************************
**（函数名）InitializeGraphics：
**（功能用途）：初始化函数
**（返回值）return： 
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
void InitializeGraphics()
{
	//设置背景颜色
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	//开启深度检测
	glEnable( GL_DEPTH_TEST );

	glutPostRedisplay();
}
/*******************************
**（函数名）DisplayFunction：
**（功能用途）：回调函数
**（返回值）return： 
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
void DisplayFunction()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if( g_dataReady )
	{
		//进行绘制
		DrawWaves();
		//将音频数据存放在list中，方便绘制
		if( g_waveList.size() <= g_depth )
		{
			if( g_waveList.size() == g_depth )
				g_waveList.pop_back();
			g_waveList.push_front( g_audioBuffer );
		}
	}
	glutSwapBuffers();
}

/*******************************
**（函数名）KeyboradFunction：
**（功能用途）：键盘回调函数
**（返回值）return： 
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
void KeyboradFunction( unsigned char key, int x, int y )
{
	switch( key )
	{
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
}

/*******************************
**（函数名）ReshapeFunction：
**（功能用途）：窗口变化回调函数
**（返回值）return： 
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
void ReshapeFunction( int w, int h )
{
	if( 0 == h )
		h = 1;

	glViewport( 0,0,w,h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 45.0, (GLfloat) w / (GLfloat) h, 1.0, 100.0 );
	//glOrtho( -1.0, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );

	gluLookAt( 0.0f, 3.0f, 2.5f , 
		0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

/*******************************
**（函数名）IdelFunction：
**（功能用途）：全局回调
**（返回值）return： 
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
void IdelFunction()
{
	glutPostRedisplay();
}