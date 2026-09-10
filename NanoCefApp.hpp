#pragma once
#include <include/cef_app.h>

class NanoCefApp : public CefApp, public CefBrowserProcessHandler {
	IMPLEMENT_REFCOUNTING(NanoCefApp);

public:
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

	void OnContextInitialized() override;
};
