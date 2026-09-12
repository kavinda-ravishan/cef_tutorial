#pragma once
#include <include/cef_client.h>

class NanoCefClient : public CefClient, public CefLifeSpanHandler {
	IMPLEMENT_REFCOUNTING(NanoCefClient);

public:
	CefRefPtr<CefBrowser> GetBrowser();

	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;

	void OnAfterCreated(CefRefPtr<CefBrowser> p_browser) override;

	void OnBeforeClose(CefRefPtr<CefBrowser> p_browser) override;

	bool IsBrowserClosed() const;
private:
	CefRefPtr<CefBrowser> _p_browser;
	bool _browser_closed = false;
};
