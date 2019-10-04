#pragma once
#include "my.h"
#include "stdafx.h"

extern HINSTANCE hInst;
INT_PTR CALLBACK	DlgMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgPush(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgPop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgTop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

enum class DialogType { Add, Get, Del };

class View {
public:
	void Paint(HWND hDlg, std::string str_to_out_iter, std::string str_to_first, std::string str_to_last, HFONT hFont);

	void MakeDialog(HINSTANCE hInst, HWND hDlg, DialogType type);

	void PrintText(HWND hDlg, int ID, const char* outBuff);

	void Line(HDC hdc, int X0, int Y0, int X, int Y);
};