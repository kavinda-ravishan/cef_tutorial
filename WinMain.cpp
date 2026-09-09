#define NOMINMAX
#include <Windows.h>

#include <filesystem>

#include <include/cef_app.h>
#include <include/cef_client.h>

class NanoCefApp : public CefApp {
	IMPLEMENT_REFCOUNTING(NanoCefApp);
};

class NanoCefClient : public CefClient {
	IMPLEMENT_REFCOUNTING(NanoCefClient);
};

CefRefPtr<NanoCefClient> g_p_client;

LRESULT CALLBACK BrowserWindowProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) {
	using namespace std::literals;
	
	switch (u_msg) {
	case WM_CREATE: {
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

		CefBrowserHost::CreateBrowser(info, g_p_client, "https://youtube.com"s, {}, {}, {});
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}

int WINAPI wWinMain(_In_ HINSTANCE h_instance, _In_opt_ HINSTANCE h_prev_instance, _In_ PWSTR p_cmd_line, _In_ int n_cmd_show) {

	const CefMainArgs main_args{ h_instance };
	CefRefPtr<NanoCefApp> p_app = new NanoCefApp{};

	if (const auto code = CefExecuteProcess(main_args, p_app, nullptr); code >= 0) {
		return (int)code;
	}

	CefSettings settings;
	settings.multi_threaded_message_loop = true;
	CefString(&settings.cache_path).FromString((std::filesystem::current_path() / "cef_cache").string());

	CefInitialize(main_args, settings, p_app, nullptr);

	constexpr const char* wnd_class_name = "$client-window$";
	HWND h_wnd_browser = nullptr;
	{
		WNDCLASSEXA wcex{};
		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = h_instance;
		wcex.lpszClassName = wnd_class_name;
		wcex.lpfnWndProc = BrowserWindowProc;
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

		RegisterClassExA(&wcex);

		h_wnd_browser = CreateWindowExA(
			0, wnd_class_name, "Nano CEF", 
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
			CW_USEDEFAULT, CW_USEDEFAULT, 
			1280, 720, 
			nullptr, nullptr, h_instance, nullptr
		);

		ShowWindow(h_wnd_browser, SW_SHOWDEFAULT);
		UpdateWindow(h_wnd_browser);
	}

	MSG msg;
	while (GetMessageA(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	CefShutdown();

	return 0;
}
