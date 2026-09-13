#include <filesystem>
#include "NanoBrowserWindow.hpp"
#include "NanoCefApp.hpp"

int main() {

	const auto h_instance = GetModuleHandle(NULL);

	const CefMainArgs main_args{ h_instance };
	CefRefPtr<NanoCefApp> p_app = new NanoCefApp{};

	if (const auto code = CefExecuteProcess(main_args, p_app, nullptr); code >= 0) {
		return (int)code;
	}

	CefSettings settings;
	settings.multi_threaded_message_loop = true;
	settings.no_sandbox = true;
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
