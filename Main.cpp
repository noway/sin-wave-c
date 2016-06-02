#include <Windows.h>
#define _USE_MATH_DEFINES

#include <math.h>


double angl = 0;
int paintIsPending = 0;

POINT* pts = NULL;
HBRUSH blue = NULL;
HBRUSH white = (HBRUSH)COLOR_WINDOW;
HPEN white_pen = NULL;
HPEN black_pen = NULL;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE: {
						RECT viewport;
						GetClientRect(hwnd, &viewport);
						pts = (POINT*) malloc(sizeof(POINT)* (viewport.bottom + 2));
						blue = CreateSolidBrush(RGB(0, 0, 255));
						white_pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						black_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

						break;
	}
	case WM_ERASEBKGND: {
							break;
	}
	case WM_TIMER: {
					  if (!paintIsPending)
					  {
						  paintIsPending = TRUE;
						  InvalidateRect(hwnd, NULL, true);
						  UpdateWindow(hwnd);
					  }
					  break;
	}
	case WM_PAINT: {
					 RECT viewport;
					 GetClientRect(hwnd, &viewport);

					 pts[0].x = viewport.right - 1;
					 pts[0].y = viewport.bottom - 1;

					 pts[1].x = viewport.right - 1;
					 pts[1].y = 0;
					 int i = 0;
					do {
						double angle = (double)i / viewport.bottom * M_PI * 2 + angl;

						pts[i + 2].x = (int)((sin(angle) + 1) / 2 * viewport.right);
						pts[i + 2].y = i;
						i += 5;
					} while (i < viewport.bottom);

					 angl += 1.0 / 180 * M_PI;

					 if (angl > M_PI*2) {
						 angl = 0;
					 }

					 //InvalidateRect(hwnd, NULL, TRUE);
					 PAINTSTRUCT ps;
					 HDC hdc = BeginPaint(hwnd, &ps);

					 HDC  Memhdc = CreateCompatibleDC(hdc);
					 HBITMAP  Membitmap = CreateCompatibleBitmap(hdc, viewport.right, viewport.bottom);

					 SelectObject(hdc, white_pen);
					 SelectObject(hdc, white);
					 Rectangle(hdc, 0, 0, viewport.right, viewport.bottom);
					 SelectObject(hdc, black_pen);
					 SelectObject(hdc, blue);
					 Polygon(hdc, pts, viewport.bottom + 2);

					 BitBlt(hdc, 0, 0, viewport.right, viewport.bottom, Memhdc, 0, 0, SRCCOPY);

					 DeleteObject(Membitmap);
					 DeleteDC(Memhdc);
					 DeleteDC(hdc);

					 EndPaint(hwnd, &ps);


					 paintIsPending = FALSE;
					 break;
	}
	case WM_CLOSE: {
					 DestroyWindow(hwnd);
					 break;
	}
	case WM_DESTROY: {
					   PostQuitMessage(0);
					   free(pts);
					   pts = 0;
					   break;
	}
	default: {
			   return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR params, int nCmdShow)
{
	const wchar_t g_szClassName[] = L"LabWeek12";

	MSG message;
	HWND hwnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;// (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, L"Error Registering Window!", L"Error", MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, L"COMP500/ENSE501", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);

	UINT_PTR myTimer = SetTimer(hwnd, 1005, 1000 / 20, NULL);
	if (NULL == hwnd)
	{
		MessageBox(0, L"Error Creating Window!", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	KillTimer(hwnd, myTimer);

	return message.wParam;
}