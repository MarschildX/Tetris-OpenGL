/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - ����������ǲο����룬����Ҫ����ο���ҵ˵��������˳������ɡ�
 * - ��������OpenGL�����������������У���ο���һ��ʵ��γ�����ĵ���
 *
 * - ��ʵ�ֹ������£�
 * - 1) �������̸�͡�L���ͷ���
 * - 2) ������/��/�¼����Ʒ�����ƶ����ϼ���ת����
 *
 * - δʵ�ֹ������£�
 * - 1) ������ɷ��鲢���ϲ�ͬ����ɫ
 * - 2) ������Զ������ƶ�
 * - 3) ����֮�����ײ���
 * - 4) ���̸���ÿһ�������֮���Զ�����
 * - 5) ����
 *
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

int starttime;			// ���Ʒ��������ƶ�ʱ��
int rotation = 0;		// ���Ƶ�ǰ�����еķ�����ת
vec2 tile[4];			// ��ʾ��ǰ�����еķ���
bool gameover = false;	// ��Ϸ�������Ʊ���
int xsize = 400;		// ���ڴ�С��������Ҫ�䶯���ڴ�С����
int ysize = 720;

int randomShape = 0;	// ���ѡ��ÿ�εķ�����״
bool isContinue = true; // ������Ϸ�Ƿ��������
GLuint program;

//// һ����ά�����ʾ���п��ܳ��ֵķ���ͷ���
//vec2 allRotationsLshape[4][4] =
//							  {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},	//   "L"
//							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1)},   //
//							   {vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
//							   {vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};

// һ����ά�����ʾ���п��ܳ��ֵ���״�ͷ���
vec2 allShapeAndRotation[7][4][4] = {
	{	
		{vec2(0, 0), vec2(1, 0), vec2(1, -1), vec2(0, -1)},		// O
		{vec2(0, 0), vec2(1, 0), vec2(1, -1), vec2(0, -1)},
		{vec2(0, 0), vec2(1, 0), vec2(1, -1), vec2(0, -1)},
		{vec2(0, 0), vec2(1, 0), vec2(1, -1), vec2(0, -1)}
	},

	{	
		{vec2(0, 0), vec2(-1, 0), vec2(-2, 0), vec2(1, 0)},		// I
		{vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(0, -2)},
		{vec2(0, 0), vec2(-1, 0), vec2(-2, 0), vec2(1, 0)},
		{ vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(0, -2) }
	},

	{	
		{vec2(0, 0), vec2(1, 0), vec2(0, -1), vec2(-1, -1)},	// S
		{vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, -1)},
		{ vec2(0, 0), vec2(1, 0), vec2(0, -1), vec2(-1, -1) },
		{ vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, -1) }
	},

	{	
		{vec2(0, 0), vec2(-1, 0), vec2(0, -1), vec2(1, -1)},	// Z
		{vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, -1)},
		{ vec2(0, 0), vec2(-1, 0), vec2(0, -1), vec2(1, -1) },
		{ vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, -1) }
	},

	{	
		{ vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1) },	// L
		{ vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1) },   
		{ vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0) },   
		{ vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1) }
	},

	{
		{vec2(0, 0), vec2(-1, 0), vec2(1, 0), vec2(1, -1)},		// J
		{vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(0, -1)},
		{vec2(0, 0), vec2(-1, 0), vec2(-1, 1), vec2(1, 0)},
		{vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(-1, -1)}
	},

	{
		{vec2(0, 0), vec2(-1, 0), vec2(1, 0), vec2(0, -1)},		// T
		{vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(0, -1)},
		{vec2(0, 0), vec2(-1, 0), vec2(0, 1), vec2(1, 0)},
		{vec2(0, 0), vec2(0, 1), vec2(-1, 0), vec2(0, -1)}
	}
};

// ���ƴ��ڵ���ɫ����
vec4 colors[7] = {
	vec4(1.0, 0.5, 0.0, 1.0), // orange
	vec4(1.0, 0.0, 0.0, 1.0), // red
	vec4(0.0, 1.0, 0.0, 1.0), // green
	vec4(0.0, 0.0, 1.0, 1.0), // blue
	vec4(1.0, 1.0, 0.0, 1.0), // yellow
	vec4(0.8, 0.0, 0.8, 1.0), // purple
	vec4(0.5, 0.6, 0.4, 1.0)  // graygreen
};
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white  = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);

// ��ǰ�����λ�ã������̸�����½�Ϊԭ�������ϵ��
vec2 tilepos = vec2(5, 19);

// ���������ʾ���̸��ĳλ���Ƿ񱻷�����䣬��board[x][y] = true��ʾ(x,y)�����ӱ���䡣
// �������̸�����½�Ϊԭ�������ϵ��
bool board[10][20];


// �����̸�ĳЩλ�ñ��������֮�󣬼�¼��Щλ���ϱ�������ɫ
vec4 boardcolours[1200];

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

void display();

//////////////////////////////////////////////////////////////////////////
// �޸����̸���posλ�õ���ɫΪcolour�����Ҹ��¶�Ӧ��VBO

void changecellcolour(vec2 pos, vec4 colour)
{
	// ÿ�������Ǹ������Σ��������������Σ��ܹ�6�����㣬�����ض���λ�ø����ʵ�����ɫ
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// ����ƫ���������ʵ���λ�ø�����ɫ
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// ��ǰ�����ƶ�������תʱ������VBO

void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ����Ϊ��һ���������ֵķ��顣����Ϸ��ʼ��ʱ�����������һ����ʼ�ķ��飬
// ����Ϸ������ʱ���жϣ�û���㹻�Ŀռ��������µķ��顣

void newtile()
{
	// ���·���������̸���������м�λ�ò�����Ĭ�ϵ���ת����
	tilepos = vec2(5 , 19);
	rotation = 0;

	/*����ʱ������������Ӳ�������������������ÿ�εķ�����״*/
	srand((unsigned)time(NULL));
	randomShape = rand();
	srand((unsigned)randomShape);
	randomShape = rand() % 7;

	for (int i = 0; i < 4; i++)
	{
		//tile[i] = allRotationsLshape[3][i];
		tile[i] = allShapeAndRotation[randomShape][0][i];
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
		if (board[x][y]) {
			isContinue = false;
		}
	}

	updatetile();

	// ���·��鸳����ɫ
	vec4 newcolours[24];
	for (int i = 0; i < 24; i++) {
		//newcolours[i] = orange;
		newcolours[i] = colors[randomShape];
	}
		

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��OpenGL��ʼ��

void init()
{
	// ��ʼ�����̸񣬰���64���������꣨�ܹ�32���ߣ�������ÿ������һ����ɫֵ
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// ������
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// ˮƽ��
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// �������߸��ɰ�ɫ
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// ��ʼ�����̸񣬲���û�б����ĸ������óɺ�ɫ
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// ��ÿ�����ӣ���ʼ��6�����㣬��ʾ���������Σ�����һ�������θ���
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

			boardpoints[6*(10*i + j)    ] = p1;
			boardpoints[6*(10*i + j) + 1] = p2;
			boardpoints[6*(10*i + j) + 2] = p3;
			boardpoints[6*(10*i + j) + 3] = p2;
			boardpoints[6*(10*i + j) + 4] = p3;
			boardpoints[6*(10*i + j) + 5] = p4;
		}

	// �����̸�����λ�õ�����������Ϊfalse��û�б���䣩
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// ������ɫ��
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// ���̸񶥵�
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ���̸�ÿ������
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// ���̸�ÿ�����Ӷ���λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸�ÿ�����Ӷ�����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ��ǰ����
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// ��Ϸ��ʼ��
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}

//////////////////////////////////////////////////////////////////////////
// �����cellposλ�õĸ����Ƿ��������Ƿ������̸�ı߽緶Χ�ڡ�

bool checkvalid(vec2 cellpos)
{
	int x = cellpos.x;
	int y = cellpos.y;
	if((cellpos.x >=0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20) && !board[x][y] ) /*����������Ƿ�ռ�õļ��*/
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// �����������㹻�ռ���������ת��ǰ����

void rotate()
{
	// ����õ���һ����ת����
	int nextrotation = (rotation + 1) % 4;

	// ��鵱ǰ��ת֮���λ�õ���Ч��
	if (checkvalid((allShapeAndRotation[randomShape][nextrotation][0]) + tilepos)
		&& checkvalid((allShapeAndRotation[randomShape][nextrotation][1]) + tilepos)
		&& checkvalid((allShapeAndRotation[randomShape][nextrotation][2]) + tilepos)
		&& checkvalid((allShapeAndRotation[randomShape][nextrotation][3]) + tilepos))
	{
		// ������ת������ǰ��������Ϊ��ת֮��ķ���
		rotation = nextrotation;
		for (int i = 0; i < 4; i++)
			tile[i] = allShapeAndRotation[randomShape][rotation][i]; /*�������������״�任����*/

		updatetile();
	}
}

//////////////////////////////////////////////////////////////////////////
// ������̸���������û�б������

void checkfullrow()
{
	for (int i = 0; i < 19; i++) {
		bool isFull = true;	// ��¼��ǰ���Ƿ�ռ��
		// ����Ƿ����ж���ռ��
		for (int j = 0; j < 10; j++) {
			if (!board[j][i]) {
				isFull = false;
				break;
			}
		}
		if (isFull) {
			// ��������ɫ���ݸ����Լ�ռ��״̬����
			for (int k = i; k < 19; k++) {
				for (int j = 0; j < 10; j++) {
					boardcolours[6 * (k * 10 + j) + 0] = boardcolours[6 * ((k + 1) * 10 + j) + 0];
					boardcolours[6 * (k * 10 + j) + 1] = boardcolours[6 * ((k + 1) * 10 + j) + 1];
					boardcolours[6 * (k * 10 + j) + 2] = boardcolours[6 * ((k + 1) * 10 + j) + 2];
					boardcolours[6 * (k * 10 + j) + 3] = boardcolours[6 * ((k + 1) * 10 + j) + 3];
					boardcolours[6 * (k * 10 + j) + 4] = boardcolours[6 * ((k + 1) * 10 + j) + 4];
					boardcolours[6 * (k * 10 + j) + 5] = boardcolours[6 * ((k + 1) * 10 + j) + 5];
					board[j][k] = board[j][k+1];
				}
			}
			// ��ʱ���̵�������һ���趨Ϊ��ɫ��ռ��״̬Ϊfalse
			for (int j = 0; j < 10; j++) {
				boardcolours[6 * (19 * 10 + j) + 0] = black;
				boardcolours[6 * (19 * 10 + j) + 1] = black;
				boardcolours[6 * (19 * 10 + j) + 2] = black;
				boardcolours[6 * (19 * 10 + j) + 3] = black;
				boardcolours[6 * (19 * 10 + j) + 4] = black;
				boardcolours[6 * (19 * 10 + j) + 5] = black;
				board[j][19] = false;
			}

			/*���°�����*/
			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 1200*sizeof(vec4), boardcolours);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);

			/*����ػ�*/
			glutPostRedisplay();
			glFlush();

			i--;	// ���¼�鵱ǰ�У���Ϊ��ʱ��ǰ����֮ǰ����һ��
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ���飬���Ҹ������̸��Ӧλ�ö������ɫVBO

void settile()
{
	// ÿ������
	for (int i = 0; i < 4; i++)
	{
		// ��ȡ���������̸��ϵ�����
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// �����Ӷ�Ӧ�����̸��ϵ�λ������Ϊ���
		board[x][y] = true;
		// ������Ӧλ�õ���ɫ�޸�
		changecellcolour(vec2(x,y), colors[randomShape]);
	}
}

//////////////////////////////////////////////////////////////////////////
// ����λ��(x,y)���ƶ����顣��Ч���ƶ�ֵΪ(-1,0)��(1,0)��(0,-1)���ֱ��Ӧ����
// �����º������ƶ�������ƶ��ɹ�������ֵΪtrue����֮Ϊfalse��

bool movetile(vec2 direction)
{
	// �����ƶ�֮��ķ����λ������
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// ����ƶ�֮�����Ч��
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			// ��Ч���ƶ��÷���
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			updatetile();

			return true;
		}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// ����������Ϸ

void restart()
{
	init();

	//// �����������̸��ռ��״̬Ϊfalse
	//for (int i = 0; i < 20; i++) {
	//	for (int j = 0; j < 10; j++) {
	//		board[j][i] = false;
	//	}
	//}
	//tilepos = vec2(5, 19);
	//rotation = 0;
	//// �����������̸����ɫΪ��ɫ
	//for (int i = 0; i < 1200; i++) {
	//	boardcolours[i] = black;
	//}


	//// �����Դ�����̸���ɫ����
	//glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, size(boardcolours), boardcolours);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//GLuint vPosition = glGetAttribLocation(program, "vPosition");
	//GLuint vColor = glGetAttribLocation(program, "vColor");

	//// ���õ�ǰ���鶥��λ��
	//glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	////glEnableVertexAttribArray(vPosition);

	//// ���õ�ǰ���鶥����ɫ
	//glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	////glEnableVertexAttribArray(vColor);

	//display();
}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��Ⱦ����

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // �������̸� (10*20*2 = 400 ��������)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // ���Ƶ�ǰ���� (8 ��������)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // �������̸����


	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// �ڴ��ڱ������ʱ�򣬿������̸�Ĵ�С��ʹ֮���̶ֹ��ı�����

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ�¼��е����ⰴ����Ӧ

void special(int key, int x, int y)
{
	if(!gameover)
	{
		switch(key)
		{
			case GLUT_KEY_UP:	// ���ϰ�����ת����
				rotate();
				break;
			case GLUT_KEY_DOWN: // ���°����ƶ�����
				if (!movetile(vec2(0, -1)))
				{
					settile();
					checkfullrow();
					newtile();
				}
				break;
			case GLUT_KEY_LEFT:  // ���󰴼��ƶ�����
				movetile(vec2(-1, 0));
				break;
			case GLUT_KEY_RIGHT: // ���Ұ����ƶ�����
				movetile(vec2(1, 0));
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧʱ���е���ͨ������Ӧ

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 033: // ESC�� �� 'q' ���˳���Ϸ
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' ��������Ϸ
			restart();
			break;
		case 32:
			if (isContinue)
				isContinue = false;
			else
				isContinue = true;
	}
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////

void idle(void)
{
	/*���ÿ��е��ã�ʹ�����Զ������ƶ�*/
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if ((currentTime - starttime) % 600 == 0 && isContinue) {
		if (!movetile(vec2(0, -1)))
		{
			settile();
			checkfullrow();
			newtile();
		}
		glutPostRedisplay();
		glFlush();
	}
	glutPostRedisplay();
	glFlush();
}

void printHelp() {
	printf("'up' -- Change the shape of tile.\n");
	printf("'down' -- Accelerate the tile going downn\n");
	printf("'left' -- Moving to left.\n");
	printf("'right' -- Moving to right.\n\n");
	printf("'space' -- Pause the game.\n");
	printf("'r' -- Reset the game.\n");
	printf("'q' -- Quit the game.\n");
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(440, 70);
	glutCreateWindow("���� - 2016150051");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	printHelp();
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
