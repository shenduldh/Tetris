#include <Windows.h>

#define TIMER01 1
#define RETRY_BUTTON 20
#define STOP_BUTTON 21
#define LEVEL_EASY 31
#define LEVEL_MEDIUM 32
#define LEVEL_HARD 33

void Init(HWND hWnd);
void UpdateFrame(HDC hDc);
void PaintBgArray(HDC hDc);
void CreateNextBlock();
void PreviewNextBlock(HDC hDc);
int BlockDown();
void AddNextBlock();
int Change1to2();
void OnTimer(HWND hWnd);
int Chage5Shapes();
int ChangeLongBlock();
void UpdatePosition(int offset_line, int offset_column);
void EraseBlocks();
void ShowDetail(HDC hDc);
int GameOver();
void RetryGame(HWND hWnd);
void ChooseLevel(HWND hWnd, WPARAM wParam);

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
	void space();
};