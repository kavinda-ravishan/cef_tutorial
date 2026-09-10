#include "NanoCefClien.hpp"

CefRefPtr<CefBrowser> NanoCefClient::GetBrowser() { return _p_browser; }

CefRefPtr<CefLifeSpanHandler> NanoCefClient::GetLifeSpanHandler() { return this; }

void NanoCefClient::OnAfterCreated(CefRefPtr<CefBrowser> p_browser) {
	_p_browser = p_browser;
}
