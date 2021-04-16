#include "util.h"

// 打印
void printf(const char* strOutputString, ...) 
{
	LPTSTR strBuffer = new TCHAR[99]; // 创建字符缓冲区
	va_list vlArgs; // 创建变参
	va_start(vlArgs, strOutputString); // 指定变参和固参
	wsprintf(strBuffer, strOutputString, vlArgs); // 将字符串写入缓冲区
	va_end(vlArgs); // 释放变参
	OutputDebugString(strBuffer); // 将缓冲区内容打印
}