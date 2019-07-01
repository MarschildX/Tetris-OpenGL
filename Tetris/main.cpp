/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - 本代码仅仅是参考代码，具体要求请参考作业说明，按照顺序逐步完成。
 * - 关于配置OpenGL开发环境、编译运行，请参考第一周实验课程相关文档。
 *
 * - 已实现功能如下：
 * - 1) 绘制棋盘格和‘L’型方块
 * - 2) 键盘左/右/下键控制方块的移动，上键旋转方块
 *
 * - 未实现功能如下：
 * - 1) 随机生成方块并赋上不同的颜色
 * - 2) 方块的自动向下移动
 * - 3) 方块之间的碰撞检测
 * - 4) 棋盘格中每一行填充满之后自动消除
 * - 5) 其他
 *
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

int starttime;			// 控制方块向下移动时间
int rotation = 0;		// 控制当前窗口中的方块旋转
vec2 tile[4];			// 表示当前窗口中的方块
bool gameover = false;	// 游戏结束控制变量
int xsize = 400;		// 窗口大小（尽量不要变动窗口大小！）
int ysize = 720;

int randomShape = 0;	// 随机选择每次的方块形状
bool isContinue = true; // 控制游戏是否继续进行
GLuint program;

//// 一个二维数组表示所有可能出现的方块和方向。
//vec2 allRotationsLshape[4][4] =
//							  {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},	//   "L"
//							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1)},   //
//							   {vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
//							   {vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};

// 一个三维数组表示所有可能出现的形状和方向
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

// 绘制窗口的颜色变量
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

// 当前方块的位置（以棋盘格的左下角为原点的坐标系）
vec2 tilepos = vec2(5, 19);

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[10][20];


// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
vec4 boardcolours[1200];

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

void display();

//////////////////////////////////////////////////////////////////////////
// 修改棋盘格在pos位置的颜色为colour，并且更新对应的VBO

void changecellcolour(vec2 pos, vec4 colour)
{
	// 每个格子是个正方形，包含两个三角形，总共6个定点，并在特定的位置赋上适当的颜色
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// 计算偏移量，在适当的位置赋上颜色
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// 当前方块移动或者旋转时，更新VBO

void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		// 每个格子包含两个三角形，所以有6个顶点坐标
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断，没有足够的空间来生成新的方块。

void newtile()
{
	// 将新方块放于棋盘格的最上行中间位置并设置默认的旋转方向
	tilepos = vec2(5 , 19);
	rotation = 0;

	/*根据时间设置随机种子并产生随机数用于随机化每次的方块形状*/
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

	// 给新方块赋上颜色
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
// 游戏和OpenGL初始化

void init()
{
	// 初始化棋盘格，包含64个顶点坐标（总共32条线），并且每个顶点一个颜色值
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// 纵向线
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// 水平线
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// 将所有线赋成白色
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// 初始化棋盘格，并将没有被填充的格子设置成黑色
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
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

	// 将棋盘格所有位置的填充与否都设置为false（没有被填充）
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// 载入着色器
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// 棋盘格顶点
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 棋盘格每个格子
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// 棋盘格每个格子顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格每个格子顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 当前方块
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// 游戏初始化
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}

//////////////////////////////////////////////////////////////////////////
// 检查在cellpos位置的格子是否被填充或者是否在棋盘格的边界范围内。

bool checkvalid(vec2 cellpos)
{
	int x = cellpos.x;
	int y = cellpos.y;
	if((cellpos.x >=0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20) && !board[x][y] ) /*加入对棋盘是否被占用的检查*/
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// 在棋盘上有足够空间的情况下旋转当前方块

void rotate()
{
	// 计算得到下一个旋转方向
	int nextrotation = (rotation + 1) % 4;

	// 检查当前旋转之后的位置的有效性
	if (checkvalid((allShapeAndRotation[randomShape][nextrotation][0]) + tilepos)
		&& checkvalid((allShapeAndRotation[randomShape][nextrotation][1]) + tilepos)
		&& checkvalid((allShapeAndRotation[randomShape][nextrotation][2]) + tilepos)
		&& checkvalid((allShapeAndRotation[randomShape][nextrotation][3]) + tilepos))
	{
		// 更新旋转，将当前方块设置为旋转之后的方块
		rotation = nextrotation;
		for (int i = 0; i < 4; i++)
			tile[i] = allShapeAndRotation[randomShape][rotation][i]; /*根据随机到的形状变换方向*/

		updatetile();
	}
}

//////////////////////////////////////////////////////////////////////////
// 检查棋盘格任意行有没有被填充满

void checkfullrow()
{
	for (int i = 0; i < 19; i++) {
		bool isFull = true;	// 记录当前行是否被占满
		// 检查是否整行都被占满
		for (int j = 0; j < 10; j++) {
			if (!board[j][i]) {
				isFull = false;
				break;
			}
		}
		if (isFull) {
			// 将棋盘颜色数据更新以及占用状态更新
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
			// 此时棋盘的最上面一行设定为黑色且占用状态为false
			for (int j = 0; j < 10; j++) {
				boardcolours[6 * (19 * 10 + j) + 0] = black;
				boardcolours[6 * (19 * 10 + j) + 1] = black;
				boardcolours[6 * (19 * 10 + j) + 2] = black;
				boardcolours[6 * (19 * 10 + j) + 3] = black;
				boardcolours[6 * (19 * 10 + j) + 4] = black;
				boardcolours[6 * (19 * 10 + j) + 5] = black;
				board[j][19] = false;
			}

			/*重新绑定数据*/
			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 1200*sizeof(vec4), boardcolours);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);

			/*标记重绘*/
			glutPostRedisplay();
			glFlush();

			i--;	// 重新检查当前行，因为此时当前行是之前的上一行
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO

void settile()
{
	// 每个格子
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// 将格子对应在棋盘格上的位置设置为填充
		board[x][y] = true;
		// 并将相应位置的颜色修改
		changecellcolour(vec2(x,y), colors[randomShape]);
	}
}

//////////////////////////////////////////////////////////////////////////
// 给定位置(x,y)，移动方块。有效的移动值为(-1,0)，(1,0)，(0,-1)，分别对应于向
// 左，向下和向右移动。如果移动成功，返回值为true，反之为false。

bool movetile(vec2 direction)
{
	// 计算移动之后的方块的位置坐标
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// 检查移动之后的有效性
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			// 有效：移动该方块
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			updatetile();

			return true;
		}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// 重新启动游戏

void restart()
{
	init();

	//// 重置所有棋盘格的占用状态为false
	//for (int i = 0; i < 20; i++) {
	//	for (int j = 0; j < 10; j++) {
	//		board[j][i] = false;
	//	}
	//}
	//tilepos = vec2(5, 19);
	//rotation = 0;
	//// 重置所有棋盘格的颜色为黑色
	//for (int i = 0; i < 1200; i++) {
	//	boardcolours[i] = black;
	//}


	//// 更新显存的棋盘格颜色数据
	//glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, size(boardcolours), boardcolours);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//GLuint vPosition = glGetAttribLocation(program, "vPosition");
	//GLuint vColor = glGetAttribLocation(program, "vColor");

	//// 重置当前方块顶点位置
	//glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	////glEnableVertexAttribArray(vPosition);

	//// 重置当前方块顶点颜色
	//glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	////glEnableVertexAttribArray(vColor);

	//display();
}

//////////////////////////////////////////////////////////////////////////
// 游戏渲染部分

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // 绘制棋盘格 (10*20*2 = 400 个三角形)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // 绘制棋盘格的线


	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应事件中的特殊按键响应

void special(int key, int x, int y)
{
	if(!gameover)
	{
		switch(key)
		{
			case GLUT_KEY_UP:	// 向上按键旋转方块
				rotate();
				break;
			case GLUT_KEY_DOWN: // 向下按键移动方块
				if (!movetile(vec2(0, -1)))
				{
					settile();
					checkfullrow();
					newtile();
				}
				break;
			case GLUT_KEY_LEFT:  // 向左按键移动方块
				movetile(vec2(-1, 0));
				break;
			case GLUT_KEY_RIGHT: // 向右按键移动方块
				movetile(vec2(1, 0));
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应时间中的普通按键响应

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 033: // ESC键 和 'q' 键退出游戏
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' 键重启游戏
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
	/*设置空闲调用，使方块自动向下移动*/
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
	glutCreateWindow("方旭 - 2016150051");
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
