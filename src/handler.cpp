#include <time.h>
#include "main.h"

char bgArray[20][10] = { 0 }; // 背景方块组
int nowBlockType = 0; // 记录当前方块类型
int nextBlockType = 0; // 记录下一个方块类型
Position bp= { 0,0 }; // 记录方块位置
bool isStart = false; // 是否开始游戏的标志
bool isStop = true; // 是否暂停游戏的标志
int score = 0; // 分数
int level = LEVEL_EASY; // 游戏难度
// 随机方块的种类
char blockArray[7][2][4] = {
	{1,1,0,0,0,1,1,0},
	{0,1,1,0,1,1,0,0},
	{1,0,0,0,1,1,1,0},
	{0,0,1,0,1,1,1,0},
	{0,1,0,0,1,1,1,0},
	{1,1,0,0,1,1,0,0},
	{1,1,1,1,0,0,0,0}
};

void Init(HWND hWnd)
{
	// 产生随机种子
	srand(unsigned int(time(NULL)));
	// 创建初始的随机方块
	CreateNextBlock();
	// 创建重玩和暂停按钮
	CreateWindowA("BUTTON", "重玩(Space)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 600, 210, 30, hWnd, (HMENU)RETRY_BUTTON, NULL, NULL);
	CreateWindowA("BUTTON", "开始/暂停(enter)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		210, 600, 210, 30, hWnd, (HMENU)STOP_BUTTON, NULL, NULL);
	// 创建三个难度按钮
	CreateWindowA("BUTTON", "简单", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 630, 140, 30, hWnd, (HMENU)LEVEL_EASY, NULL, NULL);
	CreateWindowA("BUTTON", "中等", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		140, 630, 140, 30, hWnd, (HMENU)LEVEL_MEDIUM, NULL, NULL);
	CreateWindowA("BUTTON", "困难", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		280, 630, 140, 30, hWnd, (HMENU)LEVEL_HARD, NULL, NULL);
}

void RetryGame(HWND hWnd)
{
	int i, j;
	HDC hDc;
	// 清除定时器
	KillTimer(hWnd, TIMER01);
	// 清空背景
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			bgArray[i][j] = 0;
	// 创建随机方块
	CreateNextBlock();
	// 分数清零
	score = 0;
	// 处理标志
	isStop = true;
	isStart = false;
	// 重绘界面
	hDc = GetDC(hWnd);
	UpdateFrame(hDc);
	ReleaseDC(hWnd, hDc);
}

void ChooseLevel(HWND hWnd, WPARAM wParam)
{
	level = wParam;
	RetryGame(hWnd);
}

void UpdateFrame(HDC hDc) {
	HDC hMemDc;
	HBITMAP hMemBitMap;
	hMemDc = CreateCompatibleDC(hDc); // 创建虚拟DC
	hMemBitMap = CreateCompatibleBitmap(hDc, 420, 600); // 创建画布
	SelectObject(hMemDc, hMemBitMap); // 关联画布和虚拟DC
	// 在虚拟DC上画内容
	Rectangle(hMemDc, 0, 0, 300, 600); // 画背景
	ShowDetail(hMemDc); // 显示分数
	PreviewNextBlock(hMemDc); // 预览下一个方块
	PaintBgArray(hMemDc); // 画矩形
	// 将虚拟DC上的内容复制到实际DC上
	BitBlt(hDc, 0, 0, 420, 600, hMemDc, 0, 0, SRCCOPY);
	DeleteObject(hMemBitMap); // 释放画布
	DeleteDC(hMemDc); // 释放虚拟DC
}

void PreviewNextBlock(HDC hDc)
{
	int i, j;
	HBRUSH newBrush = CreateSolidBrush(RGB(255, 255, 0)); // 创建画刷
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, newBrush); // 连接新画刷，返回旧画刷
	for (i = 0; i < 2; ++i)
		for (j = 0; j < 4; ++j)
			if (1 == blockArray[nextBlockType][i][j])
				Rectangle(hDc, 310 + j * 25, 100 + i * 25, 335 + j * 25, 125 + i * 25);
	newBrush = (HBRUSH)SelectObject(hDc, oldBrush);
	DeleteObject(newBrush);
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

void CreateNextBlock()
{
	nextBlockType = rand() % 7;
}

void AddNextBlock()
{
	int i, j;
	nowBlockType = nextBlockType;
	for (i = 0; i < 2; ++i)
		for (j = 0; j < 4; ++j)
			bgArray[i][j + 3] = blockArray[nowBlockType][i][j];
	bp.line = 0;
	bp.column = 3;
	if (6 == nowBlockType) bp.column = 4;
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
	// 方块下落
	if (1 == BlockDown())
	{
		// 如果到底了，则消除成行的方块
		EraseBlocks();
		// 判断游戏是否结束
		if (1 == GameOver())
		{
			// 清除定时器
			KillTimer(hWnd, TIMER01);
			return;
		}

		// 加入新的方块到游戏中
		AddNextBlock();
		// 产生下一个方块
		CreateNextBlock();
	}

	// 重绘界面
	HDC hDc = GetDC(hWnd); // 获取hWnd对应的窗口句柄hDc
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
	int temp = nextBlockType; // 保存值，防止被改变

	for (i = 19; i >= 0; --i)
	{
		for (j = 0; j < 10; ++j)
			sum += bgArray[i][j];
		if (20 == sum)
		{
			for (k = i - 1; k >= 0; --k)
				for (j = 0; j < 10; ++j)
					bgArray[k + 1][j] = bgArray[k][j];
			i = 20;
			++score;
		}
		sum = 0;
	}
	nextBlockType = temp; // 恢复值
}

void ShowDetail(HDC hDc)
{
	// 容器
	char strScore[10] = { 0 };
	LPCSTR strLevel = "简单";

	// int转换成字符串：目标整数、字符容器、进制
	_itoa_s(score, strScore, 10);

	// 画详情背景
	Rectangle(hDc, 300, 0 , 420, 600);
	
	if (LEVEL_MEDIUM == level) strLevel = "中等";
	if (LEVEL_HARD == level) strLevel = "困难";

	TextOut(hDc, 315, 20, "难度：", strlen("难度：")); // 显示"难度"
	TextOut(hDc, 355, 20, strLevel, strlen(strLevel)); // 显示难度
	TextOut(hDc, 315, 40, "分数：", strlen("分数：")); // 显示"分数"
	TextOut(hDc, 355, 40, strScore, strlen(strScore)); // 显示分数
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

void KEY::enter() { // 游戏开始键
	int gameSpeed = 0;
	if (LEVEL_EASY == level) gameSpeed = 500;
	if (LEVEL_MEDIUM == level) gameSpeed = 250;
	if (LEVEL_HARD == level) gameSpeed = 100;

	if (!isStart)
	{
		isStart = true;
		// 游戏开始，掉落初始创建的随机方块
		AddNextBlock();
		// 创建下一个随机方块
		CreateNextBlock();
	}
	if (isStop) {
		isStop = false;
		SetTimer(this->hWnd, TIMER01, gameSpeed, NULL);
	}
	else
	{
		isStop = true;
		KillTimer(this->hWnd, TIMER01);
	}
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

	switch (nowBlockType)
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

void KEY::space()
{
	// 重开游戏
	RetryGame(this->hWnd);
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
		this->enter();
		break;
	case VK_LEFT:
		if (isStart && !isStop) this->left();
		break;
	case VK_RIGHT:
		if (isStart && !isStop) this->right();
		break;
	case VK_UP:
		if (isStart && !isStop) this->up();
		break;
	case VK_DOWN:
		if (isStart && !isStop) this->down();
		break;
	case VK_SPACE:
		if (isStart) this->space();
		break;
	}
}