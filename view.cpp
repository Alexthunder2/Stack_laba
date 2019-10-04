#include "stdafx.h"
#include "view.h"

void View::Paint(HWND hDlg, std::string str_to_out_iter, std::string str_for_first, std::string str_for_last, HFONT hFont) {
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hDlg, &ps);
	tagRECT r = { 220, 120, 450, 800 };
	SelectObject(hdc, hFont);
	PrintText(hDlg, IDC_STATIC4, str_to_out_iter.c_str());
	PrintText(hDlg, IDC_STATIC2, str_for_first.c_str());
	PrintText(hDlg, IDC_STATIC3, str_for_last.c_str());
	EndPaint(hDlg, &ps);
}

void View::MakeDialog(HINSTANCE hInst, HWND hDlg, DialogType type) {
	switch (type)
	{
	case DialogType::Add:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ADD), hDlg, DlgPush);
		break;
	case DialogType::Get:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_GET), hDlg, DlgTop);
		break;
	case DialogType::Del:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DEL), hDlg, DlgPop);
		break;
	default:
		break;
	}
}

void View::PrintText(HWND hDlg, int ID, const char* outBuff) {
	SetDlgItemText(hDlg, ID, outBuff);
}

void View::Line(HDC hdc, int X0, int Y0, int X, int Y) {
	MoveToEx(hdc, X0, Y0, NULL);
	LineTo(hdc, X, Y);
}
