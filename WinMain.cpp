#include <filesystem>
#include "NanoBrowserWindow.hpp"
#include "NanoCefApp.hpp"

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

	CreateBrowserWindow(h_instance);

	MSG msg;
	while (GetMessageA(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	CleanupBrowserWindow(h_instance);
	CefShutdown();

	return (int)msg.wParam;
}
