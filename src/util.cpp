#include "util.h"

// ��ӡ
void printf(const char* strOutputString, ...) 
{
	LPTSTR strBuffer = new TCHAR[99]; // �����ַ�������
	va_list vlArgs; // �������
	va_start(vlArgs, strOutputString); // ָ����κ͹̲�
	wsprintf(strBuffer, strOutputString, vlArgs); // ���ַ���д�뻺����
	va_end(vlArgs); // �ͷű��
	OutputDebugString(strBuffer); // �����������ݴ�ӡ
}