#include "NanoBrowserWindow.hpp"
#include "NanoCefClient.hpp"

static CefRefPtr<NanoCefClient> g_p_client;
static constexpr const char* g_wnd_class_name = "$client-window$";

LRESULT CALLBACK BrowserWindowProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) {
	using namespace std::literals;

	switch (u_msg) {
	case WM_CREATE:
	{
		g_p_client = new NanoCefClient{};

		RECT rect{};
		GetClientRect(hwnd, &rect);
		CefRect cef_rect{};
		cef_rect.x = rect.left;
		cef_rect.y = rect.top;
		cef_rect.width = rect.right - rect.left;
		cef_rect.height = rect.bottom - rect.top;

		CefWindowInfo info{};
		info.SetAsChild(hwnd, cef_rect);

		CefBrowserHost::CreateBrowser(info, g_p_client, "http://disk/index.html"s, {}, {}, {});
		//CefBrowserHost::CreateBrowser(info, g_p_client, "http://localhost:5173"s, {}, {}, {});
	}
	break;
	case WM_CLOSE:
		if (g_p_client && g_p_client->GetBrowser()) {
			g_p_client->GetBrowser()->GetHost()->CloseBrowser(false);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (w_param != SIZE_MINIMIZED && g_p_client) {
			if (auto p_browser = g_p_client->GetBrowser()) {
				if (auto h_wnd_browser = p_browser->GetHost()->GetWindowHandle()) {
					RECT rect{};
					GetClientRect(hwnd, &rect);
					SetWindowPos(h_wnd_browser, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
				}
			}
		}
		break;
	case WM_ERASEBKGND:
		if (g_p_client) {
			if (auto p_browser = g_p_client->GetBrowser()) {
				if (auto h_wnd_browser = p_browser->GetHost()->GetWindowHandle()) {
					return 1;
				}
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}

HWND CreateBrowserWindow(HINSTANCE h_instance) {

	HWND h_wnd_browser = nullptr;

	WNDCLASSEXA wcex{};
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = h_instance;
	wcex.lpszClassName = g_wnd_class_name;
	wcex.lpfnWndProc = BrowserWindowProc;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClassExA(&wcex);

	h_wnd_browser = CreateWindowExA(
		0, g_wnd_class_name, "Nano CEF",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 720,
		nullptr, nullptr, h_instance, nullptr
	);

	ShowWindow(h_wnd_browser, SW_SHOWDEFAULT);
	UpdateWindow(h_wnd_browser);

	return h_wnd_browser;
}

void CleanupBrowserWindow(HINSTANCE h_instance) {
	if (g_p_client) {
		while (!g_p_client->IsBrowserClosed()) {
			Sleep(10);
		}
	}

	g_p_client.reset();
	UnregisterClassA(g_wnd_class_name, h_instance);
}
