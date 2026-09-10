#pragma once
#include <include/cef_scheme.h>

class NanoFileSchemeHandlerFactory : public CefSchemeHandlerFactory {
	IMPLEMENT_REFCOUNTING(NanoFileSchemeHandlerFactory);

public:
    CefRefPtr<CefResourceHandler> Create(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& scheme_name,
        CefRefPtr<CefRequest> request) override;
};
