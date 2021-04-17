#include "resource.h"
#include "main.h"


// 声明回调函数
LRESULT CALLBACK MsgProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


/* WinMain：Win32程序指定入口函数
** WINAPI：调用协议，用于约定在调用函数时如何情理堆栈，有__cdecl和__stdcall两种，WINAPI是__stdcall的宏定义
** hInstance：当前程序的实例句柄
** hPreInstance：当前程序打开前上一个程序的句柄
** lpCmdLine：传递给程序的命令行参数
** nCmdShow：指定窗口的显示方式
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; // 创建窗口类
	HWND hWnd;
	MSG msg;
	int errorCode;

	wc.cbClsExtra = 0; // TODOs
	wc.cbSize = sizeof(WNDCLASSEX); // 窗口内存大小
	wc.cbWndExtra = 0; // TODO
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // 窗口颜色，须转化为HBRUSH类型
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_NOCURSOR)); // 光标
	// LoadCursor(NULL, IDC_ARROW)：加载系统光标
	// LoadCursor(hInstance, MAKEINTRESOURCE(IDC_EXAMPLE))：加载自定义光标，IDC_EXAMPLE为头文件中设置的资源ID
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLOCKICON)); // 底栏图标
	// LoadIcon(NULL, IDI_APPLICATION)：加载系统图标
	// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXAMPLE))：加载自定义图标
	wc.hIconSm = NULL; // 左上角图标
	// NULL：使用hIcon指定的图标
	wc.hInstance = hInstance; // 窗口句柄
	wc.lpfnWndProc = MsgProc; // 接受程序回调函数
	wc.lpszClassName = "ELS"; // 该窗口实例的名字（debug：将项目属性中的字符集改为多字节字符集）
	wc.lpszMenuName = NULL; // 菜单名字
	wc.style = CS_HREDRAW | CS_VREDRAW; // 窗口样式。|是按位或运算符
	// CS_HREDRAW和CS_VREDRAW分别是水平刷新和垂直刷新，即窗口大小变化时会重绘活动区域

	// 将窗口类进行注册，注册失败返回0
	if (0 == RegisterClassEx(&wc)) {
		// 获取错误码（可以根据错误码在工具选项中查看错误类型）
		errorCode = GetLastError();
		return 0;
	}

	// 创建窗口，创建失败返回NULL，创建成功返回该新窗口的句柄
	hWnd = CreateWindowEx(
		WS_EX_TOPMOST, // 窗口置顶
		"ELS", // 窗口类名
		"Tetris", // 窗口显示名
		WS_OVERLAPPEDWINDOW, // 窗口风格
		500, 50, // 窗口初始位置
		435, 698, // 窗口初始宽高
		NULL, // 父窗口句柄
		NULL, // 菜单窗口句柄
		hInstance, // 当前程序句柄
		NULL // TODO
	);
	if (NULL == hWnd)
		return 0;

	// 设置窗口状态。若使得窗口可见则返回非0，不可见则返回0
	// 这里将上面创建的窗口进行显示
	ShowWindow(hWnd, nCmdShow);

	// 消息循环
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


// 程序回调函数：窗口句柄、消息ID、消息附加参数
LRESULT CALLBACK MsgProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps;
	HDC hDc;
	KEY Key(hWnd);

	switch (nMsg)
	{
	case WM_CREATE: // 该消息在程序初始时产生一次
		Init(hWnd);
		break;

	case WM_KEYDOWN: // 按键消息
		Key.run(wParam);
		break;

	case WM_TIMER: // 定时器消息
		OnTimer(hWnd);
		break;

	case WM_PAINT: // 该消息在窗口重绘时产生
		hDc = BeginPaint(hWnd, &ps); // 表明开始重绘，同时获取重绘区域的句柄
		UpdateFrame(hDc); // 重绘句柄hDc对应区域
		EndPaint(hWnd, &ps); // 表明结束重绘，同时释放句柄
		break;

	case WM_COMMAND: // 接收按钮信息
		if (RETRY_BUTTON == wParam)
			RetryGame(hWnd);
		else if (STOP_BUTTON == wParam)
			Key.run(VK_RETURN);
		else
			ChooseLevel(hWnd, wParam);
		SetFocus(hWnd); // 使主窗口重新获得焦点
		break;

	case WM_DESTROY: // 该消息在点击×时产生，用于让程序退出
		// 清除定时器
		KillTimer(hWnd, TIMER01);
		// 发出退出消息循环的消息
		PostQuitMessage(0);
		break;
	}
	// 让系统处理一些默认消息
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}


/* NOTE
 1. #表示预处理命令
 2. *和&的用法
	int q=3; // &q取q的地址，*&q取地址&q指向的值
	int* p = &q; // 创建一个值为地址&q（或指向整数3）的指针p，p指代地址，*p指代3这个值
 3. error LNK2019: 无法解析的外部符号
	https://www.pianshen.com/article/67621179837/
 4. win32采用消息循环机制作为事件处理的方式
	 ① 每一个操作都会产生一个消息，不同操作的消息不一样
	 ② 产生的消息在FIFO队列中排列
	 ③ 通过从队列中取出消息来处理事件
	BOOL GetMessage(lpMsg, hWnd, a, b)：取消息，失败返回false
	 - lpMsg：用于接受消息结构体（实际的返回值）
	 - hWnd：指定接受哪个窗口的消息，NULL表示接受任意窗口的消息
	 - a和b：指定消息接受范围，0和0表示接受任何消息
	BOOL TranslateMessage(lpMsg)：将不能直接处理的消息转化为字符消息，并将字符消息加入到消息队列（不修改lpMsg指向的原消息）
	LONG DispatchMessage(lpMsg)：将lpMsg指向的消息（可直接处理的消息或被翻译后的消息）分发给窗口程序的回调函数
 5. 头文件负责声明和一些宏定义，在预处理时会替换预处理指令#include
	头文件负责告诉有这些个玩意可以使用，相当于一种约束
	只需要引入头文件即可，不需要引入源文件，源文件根据实际代码的引用在链接时被引入
 6. 防止重复引入头文件
	方法一：#ifndef ... #endif
	方法二：手动检查
 7. 兼容性DC：在内存创建虚拟窗口来进行绘制，然后一次性将绘制好的内容复制到实际窗口上，以防止闪动
	HDC CreateCompatibleDC(hDc)：根据传入的实际窗口在内存中创建虚拟窗口，并返回虚拟窗口的句柄
	HBITMAP CreateCompatibleBitmap(hDc, 300, 600)：根据传入的实际窗口句柄在内存中创建指定大小的画布，并返回画布的ID
	SelectObject(hMemDc, hMemBitMap)：将虚拟窗口和画布关联起来
	Rectangle(hMemDc, 0, 0, 300, 600)：在虚拟窗口画矩形
	BitBlt(hDc, 0, 0, 300, 600, hMemDc, 0, 0, SRCCOPY)：将虚拟窗口绘制的内容复制到实际窗口上
	DeleteObject(hMemBitMap)：清除画布
	DeleteDC(hMemDc)：清除虚拟窗口的句柄
 8. GetMessage函数的参数2如果是hWnd，则只会响应hWnd指定窗口的消息，如果焦点被主程序捕获，则会使该窗口处于无法响应的状态
 9. 自动改值的问题记录：
	 ① 奇怪：记录下一个随机方块数组nextBlockArray的数值都自动变成了2。
	 ② 奇怪：全局变量nextBlockType的值被自动改变了。
	理解：猜测与内存有关。
	解决：在被改变的地方，用临时变量先记录起来，然后恢复成正确的值。
 */
