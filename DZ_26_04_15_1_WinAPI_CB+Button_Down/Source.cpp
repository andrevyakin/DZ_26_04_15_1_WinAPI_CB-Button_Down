#include<Windows.h>
#include<tchar.h>
#include<string>

#define ID_ComboBox 1001
#define ID_Button 1002

static TCHAR WindowsClass[] = L"win32app";
static TCHAR Title[] = L"MyApp";
HINSTANCE hinst;
RECT desktop, cr;
LRESULT cur_sel, count;

HWND combo, button;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	/*wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);*/
	wcex.hbrBackground = CreateSolidBrush(RGB(10, 128, 256));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WindowsClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, L"Call faild!", L"MyApp", NULL);
		return 1;
	}

	hinst = hinstance;
	GetWindowRect(GetDesktopWindow(), &desktop);
	HWND hWnd = CreateWindow(
		WindowsClass,
		Title,
		WS_OVERLAPPEDWINDOW,
		desktop.right / 4,
		desktop.bottom / 6,
		960,
		540,
		NULL,
		NULL,
		hinst,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"Create window faild!", L"MyApp", NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_ComboBox && HIWORD(wParam) == CBN_SELENDOK) {
			cur_sel = SendMessage(combo, CB_GETCURSEL, 0, 0);									//�������� ����� ��������� �������
			count = SendMessage(combo, CB_GETCOUNT, 0, 0);										//������ ���������� ����� � ������
			if (cur_sel < count - 1)
				EnableWindow(button, TRUE);														//������������ ������
		}

	case BN_CLICKED:

		if (LOWORD(wParam) == ID_Button){
			if (cur_sel < count -1){
				wchar_t str[40];
				SendMessage(combo, CB_GETLBTEXT, cur_sel, (LPARAM)str);							//��������� ������ �� ��������� �������
				SendMessage(combo, CB_DELETESTRING, cur_sel, 0);								//������� ��������� ������
				SendMessage(combo, CB_INSERTSTRING, cur_sel + 1, (LPARAM)str);					//�������� ��������� ������ � ������ �������
				SendMessage(combo, CB_SETCURSEL, ++cur_sel, 0);									//���������� ������ � ��� �������
			}
			else
			{
				MessageBox(hWnd, L"��� ��������� ������� � ������", L"", MB_OK);				
				EnableWindow(button, FALSE);													//�������������� ������
			}
		}

		break;

	case WM_CREATE:
		
		GetClientRect(hWnd, &cr);

		combo = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"combobox",
			L"",
			WS_CHILD | WS_VISIBLE | CBS_SIMPLE,
			cr.right / 8,
			cr.bottom / 8,
			cr.right / 2,
			cr.bottom / 1.5,
			hWnd,
			(HMENU)ID_ComboBox,
			hinst,
			NULL);
		
		SendMessage(combo, WM_SETTEXT, 0, (LPARAM)L"�������� ������ �� ����������� ������");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"������ ������� �����������,");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"���������� �����");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"� �������� �����������...");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"�� ��������� ���� � ������,");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"� ��� �� ���������...");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"����� ������ ����");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"��� ������ �������...");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"������ ����, ����� ������,");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"�������� ���� ��� � �����...");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"��� � ��� �����,");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"��� � ��� ����...");
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"(�) \"LUMEN\"");
		
		button = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"button",
			L"����",
			WS_CHILD | WS_VISIBLE,
			cr.right / 8 * 6,
			cr.bottom / 8,
			100,
			25,
			hWnd,
			(HMENU)ID_Button,
			hinst, NULL);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}