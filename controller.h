#pragma once
#include <memory>
#include <string>
#include "view.h"
#include "model.h"

template<typename T>
class Controller {
public:

	explicit Controller(Model<T>* model) : view_(), model_(model) {}

	int Start(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);

		if (!InitInstance(hInstance, nCmdShow))
		{
			return FALSE;
		}

		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY5));

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}

	void Main(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, HFONT hFont) {
		std::string str;
		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_BUTTON_1: // Add
				view_.MakeDialog(hInst, hDlg, DialogType::Add);
				break;
			case IDC_BUTTON_3: // Delete
				view_.MakeDialog(hInst, hDlg, DialogType::Del);
				break;
			case IDC_BUTTON_4: // Get
				view_.MakeDialog(hInst, hDlg, DialogType::Get);
				break;
			case ID_GAME_EXIT:
			case WM_DESTROY:
				DeleteObject(hFont);
				PostQuitMessage(0);
				break;
			}
			break;
		case WM_PAINT:
			view_.Paint(hDlg, model_->GetElementsString(), model_->GetStringForFirst(),
				model_->GetStringForLast(), hFont);
			break;
		}
	}

	void Push(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		MaxVisitor<std::string> max_visitor;
		MinVisitor<std::string> min_visitor;

		char buff[100] = ""; // here I put text from dialog box
		std::string elements_string = "", max_visitor_string = "", min_visitor_string = "";

		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemTextA(hDlg, IDC_EDIT1, buff, 100);
			model_->Push(std::string(buff));
			max_visitor_string = model_->Accept(max_visitor);
			min_visitor_string = model_->Accept(min_visitor);
			for (auto iter = model_->begin(); iter != model_->end(); iter++) {
				elements_string += *iter;
				elements_string += '\n';
			}

			model_->SetElementsString(elements_string);
			model_->SetStringForFirst(max_visitor_string);
			model_->SetStringForLast(min_visitor_string);
		case WM_DESTROY:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	void Pop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		MaxVisitor<std::string> max_visitor;
		MinVisitor<std::string> min_visitor;
		std::string elements_string = "", max_visitor_string = "", min_visitor_string = "";
		switch (LOWORD(wParam)) {
		case IDOK:
			model_->Pop();
			max_visitor_string = model_->Accept(max_visitor);
			min_visitor_string = model_->Accept(min_visitor);
			for (auto iter = model_->begin(); iter != model_->end(); iter++) {
				elements_string += *iter;
				elements_string += '\n';
			}

			model_->SetElementsString(elements_string);
			model_->SetStringForFirst(max_visitor_string);
			model_->SetStringForLast(min_visitor_string);
		case WM_DESTROY:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	void Top(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		T element;
		element = model_->Top();
		view_.PrintText(hDlg, IDC_STATIC1, element.c_str());
		switch (LOWORD(wParam)) {
		case WM_DESTROY:
		case IDC_BUTTON_2:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	void PrintTextOnView(HWND hDlg, const std::string& str) {
		view_.PrintText(hDlg, IDC_STATIC1, str.c_str());
	}

	const static HFONT hFont;

private:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
		hInst = hInstance;

		HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgMain);

		if (!hWnd)
		{
			return FALSE;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return TRUE;
	}

	HWND hMainDlg;
	View view_;
	Model<T>* model_;
};

template<typename T>
const HFONT Controller<T>::hFont = CreateFont(20, 0, 0, 0,
	FW_DONTCARE,
	FALSE, FALSE, FALSE, // курсив подчеркивание зачеркивание (прост поставить true)
	DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	VARIABLE_PITCH, TEXT("Areal")); // в ковычках можно указать какой-нибудь шрифт
