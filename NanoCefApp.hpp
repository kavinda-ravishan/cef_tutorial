#pragma once
#include <include/cef_app.h>

class NanoCefApp : 
	public CefApp, 
	public CefBrowserProcessHandler, 
	public CefRenderProcessHandler, 
	public CefV8Handler
{
	IMPLEMENT_REFCOUNTING(NanoCefApp);

public:
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

	void OnContextInitialized() override;

	void OnContextCreated(
		CefRefPtr<CefBrowser> p_browser,
		CefRefPtr<CefFrame> p_frame,
		CefRefPtr<CefV8Context> p_v8_context) override;

	bool Execute(
		const CefString& name,
		CefRefPtr<CefV8Value> p_object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& p_retval,
		CefString& exception) override;
};
