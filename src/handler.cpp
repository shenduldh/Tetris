#include <time.h>
#include "main.h"

char bgArray[20][10] = { 0 }; // ����������
int nowBlockType = 0; // ��¼��ǰ��������
int nextBlockType = 0; // ��¼��һ����������
Position bp= { 0,0 }; // ��¼����λ��
bool isStart = false; // �Ƿ�ʼ��Ϸ�ı�־
bool isStop = true; // �Ƿ���ͣ��Ϸ�ı�־
int score = 0; // ����
int level = LEVEL_EASY; // ��Ϸ�Ѷ�
// ������������
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
	// �����������
	srand(unsigned int(time(NULL)));
	// ������ʼ���������
	CreateNextBlock();
	// �����������ͣ��ť
	CreateWindowA("BUTTON", "����(Space)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 600, 210, 30, hWnd, (HMENU)RETRY_BUTTON, NULL, NULL);
	CreateWindowA("BUTTON", "��ʼ/��ͣ(enter)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		210, 600, 210, 30, hWnd, (HMENU)STOP_BUTTON, NULL, NULL);
	// ���������ѶȰ�ť
	CreateWindowA("BUTTON", "��", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 630, 140, 30, hWnd, (HMENU)LEVEL_EASY, NULL, NULL);
	CreateWindowA("BUTTON", "�е�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		140, 630, 140, 30, hWnd, (HMENU)LEVEL_MEDIUM, NULL, NULL);
	CreateWindowA("BUTTON", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		280, 630, 140, 30, hWnd, (HMENU)LEVEL_HARD, NULL, NULL);
}

void RetryGame(HWND hWnd)
{
	int i, j;
	HDC hDc;
	// �����ʱ��
	KillTimer(hWnd, TIMER01);
	// ��ձ���
	for (i = 0; i < 20; ++i)
		for (j = 0; j < 10; ++j)
			bgArray[i][j] = 0;
	// �����������
	CreateNextBlock();
	// ��������
	score = 0;
	// �����־
	isStop = true;
	isStart = false;
	// �ػ����
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
	hMemDc = CreateCompatibleDC(hDc); // ��������DC
	hMemBitMap = CreateCompatibleBitmap(hDc, 420, 600); // ��������
	SelectObject(hMemDc, hMemBitMap); // ��������������DC
	// ������DC�ϻ�����
	Rectangle(hMemDc, 0, 0, 300, 600); // ������
	ShowDetail(hMemDc); // ��ʾ����
	PreviewNextBlock(hMemDc); // Ԥ����һ������
	PaintBgArray(hMemDc); // ������
	// ������DC�ϵ����ݸ��Ƶ�ʵ��DC��
	BitBlt(hDc, 0, 0, 420, 600, hMemDc, 0, 0, SRCCOPY);
	DeleteObject(hMemBitMap); // �ͷŻ���
	DeleteDC(hMemDc); // �ͷ�����DC
}

void PreviewNextBlock(HDC hDc)
{
	int i, j;
	HBRUSH newBrush = CreateSolidBrush(RGB(255, 255, 0)); // ������ˢ
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, newBrush); // �����»�ˢ�����ؾɻ�ˢ
	for (i = 0; i < 2; ++i)
		for (j = 0; j < 4; ++j)
			if (1 == blockArray[nextBlockType][i][j])
				Rectangle(hDc, 310 + j * 25, 100 + i * 25, 335 + j * 25, 125 + i * 25);
	newBrush = (HBRUSH)SelectObject(hDc, oldBrush);
	DeleteObject(newBrush);
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
	// ��������
	if (1 == BlockDown())
	{
		// ��������ˣ����������еķ���
		EraseBlocks();
		// �ж���Ϸ�Ƿ����
		if (1 == GameOver())
		{
			// �����ʱ��
			KillTimer(hWnd, TIMER01);
			return;
		}

		// �����µķ��鵽��Ϸ��
		AddNextBlock();
		// ������һ������
		CreateNextBlock();
	}

	// �ػ����
	HDC hDc = GetDC(hWnd); // ��ȡhWnd��Ӧ�Ĵ��ھ��hDc
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
	int temp = nextBlockType; // ����ֵ����ֹ���ı�

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
	nextBlockType = temp; // �ָ�ֵ
}

void ShowDetail(HDC hDc)
{
	// ����
	char strScore[10] = { 0 };
	LPCSTR strLevel = "��";

	// intת�����ַ�����Ŀ���������ַ�����������
	_itoa_s(score, strScore, 10);

	// �����鱳��
	Rectangle(hDc, 300, 0 , 420, 600);
	
	if (LEVEL_MEDIUM == level) strLevel = "�е�";
	if (LEVEL_HARD == level) strLevel = "����";

	TextOut(hDc, 315, 20, "�Ѷȣ�", strlen("�Ѷȣ�")); // ��ʾ"�Ѷ�"
	TextOut(hDc, 355, 20, strLevel, strlen(strLevel)); // ��ʾ�Ѷ�
	TextOut(hDc, 315, 40, "������", strlen("������")); // ��ʾ"����"
	TextOut(hDc, 355, 40, strScore, strlen(strScore)); // ��ʾ����
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

void KEY::enter() { // ��Ϸ��ʼ��
	int gameSpeed = 0;
	if (LEVEL_EASY == level) gameSpeed = 500;
	if (LEVEL_MEDIUM == level) gameSpeed = 250;
	if (LEVEL_HARD == level) gameSpeed = 100;

	if (!isStart)
	{
		isStart = true;
		// ��Ϸ��ʼ�������ʼ�������������
		AddNextBlock();
		// ������һ���������
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

	switch (nowBlockType)
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

void KEY::space()
{
	// �ؿ���Ϸ
	RetryGame(this->hWnd);
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