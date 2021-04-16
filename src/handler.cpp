#include <time.h>
#include "main.h"

char bgArray[20][10] = { 0 }; // ����
char randomBlock[2][4] = { 0 };
int blockType = 0; // ��¼��������
Position bp= { 0,0 }; // ��¼����λ��
bool isStart = false; // �Ƿ�ʼ��Ϸ
int score = 0; // ����

void Init(HWND hWnd)
{
	// �����������
	srand(unsigned int(time(NULL)));
	// �����������
	CreateRandomBlock();
	// �������水ť
	CreateWindowA("BUTTON", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 600, 400, 30, hWnd, (HMENU)RETRY_BUTTON, NULL, NULL);
}

void RetryGame(HWND hWnd)
{
	int i, j;
	HDC hDc;
	// �����ʱ��
	KillTimer(hWnd, TIMER01);
	// ʹ���������»�ý���
	SetFocus(hWnd);
	// ��ձ���
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			bgArray[i][j] = 0;
	// �����������
	CreateRandomBlock();
	// ��������
	score = 0;
	// �رտ�ʼ��Ϸ��־
	isStart = false;
	// �ػ����
	hDc = GetDC(hWnd);
	UpdateFrame(hDc);
	ReleaseDC(hWnd, hDc);
}

void UpdateFrame(HDC hDc) {
	HDC hMemDc;
	HBITMAP hMemBitMap;
	hMemDc = CreateCompatibleDC(hDc); // ��������DC
	hMemBitMap = CreateCompatibleBitmap(hDc, 400, 600); // ��������
	SelectObject(hMemDc, hMemBitMap); // ��������������DC
	// ������DC�ϻ�����
	Rectangle(hMemDc, 0, 0, 300, 600); // ������
	PaintBgArray(hMemDc); // ������
	ShowScore(hMemDc); // ��ʾ����
	// ������DC�ϵ����ݸ��Ƶ�ʵ��DC��
	BitBlt(hDc, 0, 0, 400, 600, hMemDc, 0, 0, SRCCOPY);
	DeleteObject(hMemBitMap); // �ͷŻ���
	DeleteDC(hMemDc); // �ͷ�����DC
}

void PaintBgArray(HDC hDc) {
	int i, j;
	// ��1
	HBRUSH newBrush = CreateSolidBrush(RGB(255, 255, 0)); // ������ˢ
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, newBrush); // �����»�ˢ�����ؾɻ�ˢ
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			if (1== bgArray[i][j])	
				Rectangle(hDc, j * 30, i * 30, (j + 1) * 30, (i + 1) * 30);
	newBrush = (HBRUSH)SelectObject(hDc, oldBrush); // ���ؾɻ�ˢ
	DeleteObject(newBrush); // ɾ���»�ˢ

	// ��2
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

	// ���ײ���
	for (i = 0; i < 10; ++i) 
		if (1 == bgArray[19][i])
			return Change1to2();

	// ��2����
	for (i = 18; i >= 0; --i)
		for (j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
				if (2 == bgArray[i + 1][j])
					return Change1to2();

	// ����һ��
	for (i = 18; i >= 0; --i)
		for (j = 9; j >= 0; --j)
			if (1 == bgArray[i][j])
			{
				bgArray[i + 1][j] = bgArray[i][j];
				bgArray[i][j] = 0;
			}

	// ��¼λ�ã��м�1
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
	HDC hDc = GetDC(hWnd); // ��ȡhWnd��Ӧ�Ĵ��ھ��hDc
	// ��������
	if (1 == BlockDown())
	{
		// ��������ˣ����������еķ��飬ͬʱ����һ���µ��������
		EraseBlocks();
		// �ж���Ϸ�Ƿ����
		if (1 == GameOver())
		{
			// �����ʱ��
			KillTimer(hWnd, TIMER01);
			return;
		}
		CreateRandomBlock();
	}
	UpdateFrame(hDc); // �ػ���hDc��Ӧ����
	ReleaseDC(hWnd, hDc); // �ͷŴ��ھ��
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
	char strScore[10] = { 0 }; // ����
	_itoa_s(score, strScore, 10); // intת�����ַ�����Ŀ���������ַ�����������
	Rectangle(hDc, 300, 0 , 400, 600); // ��������
	TextOut(hDc, 325, 50, strScore, strlen(strScore)); // ��ʾ����
	TextOut(hDc, 325, 30, "����", strlen("����")); // ��ʾ"����"
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

// ʵ��KEY��
KEY::KEY(HWND hWnd) {
	this->hWnd = hWnd;
}

void KEY::enter() {
	// ������ʱ��
	SetTimer(this->hWnd, TIMER01, 500, NULL);
}

void KEY::left()
{
	// �߽���
	for (int i = 0; i < 20; ++i)
		if (1 == bgArray[i][0])
			return;

	// ������ײ���
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
				if (2 == bgArray[i][j-1])
					return;

	// ����һ��
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
			{
				bgArray[i][j-1] = bgArray[i][j];
				bgArray[i][j] = 0;
			}

	// ��¼λ�ã��м�1
	UpdatePosition(0, -1);

	// �ػ�
	HDC hDc = GetDC(this->hWnd); // ��ȡhWnd��Ӧ�Ĵ��ھ��hDc
	UpdateFrame(hDc); // �ػ���hDc��Ӧ����
	ReleaseDC(hWnd, hDc); // �ͷŴ��ھ��
}

void KEY::right()
{
	// �߽���
	for (int i = 0; i < 20; ++i)
		if (1 == bgArray[i][9])
			return;

	// ������ײ���
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			if (1 == bgArray[i][j])
				if (2 == bgArray[i][j + 1])
					return;

	// ����һ��
	for (int i = 0; i < 20; ++i)
		for (int j = 9; j >= 0; --j)
			if (1 == bgArray[i][j])
			{
				bgArray[i][j + 1] = bgArray[i][j];
				bgArray[i][j] = 0;
			}

	// ��¼λ�ã��м�1
	UpdatePosition(0, 1);

	// �ػ�
	HDC hDc = GetDC(this->hWnd); // ��ȡhWnd��Ӧ�Ĵ��ھ��hDc
	UpdateFrame(hDc); // �ػ���hDc��Ӧ����
	ReleaseDC(hWnd, hDc); // �ͷŴ��ھ��
}

void KEY::up()
{
	HDC hDc = GetDC(this->hWnd); // ��ȡhWnd��Ӧ�Ĵ��ھ��hDc

	switch (blockType)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		// ������ܱ��Σ���ֱ���˳�
		if (1 == Chage5Shapes()) return;
		break;
	case 5:
		break;
	case 6:
		if (1 == ChangeLongBlock()) return;
		break;
	}

	UpdateFrame(hDc); // �ػ���hDc��Ӧ����
	ReleaseDC(this->hWnd, hDc); // �ͷŴ��ھ��
}

int ChangeLongBlock()
{
	int i, j, k;
	if (1==bgArray[bp.line][bp.column - 1]) // ������
	{
		// �ж��Ƿ���Ա���
		for (i = 1; i < 4; ++i)
			if (2 == bgArray[bp.line - 1][bp.column] || bp.line - i < 0) break;
		for (j = 1; j < 4; ++j)
			if (2 == bgArray[bp.line + j][bp.column]) break;
		if (i + j < 5) return 1;

		// ���ԭ��λ�õķ���
		bgArray[bp.line][bp.column - 1] = 0;
		bgArray[bp.line][bp.column] = 0;
		bgArray[bp.line][bp.column + 1] = 0;
		bgArray[bp.line][bp.column + 2] = 0;

		// �ӵײ��������ĸ���λ���÷���
		k = j - 1;
		i = 0;
		j = 2;
		if (bp.line < 1) j = 3; // ��������ϱ����ϰ������
		if (k < j) j = k; // ���⴩ģ
		while (i < 4)
		{
			// �ҿ�λ���ҿ�λ���ܳ����ײ��߽�
			if (0 == bgArray[bp.line + j][bp.column] && (bp.line + j) < 20)
			{
				bgArray[bp.line + j][bp.column] = 1;
				++i;
			}
			--j;
		}
		// ����λ��
		UpdatePosition(j + 2, 0);
	}
	else // �������
	{
		// �ж��Ƿ���Ա���
		for (i = 1; i < 4; ++i)
			if (2 == bgArray[bp.line][bp.column - i] || bp.column - i < 0) break;
		for (j = 1; j < 4; ++j)
			if (2 == bgArray[bp.line][bp.column + j] || bp.column + j > 9) break;
		if (i + j < 5) return 1;

		// ���ԭ��λ�õķ���
		bgArray[bp.line - 1][bp.column] = 0;
		bgArray[bp.line][bp.column] = 0;
		bgArray[bp.line + 1][bp.column] = 0;
		bgArray[bp.line + 2][bp.column] = 0;

		// �����������ĸ���λ���÷���
		k = j - 1;
		i = 0;
		j = 2;
		if (bp.column < 1 || 2 == bgArray[bp.line][bp.column - 1]) j = 3; // �������������ϰ������
		if (k < j) j = k; // ���⴩ģ
		while (i < 4)
		{
			// �ҿ�λ���ҿ�λ���ܳ����Ҳ�߽�
			if (0 == bgArray[bp.line][bp.column + j] && bp.column + j < 10)
			{
				bgArray[bp.line][bp.column + j] = 1;
				++i;
			}
			--j;
		}
		// ����λ��
		UpdatePosition(0, j + 2);
	}

	return 0;
}

int Chage5Shapes()
{
	char copyArray[3][3] = { 0 };
	int i, j;

	// �߽���ײ�������������򳬳����ұ߽磬��λ���Ƶ��ɱ���������
	if (bp.column < 0) bp.column = 0;
	if (bp.column > 7) bp.column = 7;
	if (bp.line > 17) bp.line = 17;

	// ����
	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
		{
			// ������������ڴ��ڷ���2�����ܱ��Σ�ֱ�ӷ���
			if (2 == bgArray[bp.line + i][bp.column + j]) return 1;
			copyArray[i][j] = bgArray[bp.line + i][bp.column + j];
		}

	// ����
	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
			bgArray[bp.line + i][bp.column + j] = copyArray[2-j][i];

	return 0;
}

void KEY::down()
{
	// �����¼�Ч��ͬ��ʱ���Ļص�����
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