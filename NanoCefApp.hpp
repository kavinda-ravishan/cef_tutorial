#pragma once
#include <future>
#include <unordered_map>
#include <include/cef_app.h>

class NanoCefApp : 
	public CefApp, 
	public CefBrowserProcessHandler, 
	public CefRenderProcessHandler, 
	public CefV8Handler
{
	IMPLEMENT_REFCOUNTING(NanoCefApp);
private:
	struct _Invocation {
		CefRefPtr<CefV8Value> p_accept;
		CefRefPtr<CefV8Value> p_reject;
		CefRefPtr<CefV8Context> p_v8_context;
		std::future<void> task;
	};

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

private:
	const CefString _sync_func_name{ "SyncCefJSFunc" };
	const CefString _async_func_name{ "AsyncCefJSFunc" };

	uint32_t _next_invocation_id{ 0 };
	std::unordered_map<uint32_t, _Invocation> _invocations{};

private:
	void SyncCefJSFunc(const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& p_retval, CefString& exception);
	void AsyncCefJSFunc(const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& p_retval, CefString& exception);
	void ResolveAsyncCef(const int msg_box_ret_val, const uint32_t invocation_id);
};
