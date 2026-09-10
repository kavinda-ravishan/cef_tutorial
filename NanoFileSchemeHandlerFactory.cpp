#include "NanoCefResourceHandler.hpp"
#include "NanoFileSchemeHandlerFactory.hpp"

CefRefPtr<CefResourceHandler> NanoFileSchemeHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request) 
{
    return new NanoCefResourceHandler{};
}
