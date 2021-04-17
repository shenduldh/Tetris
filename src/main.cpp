#include "resource.h"
#include "main.h"


// �����ص�����
LRESULT CALLBACK MsgProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


/* WinMain��Win32����ָ����ں���
** WINAPI������Э�飬����Լ���ڵ��ú���ʱ��������ջ����__cdecl��__stdcall���֣�WINAPI��__stdcall�ĺ궨��
** hInstance����ǰ�����ʵ�����
** hPreInstance����ǰ�����ǰ��һ������ľ��
** lpCmdLine�����ݸ�����������в���
** nCmdShow��ָ�����ڵ���ʾ��ʽ
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; // ����������
	HWND hWnd;
	MSG msg;
	int errorCode;

	wc.cbClsExtra = 0; // TODOs
	wc.cbSize = sizeof(WNDCLASSEX); // �����ڴ��С
	wc.cbWndExtra = 0; // TODO
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // ������ɫ����ת��ΪHBRUSH����
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_NOCURSOR)); // ���
	// LoadCursor(NULL, IDC_ARROW)������ϵͳ���
	// LoadCursor(hInstance, MAKEINTRESOURCE(IDC_EXAMPLE))�������Զ����꣬IDC_EXAMPLEΪͷ�ļ������õ���ԴID
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLOCKICON)); // ����ͼ��
	// LoadIcon(NULL, IDI_APPLICATION)������ϵͳͼ��
	// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXAMPLE))�������Զ���ͼ��
	wc.hIconSm = NULL; // ���Ͻ�ͼ��
	// NULL��ʹ��hIconָ����ͼ��
	wc.hInstance = hInstance; // ���ھ��
	wc.lpfnWndProc = MsgProc; // ���ܳ���ص�����
	wc.lpszClassName = "ELS"; // �ô���ʵ�������֣�debug������Ŀ�����е��ַ�����Ϊ���ֽ��ַ�����
	wc.lpszMenuName = NULL; // �˵�����
	wc.style = CS_HREDRAW | CS_VREDRAW; // ������ʽ��|�ǰ�λ�������
	// CS_HREDRAW��CS_VREDRAW�ֱ���ˮƽˢ�ºʹ�ֱˢ�£������ڴ�С�仯ʱ���ػ�����

	// �����������ע�ᣬע��ʧ�ܷ���0
	if (0 == RegisterClassEx(&wc)) {
		// ��ȡ�����루���Ը��ݴ������ڹ���ѡ���в鿴�������ͣ�
		errorCode = GetLastError();
		return 0;
	}

	// �������ڣ�����ʧ�ܷ���NULL�������ɹ����ظ��´��ڵľ��
	hWnd = CreateWindowEx(
		WS_EX_TOPMOST, // �����ö�
		"ELS", // ��������
		"Tetris", // ������ʾ��
		WS_OVERLAPPEDWINDOW, // ���ڷ��
		500, 50, // ���ڳ�ʼλ��
		435, 698, // ���ڳ�ʼ���
		NULL, // �����ھ��
		NULL, // �˵����ھ��
		hInstance, // ��ǰ������
		NULL // TODO
	);
	if (NULL == hWnd)
		return 0;

	// ���ô���״̬����ʹ�ô��ڿɼ��򷵻ط�0�����ɼ��򷵻�0
	// ���ｫ���洴���Ĵ��ڽ�����ʾ
	ShowWindow(hWnd, nCmdShow);

	// ��Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


// ����ص����������ھ������ϢID����Ϣ���Ӳ���
LRESULT CALLBACK MsgProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps;
	HDC hDc;
	KEY Key(hWnd);

	switch (nMsg)
	{
	case WM_CREATE: // ����Ϣ�ڳ����ʼʱ����һ��
		Init(hWnd);
		break;

	case WM_KEYDOWN: // ������Ϣ
		Key.run(wParam);
		break;

	case WM_TIMER: // ��ʱ����Ϣ
		OnTimer(hWnd);
		break;

	case WM_PAINT: // ����Ϣ�ڴ����ػ�ʱ����
		hDc = BeginPaint(hWnd, &ps); // ������ʼ�ػ棬ͬʱ��ȡ�ػ�����ľ��
		UpdateFrame(hDc); // �ػ���hDc��Ӧ����
		EndPaint(hWnd, &ps); // ���������ػ棬ͬʱ�ͷž��
		break;

	case WM_COMMAND: // ���հ�ť��Ϣ
		if (RETRY_BUTTON == wParam)
			RetryGame(hWnd);
		else if (STOP_BUTTON == wParam)
			Key.run(VK_RETURN);
		else
			ChooseLevel(hWnd, wParam);
		SetFocus(hWnd); // ʹ���������»�ý���
		break;

	case WM_DESTROY: // ����Ϣ�ڵ����ʱ�����������ó����˳�
		// �����ʱ��
		KillTimer(hWnd, TIMER01);
		// �����˳���Ϣѭ������Ϣ
		PostQuitMessage(0);
		break;
	}
	// ��ϵͳ����һЩĬ����Ϣ
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}


/* NOTE
 1. #��ʾԤ��������
 2. *��&���÷�
	int q=3; // &qȡq�ĵ�ַ��*&qȡ��ַ&qָ���ֵ
	int* p = &q; // ����һ��ֵΪ��ַ&q����ָ������3����ָ��p��pָ����ַ��*pָ��3���ֵ
 3. error LNK2019: �޷��������ⲿ����
	https://www.pianshen.com/article/67621179837/
 4. win32������Ϣѭ��������Ϊ�¼�����ķ�ʽ
	 �� ÿһ�������������һ����Ϣ����ͬ��������Ϣ��һ��
	 �� ��������Ϣ��FIFO����������
	 �� ͨ���Ӷ�����ȡ����Ϣ�������¼�
	BOOL GetMessage(lpMsg, hWnd, a, b)��ȡ��Ϣ��ʧ�ܷ���false
	 - lpMsg�����ڽ�����Ϣ�ṹ�壨ʵ�ʵķ���ֵ��
	 - hWnd��ָ�������ĸ����ڵ���Ϣ��NULL��ʾ�������ⴰ�ڵ���Ϣ
	 - a��b��ָ����Ϣ���ܷ�Χ��0��0��ʾ�����κ���Ϣ
	BOOL TranslateMessage(lpMsg)��������ֱ�Ӵ������Ϣת��Ϊ�ַ���Ϣ�������ַ���Ϣ���뵽��Ϣ���У����޸�lpMsgָ���ԭ��Ϣ��
	LONG DispatchMessage(lpMsg)����lpMsgָ�����Ϣ����ֱ�Ӵ������Ϣ�򱻷�������Ϣ���ַ������ڳ���Ļص�����
 5. ͷ�ļ�����������һЩ�궨�壬��Ԥ����ʱ���滻Ԥ����ָ��#include
	ͷ�ļ������������Щ���������ʹ�ã��൱��һ��Լ��
	ֻ��Ҫ����ͷ�ļ����ɣ�����Ҫ����Դ�ļ���Դ�ļ�����ʵ�ʴ��������������ʱ������
 6. ��ֹ�ظ�����ͷ�ļ�
	����һ��#ifndef ... #endif
	���������ֶ����
 7. ������DC�����ڴ洴�����ⴰ�������л��ƣ�Ȼ��һ���Խ����ƺõ����ݸ��Ƶ�ʵ�ʴ����ϣ��Է�ֹ����
	HDC CreateCompatibleDC(hDc)�����ݴ����ʵ�ʴ������ڴ��д������ⴰ�ڣ����������ⴰ�ڵľ��
	HBITMAP CreateCompatibleBitmap(hDc, 300, 600)�����ݴ����ʵ�ʴ��ھ�����ڴ��д���ָ����С�Ļ����������ػ�����ID
	SelectObject(hMemDc, hMemBitMap)�������ⴰ�ںͻ�����������
	Rectangle(hMemDc, 0, 0, 300, 600)�������ⴰ�ڻ�����
	BitBlt(hDc, 0, 0, 300, 600, hMemDc, 0, 0, SRCCOPY)�������ⴰ�ڻ��Ƶ����ݸ��Ƶ�ʵ�ʴ�����
	DeleteObject(hMemBitMap)���������
	DeleteDC(hMemDc)��������ⴰ�ڵľ��
 8. GetMessage�����Ĳ���2�����hWnd����ֻ����ӦhWndָ�����ڵ���Ϣ��������㱻�����򲶻����ʹ�ô��ڴ����޷���Ӧ��״̬
 9. �Զ���ֵ�������¼��
	 �� ��֣���¼��һ�������������nextBlockArray����ֵ���Զ������2��
	 �� ��֣�ȫ�ֱ���nextBlockType��ֵ���Զ��ı��ˡ�
	��⣺�²����ڴ��йء�
	������ڱ��ı�ĵط�������ʱ�����ȼ�¼������Ȼ��ָ�����ȷ��ֵ��
 */
