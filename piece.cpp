#include "piece.h"
#include <Windows.h>

HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD n;

piece::piece(int num)
{
	piece_type = num;
}

void piece::print()
{
	switch (piece_type)
	{
	case 1:
	{
		wchar_t wking[] = L"\u2723"; //pawn
		WriteConsoleW(cons, wking, wcslen(wking), &n, NULL);
		break;
	}
	case 2:
	{
		wchar_t wking[] = L"\u263B"; //pawn
		WriteConsoleW(cons, wking, wcslen(wking), &n, NULL);
		break;
	}
	case 3:
	{
		wchar_t wking[] = L"\u272F"; //pawn
		WriteConsoleW(cons, wking, wcslen(wking), &n, NULL);
		break;
	}
	case 4:
	{
		wchar_t wking[] = L"\u274F"; //pawn
		WriteConsoleW(cons, wking, wcslen(wking), &n, NULL);
		break;
	}
	case 5:
	{
		wchar_t wking[] = L"\u273F"; //pawn
		WriteConsoleW(cons, wking, wcslen(wking), &n, NULL);
		break;
	}
	}
}

piece::~piece()
{
}
