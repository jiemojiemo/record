#include "opengl_part.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <list>
using std::list;

GLuint g_width = 800;				//���ڵĿ��
GLuint g_height = 600;				//���ڵĸ߶�
bool g_dataReady =false;			//��Ƶ�����Ƿ�׼���ø�opengl����
list<SWaves> g_waveList;			//���һ����������

int OpenGLDisplay( void* data )
{
	int argc = 1;
	char *argv[] = {"Display"};

	//init
	glutInit( &argc, argv );
	//������ʾģʽ
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	//���ô��ڴ�С
	glutInitWindowSize( g_width, g_height );
	//������ʼλ��
	glutInitWindowPosition( 100, 100 );
	//��������
	glutCreateWindow( "Display" );
	//����ȫ�ֵĻص�������ÿʱÿ�̶��ڻص���
	glutIdleFunc( IdelFunction );
	//���ûص�����
	glutDisplayFunc( DisplayFunction );
	//���ô��ڱ仯����
	glutReshapeFunc( ReshapeFunction );
	//����һЩ��ʼ��
	InitializeGraphics();

	//����openglʱ��ѭ��
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
**����������InitializeGraphics��
**��������;������ʼ������
**������ֵ��return�� 
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
*******************************/
void InitializeGraphics()
{
	//���ñ�����ɫ
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	//������ȼ��
	glEnable( GL_DEPTH_TEST );

	glutPostRedisplay();
}
/*******************************
**����������DisplayFunction��
**��������;�����ص�����
**������ֵ��return�� 
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
*******************************/
void DisplayFunction()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if( g_dataReady )
	{
		//���л���
		DrawWaves();
		//����Ƶ���ݴ����list�У��������
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
**����������KeyboradFunction��
**��������;�������̻ص�����
**������ֵ��return�� 
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
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
**����������ReshapeFunction��
**��������;�������ڱ仯�ص�����
**������ֵ��return�� 
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
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
**����������IdelFunction��
**��������;����ȫ�ֻص�
**������ֵ��return�� 
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
*******************************/
void IdelFunction()
{
	glutPostRedisplay();
}