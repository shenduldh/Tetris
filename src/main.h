#include <Windows.h>

#define TIMER01 1
#define RETRY_BUTTON 2

void Init(HWND hWnd);
void UpdateFrame(HDC hDc);
void PaintBgArray(HDC hDc);
void CreateRandomBlock();
int BlockDown();
int Change1to2();
void OnTimer(HWND hWnd);
int Chage5Shapes();
int ChangeLongBlock();
void UpdatePosition(int offset_line, int offset_column);
void EraseBlocks();
void ShowScore(HDC hDc);
int GameOver();
void RetryGame(HWND hWnd);

struct Position
{
	int line;
	int column;
};

// 处理按键消息
class KEY {
public:
	KEY(HWND hWnd);
	void run(WPARAM keyCode);
private:
	HWND hWnd;
	void enter();
	void left();
	void right();
	void up();
	void down();
};