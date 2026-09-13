#include "NanoCefClient.hpp"

CefRefPtr<CefBrowser> NanoCefClient::GetBrowser() { return _p_browser; }

CefRefPtr<CefLifeSpanHandler> NanoCefClient::GetLifeSpanHandler() { return this; }

CefRefPtr<CefDisplayHandler> NanoCefClient::GetDisplayHandler() { return this; }

bool NanoCefClient::OnConsoleMessage(
    CefRefPtr<CefBrowser> browser,
    cef_log_severity_t level,
    const CefString& message,
    const CefString& source,
    int line)
{
    // This receives your website's console.log calls in C++
    // You can print them to the console or pipe them to your own log file
    std::printf("JS Log: %s [Source: %s at line %d]\n",
        message.ToString().c_str(),
        source.ToString().c_str(),
        line);

    return true; // Return true to block the message to appear in the DevTools console
}

void NanoCefClient::OnAfterCreated(CefRefPtr<CefBrowser> p_browser) {
	_p_browser = p_browser;
}

void NanoCefClient::OnBeforeClose(CefRefPtr<CefBrowser> p_browser) {
    _p_browser.reset();
    _browser_closed = true;
}

bool NanoCefClient::IsBrowserClosed() const { return _browser_closed; }
