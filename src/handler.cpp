#include <time.h>
#include "main.h"

char bgArray[20][10] = { 0 }; // 背景
char randomBlock[2][4] = { 0 };
int blockType = 0; // 记录方块类型
Position bp= { 0,0 }; // 记录方块位置
bool isStart = false; // 是否开始游戏
int score = 0; // 分数

void Init(HWND hWnd)
{
	// 产生随机种子
	srand(unsigned int(time(NULL)));
	// 创建随机方块
	CreateRandomBlock();
	// 创建重玩按钮
	CreateWindowA("BUTTON", "重玩", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 600, 400, 30, hWnd, (HMENU)RETRY_BUTTON, NULL, NULL);
}

void RetryGame(HWND hWnd)
{
	int i, j;
	HDC hDc;
	// 清除定时器
	KillTimer(hWnd, TIMER01);
	// 使主窗口重新获得焦点
	SetFocus(hWnd);
	// 清空背景
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			bgArray[i][j] = 0;
	// 创建随机方块
	CreateRandomBlock();
	// 分数清零
	score = 0;
	// 关闭开始游戏标志
	isStart = false;
	// 重绘界面
	hDc = GetDC(hWnd);
	UpdateFrame(hDc);
	ReleaseDC(hWnd, hDc);
}

void UpdateFrame(HDC hDc) {
	HDC hMemDc;
	HBITMAP hMemBitMap;
	hMemDc = CreateCompatibleDC(hDc); // 创建虚拟DC
	hMemBitMap = CreateCompatibleBitmap(hDc, 400, 600); // 创建画布
	SelectObject(hMemDc, hMemBitMap); // 关联画布和虚拟DC
	// 在虚拟DC上画内容
	Rectangle(hMemDc, 0, 0, 300, 600); // 画背景
	PaintBgArray(hMemDc); // 画矩形
	ShowScore(hMemDc); // 显示分数
	// 将虚拟DC上的内容复制到实际DC上
	BitBlt(hDc, 0, 0, 400, 600, hMemDc, 0, 0, SRCCOPY);
	DeleteObject(hMemBitMap); // 释放画布
	DeleteDC(hMemDc); // 释放虚拟DC
}

void PaintBgArray(HDC hDc) {
	int i, j;
	// 画1
	HBRUSH newBrush = CreateSolidBrush(RGB(255, 255, 0)); // 创建画刷
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, newBrush); // 连接新画刷，返回旧画刷
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			if (1== bgArray[i][j])	
				Rectangle(hDc, j * 30, i * 30, (j + 1) * 30, (i + 1) * 30);
	newBrush = (HBRUSH)SelectObject(hDc, oldBrush); // 换回旧画刷
	DeleteObject(newBrush); // 删除新画刷

	// 画2
	newBrush = CreateSolidBrush(RGB(233, 25, 62));
	oldBrush = (HBRUSH)SelectObject(hDc, newBrush);
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			if (2 == bgArray[i][j])
				Rectangle(hDc, j * 30, i * 30, (j + 1) * 30, (i + 1) * 30);
	newBrush = (HBRUSH)SelectObject(hDc, oldBrush);
	DeleteObject(newBrush);
}

void CreateRandomBlock()
{
	int i, j;
	blockType = rand() % 7;
	bp.line = 0;
	bp.column = 3;
	if (6 == blockType) bp.column = 4;

	char randomBlockArray[7][8] = {
		{1,1,0,0,0,1,1,0},
		{0,1,1,0,1,1,0,0},
		{1,0,0,0,1,1,1,0},
		{0,0,1,0,1,1,1,0},
		{0,1,0,0,1,1,1,0},
		{1,1,0,0,1,1,0,0},
		{1,1,1,1,0,0,0,0}
	};
	for (i = 0; i < 8; ++i)
		randomBlock[i/4][i%4] = randomBlockArray[blockType][i];

	for (i = 0; i < 2; ++i)
		for (j = 0; j < 4; ++j)
			bgArray[i][j + 3] = randomBlock[i][j];
}

int BlockDown()
{
	int i,j;

	// 到底不落
	for (i = 0; i < 10; ++i) 
		if (1 == bgArray[19][i])
			return Change1to2();

	// 遇2不落
	for (i = 18; i >= 0; --i)
		for (j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
				if (2 == bgArray[i + 1][j])
					return Change1to2();

	// 下落一格
	for (i = 18; i >= 0; --i)
		for (j = 9; j >= 0; --j)
			if (1 == bgArray[i][j])
			{
				bgArray[i + 1][j] = bgArray[i][j];
				bgArray[i][j] = 0;
			}

	// 记录位置：行加1
	UpdatePosition(1, 0);
	return 0;
}

int Change1to2()
{
	for (int i = 19; i >= 0; --i)
		for (int j = 9; j >= 0; --j)
			if (1 == bgArray[i][j])
				bgArray[i][j] = 2;
	return 1;
}

void OnTimer(HWND hWnd)
{
	HDC hDc = GetDC(hWnd); // 获取hWnd对应的窗口句柄hDc
	// 方块下落
	if (1 == BlockDown())
	{
		// 如果到底了，则消除成行的方块，同时创建一个新的随机方块
		EraseBlocks();
		// 判断游戏是否结束
		if (1 == GameOver())
		{
			// 清除定时器
			KillTimer(hWnd, TIMER01);
			return;
		}
		CreateRandomBlock();
	}
	UpdateFrame(hDc); // 重绘句柄hDc对应区域
	ReleaseDC(hWnd, hDc); // 释放窗口句柄
}

void UpdatePosition(int offset_line, int offset_column) 
{
	bp.line += offset_line;
	bp.column += offset_column;
}

void EraseBlocks()
{
	int i, j, k;
	int sum = 0;

	for (i = 19; i >= 0; --i)
	{
		for (j = 0; j < 10; ++j)
			sum += bgArray[i][j];
		if (20 == sum)
		{
			for (k = i - 1; i >= 0; --i)
				for (j = 0; j < 10; ++j)
					bgArray[i + 1][j] = bgArray[i][j];
			i = 20;
			++score;
		}
		sum = 0;
	}
}

void ShowScore(HDC hDc)
{
	char strScore[10] = { 0 }; // 容器
	_itoa_s(score, strScore, 10); // int转换成字符串：目标整数、字符容器、进制
	Rectangle(hDc, 300, 0 , 400, 600); // 分数背景
	TextOut(hDc, 325, 50, strScore, strlen(strScore)); // 显示分数
	TextOut(hDc, 325, 30, "分数", strlen("分数")); // 显示"分数"
}

int GameOver()
{
	for (int i = 0; i < 10; ++i)
		if (2 == bgArray[0][i])
		{
			MessageBox(NULL, "YOU FAILED!", "note", MB_OK);
			return 1;
		}
	return 0;
}

// 实现KEY类
KEY::KEY(HWND hWnd) {
	this->hWnd = hWnd;
}

void KEY::enter() {
	// 开启定时器
	SetTimer(this->hWnd, TIMER01, 500, NULL);
}

void KEY::left()
{
	// 边界检测
	for (int i = 0; i < 20; ++i)
		if (1 == bgArray[i][0])
			return;

	// 方块碰撞检测
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
				if (2 == bgArray[i][j-1])
					return;

	// 左移一格
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
			{
				bgArray[i][j-1] = bgArray[i][j];
				bgArray[i][j] = 0;
			}

	// 记录位置：列减1
	UpdatePosition(0, -1);

	// 重绘
	HDC hDc = GetDC(this->hWnd); // 获取hWnd对应的窗口句柄hDc
	UpdateFrame(hDc); // 重绘句柄hDc对应区域
	ReleaseDC(hWnd, hDc); // 释放窗口句柄
}

void KEY::right()
{
	// 边界检测
	for (int i = 0; i < 20; ++i)
		if (1 == bgArray[i][9])
			return;

	// 方块碰撞检测
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
				if (2 == bgArray[i][j + 1])
					return;

	// 右移一格
	for (int i = 0; i < 20; ++i)
		for (int j = 9; j >= 0; --j)
			if (1 == bgArray[i][j])
			{
				bgArray[i][j + 1] = bgArray[i][j];
				bgArray[i][j] = 0;
			}

	// 记录位置：列加1
	UpdatePosition(0, 1);

	// 重绘
	HDC hDc = GetDC(this->hWnd); // 获取hWnd对应的窗口句柄hDc
	UpdateFrame(hDc); // 重绘句柄hDc对应区域
	ReleaseDC(hWnd, hDc); // 释放窗口句柄
}

void KEY::up()
{
	HDC hDc = GetDC(this->hWnd); // 获取hWnd对应的窗口句柄hDc

	switch (blockType)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		// 如果不能变形，则直接退出
		if (1 == Chage5Shapes()) return;
		break;
	case 5:
		break;
	case 6:
		if (1 == ChangeLongBlock()) return;
		break;
	}

	UpdateFrame(hDc); // 重绘句柄hDc对应区域
	ReleaseDC(this->hWnd, hDc); // 释放窗口句柄
}

int ChangeLongBlock()
{
	int i, j, k;
	if (1==bgArray[bp.line][bp.column - 1]) // 横的情况
	{
		// 判断是否可以变形
		for (i = 1; i < 4; ++i)
			if (2 == bgArray[bp.line - 1][bp.column] || bp.line - i < 0) break;
		for (j = 1; j < 4; ++j)
			if (2 == bgArray[bp.line + j][bp.column]) break;
		if (i + j < 5) return 1;

		// 清除原来位置的方块
		bgArray[bp.line][bp.column - 1] = 0;
		bgArray[bp.line][bp.column] = 0;
		bgArray[bp.line][bp.column + 1] = 0;
		bgArray[bp.line][bp.column + 2] = 0;

		// 从底部向上找四个空位放置方块
		k = j - 1;
		i = 0;
		j = 2;
		if (bp.line < 1) j = 3; // 处理轴点上边有障碍的情况
		if (k < j) j = k; // 避免穿模
		while (i < 4)
		{
			// 找空位，且空位不能超出底部边界
			if (0 == bgArray[bp.line + j][bp.column] && (bp.line + j) < 20)
			{
				bgArray[bp.line + j][bp.column] = 1;
				++i;
			}
			--j;
		}
		// 更新位置
		UpdatePosition(j + 2, 0);
	}
	else // 竖的情况
	{
		// 判断是否可以变形
		for (i = 1; i < 4; ++i)
			if (2 == bgArray[bp.line][bp.column - i] || bp.column - i < 0) break;
		for (j = 1; j < 4; ++j)
			if (2 == bgArray[bp.line][bp.column + j] || bp.column + j > 9) break;
		if (i + j < 5) return 1;

		// 清除原来位置的方块
		bgArray[bp.line - 1][bp.column] = 0;
		bgArray[bp.line][bp.column] = 0;
		bgArray[bp.line + 1][bp.column] = 0;
		bgArray[bp.line + 2][bp.column] = 0;

		// 从右向左找四个空位放置方块
		k = j - 1;
		i = 0;
		j = 2;
		if (bp.column < 1 || 2 == bgArray[bp.line][bp.column - 1]) j = 3; // 处理轴点左边有障碍的情况
		if (k < j) j = k; // 避免穿模
		while (i < 4)
		{
			// 找空位，且空位不能超出右侧边界
			if (0 == bgArray[bp.line][bp.column + j] && bp.column + j < 10)
			{
				bgArray[bp.line][bp.column + j] = 1;
				++i;
			}
			--j;
		}
		// 更新位置
		UpdatePosition(0, j + 2);
	}

	return 0;
}

int Chage5Shapes()
{
	char copyArray[3][3] = { 0 };
	int i, j;

	// 边界碰撞处理：若变形区域超出左右边界，则将位置移到可变形区域内
	if (bp.column < 0) bp.column = 0;
	if (bp.column > 7) bp.column = 7;
	if (bp.line > 17) bp.line = 17;

	// 复制
	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
		{
			// 如果变形区域内存在方块2，则不能变形，直接返回
			if (2 == bgArray[bp.line + i][bp.column + j]) return 1;
			copyArray[i][j] = bgArray[bp.line + i][bp.column + j];
		}

	// 变形
	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
			bgArray[bp.line + i][bp.column + j] = copyArray[2-j][i];

	return 0;
}

void KEY::down()
{
	// 按向下键效果同计时器的回调函数
	OnTimer(this->hWnd);
}

void KEY::run(WPARAM keyCode) {
	switch (keyCode)
	{
	case VK_RETURN:
		isStart = true;
		this->enter();
		break;
	case VK_LEFT:
		this->left();
		break;
	case VK_RIGHT:
		this->right();
		break;
	case VK_UP:
		if(isStart) 
			this->up();
		break;
	case VK_DOWN:
		if (isStart)
			this->down();
		break;
	}
}