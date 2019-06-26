// Win32Project2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "tic_tac_toe.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    /*LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);*/
	_tcscpy_s(szTitle, _T("Крестики нолики"));
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      600, 200, 500, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

#ifdef UNICODE
typedef std::wstringstream streamOut;
#else
typedef std::stringstream streamOut;
#endif

void botMove(UINT[]);
bool isWinner(UINT[], int);
void MessageOut(TCHAR*, UINT[], int &, streamOut &, int &);

TCHAR *player1 = TEXT("Ваш ход");
TCHAR *player2 = TEXT("Ход компьютера");
streamOut str_win_player1, str_win_player2, str_draw;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HPEN hPen, hPen1;
	static POINT pstart;
	static UINT field[9] = { 0 }, x, y;
	static int  move = 0, msg = 0, win_player1 = 0, win_player2 = 0, draw = 0,  run_player = 1;
	switch (message)
	{
		case WM_CREATE:
			srand(time(NULL));
			str_win_player1.str(TEXT(""));
			str_win_player1.clear();
			str_win_player1 << win_player1;
			str_win_player2.str(TEXT(""));
			str_win_player2.clear();
			str_win_player2 << win_player2;
			str_draw.str(TEXT(""));
			str_draw.clear();
			str_draw << draw;
			break;
		case WM_LBUTTONUP:
			pstart.x = LOWORD(lParam);
			pstart.y = HIWORD(lParam);
			if (pstart.x > 25 && pstart.x < 95)
				x = 0;
			else if (pstart.x > 95 && pstart.x < 165)
				x = 1;
			else if (pstart.x > 165 && pstart.x < 235)
				x = 2;
			else
				break;
			if (pstart.y > 25 && pstart.y < 95)
				y = 0;
			else if (pstart.y > 95 && pstart.y < 165)
				y = 1;
			else if (pstart.y > 165 && pstart.y < 235)
				y = 2;
			else
				break;
			if (!field[x + 3 * y] && run_player && move < 9)
			{
				field[x + 3 * y] = 1;
				move++;
				
				if (isWinner(field, 1))
				{
					move = 9;
					InvalidateRect(hWnd, NULL, true);
					MessageOut(L"Вы выиграли\nиграем ещё?", field, move, str_win_player1, win_player1);
				}
				else if (move > 8)
				{
					InvalidateRect(hWnd, NULL, true);
					MessageOut(L"Ничья\nиграем ещё", field, move, str_draw, draw);
					
				}
				run_player = !run_player;
				InvalidateRect(hWnd, NULL, true);
				return 0;
			}
			break;
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			if (run_player)
				TextOut(hdc, 100, 8, player1, lstrlen(player1));
			else
			{
				TextOut(hdc, 80, 8, player2, lstrlen(player2));
			}
			TextOut(hdc, 335, 30, L"Счет игр", lstrlen(L"Счет игр"));
			TextOut(hdc, 260, 50, L"Вы               - ", lstrlen(L"Вы               - "));
			TextOut(hdc, 260, 70, L"Компьютер  - ", lstrlen(L"Компьютер  - "));
			TextOut(hdc, 260, 90, L"Ничьих        - ", lstrlen(L"Ничьих        - "));
			TextOut(hdc, 360, 50, str_win_player1.str().c_str(), str_win_player1.str().length());
			TextOut(hdc, 360, 70, str_win_player2.str().c_str(), str_win_player2.str().length());
			TextOut(hdc, 360, 90, str_draw.str().c_str(), str_draw.str().length());
			hPen = CreatePen(PS_SOLID, 3, RGB(139, 69, 19));
			SelectObject(hdc, hPen);
			MoveToEx(hdc, 25, 25, NULL);
			LineTo(hdc, 235, 25);
			LineTo(hdc, 235, 235);
			LineTo(hdc, 25, 235);
			LineTo(hdc, 25, 25);
			hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			SelectObject(hdc, hPen);
			MoveToEx(hdc, 95, 25, NULL);
			LineTo(hdc, 95, 235);
			MoveToEx(hdc, 165, 25, NULL);
			LineTo(hdc, 165, 235);
			MoveToEx(hdc, 25, 95, NULL);
			LineTo(hdc, 235, 95);
			MoveToEx(hdc, 25, 165, NULL);
			LineTo(hdc, 235, 165);
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, hPen);
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (field[i * 3 + j] == 1)
					{
						MoveToEx(hdc, 40 + 70 * j, 40 + 70 * i, NULL);
						LineTo(hdc, 80 + 70 * j, 80 + 70 * i);
						MoveToEx(hdc, 40 + 70 * j, 80 + 70 * i, NULL);
						LineTo(hdc, 80 + 70 * j, 40 + 70 * i);
					}
					else if (field[i * 3 + j] == 2)
					{
						Ellipse(hdc, 40 + 70 * j, 40 + 70 * i, 80 + 70 * j, 80 + 70 * i);
					}
				}
			}

			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	if (!run_player && move < 9)
	{
		move++;
		botMove(field);
		if (isWinner(field, 2))
		{
			move = 9;
			InvalidateRect(hWnd, NULL, true);
			MessageOut(L"Компьютер выиграл\nиграем ещё?", field, move, str_win_player2, win_player2);
		}
		else if (move > 8)
		{
			InvalidateRect(hWnd, NULL, true);
			MessageOut(L"Ничья\nиграем ещё", field, move, str_draw, draw);
		}
		run_player = !run_player;
		InvalidateRect(hWnd, NULL, true);
	}
    return 0;
}

void MessageOut( TCHAR *Message, UINT field[], int & move, streamOut & str, int & player)
{
	int res = MessageBox(NULL, Message, L"Игра окончена", MB_YESNO);
	switch (res)
	{
	case IDYES:
		for (int i = 0; i < 9; i++)
		{
			field[i] = 0;
		}
		move = 0;
		str.str(TEXT(""));
		str.clear();
		str << ++player;
		break;
	case IDNO:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void botMove(UINT field[]) 
{
	Sleep(1000);
	for (int i = 0; i < 9; i++)
		if (field[i] == 0) 
		{
			field[i] = 2;
			if (isWinner(field, 2))
				return;
			else
				field[i] = 0;
		}
	for (int i = 0; i < 9; i++)
		if (field[i] == 0) 
		{
			field[i] = 1;
			if (isWinner(field, 1))
			{
				field[i] = 2;
				return;
			}
			else
				field[i] = 0;
		}
	int j, k = 0;
	while (field[j = (rand() % 9)])
	{
		k++;
		if (k > 100)
		{
			for (int i = 0; i < 9; i++)
				if (!field[i])
				{
					field[i] = 2;
					return;
				}
		}
	}
	field[j] = 2;
}

bool isWinner(UINT field[], int who)
{
	if (field[0] == who && field[1] == who && field[2] == who) return true;
	if (field[3] == who && field[4] == who && field[5] == who) return true;
	if (field[6] == who && field[7] == who && field[8] == who) return true;
	if (field[0] == who && field[3] == who && field[6] == who) return true;
	if (field[1] == who && field[4] == who && field[7] == who) return true;
	if (field[2] == who && field[5] == who && field[8] == who) return true;
	if (field[0] == who && field[4] == who && field[8] == who) return true;
	if (field[2] == who && field[4] == who && field[6] == who) return true;
	return false;
}