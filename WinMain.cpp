#define NOMINMAX
#include <Windows.h>

#include <include/cef_app.h>
#include <include/cef_client.h>

class NanoCefApp : public CefApp {
	IMPLEMENT_REFCOUNTING(NanoCefApp);
};

int WINAPI wWinMain(_In_ HINSTANCE h_instance, _In_opt_ HINSTANCE h_prev_instance, _In_ PWSTR p_cmd_line, _In_ int n_cmd_show) {

	const CefMainArgs main_args{ h_instance };
	CefRefPtr<NanoCefApp> p_app = new NanoCefApp{};

	if (const auto code = CefExecuteProcess(main_args, p_app, nullptr); code >= 0) {
		return (int)code;
	}

	return 0;
}
