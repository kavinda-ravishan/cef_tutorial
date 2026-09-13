#include <include/wrapper/cef_closure_task.h>
#include <include/base/cef_callback.h>
#include "NanoCefApp.hpp"
#include "NanoFileSchemeHandlerFactory.hpp"

CefRefPtr<CefBrowserProcessHandler> NanoCefApp::GetBrowserProcessHandler() {
	return this;
}

CefRefPtr<CefRenderProcessHandler> NanoCefApp::GetRenderProcessHandler() {
	return this;
}

void NanoCefApp::OnContextInitialized() {
	CefRegisterSchemeHandlerFactory("http", "disk", new NanoFileSchemeHandlerFactory());
}

void NanoCefApp::OnContextCreated(CefRefPtr<CefBrowser> p_browser, CefRefPtr<CefFrame> p_frame, CefRefPtr<CefV8Context> p_v8_context) {
	p_v8_context->GetGlobal()->SetValue(_sync_func_name, CefV8Value::CreateFunction(_sync_func_name, this), V8_PROPERTY_ATTRIBUTE_NONE);
	p_v8_context->GetGlobal()->SetValue(_async_func_name, CefV8Value::CreateFunction(_async_func_name, this), V8_PROPERTY_ATTRIBUTE_NONE);
}

bool NanoCefApp::Execute(const CefString& name, CefRefPtr<CefV8Value> p_object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& p_retval, CefString& exception) {

	if (name == _sync_func_name) {
		SyncCefJSFunc(arguments, p_retval, exception);
	}
	else if (name == _async_func_name) {
		AsyncCefJSFunc(arguments, p_retval, exception);
	}

	return true;
}

void NanoCefApp::SyncCefJSFunc(const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& p_retval, CefString& exception) {
	const std::string msg_box_txt = arguments[0]->GetStringValue().ToString();
	const int ret = MessageBoxA(nullptr, msg_box_txt.c_str(), "Sync CEF Function Called from JS side", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_YESNOCANCEL);

	bool ret_val = false;

	switch (ret) {
	case IDYES:
		ret_val = true;
		break;
	case IDNO:
		ret_val = false;
		break;
	case IDCANCEL:
	default:
		exception = "Exception: Cancelled";
		break;
	}

	p_retval = CefV8Value::CreateBool(ret_val);
}

void NanoCefApp::AsyncCefJSFunc(const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& p_retval, CefString& exception) {
	
	const auto invocation_id = _next_invocation_id++;
	_Invocation& invocation = _invocations[invocation_id];

	const std::string msg_box_txt = arguments[0]->GetStringValue().ToString();
	invocation.p_accept = arguments[1];
	invocation.p_reject = arguments[2];

	invocation.p_v8_context = CefV8Context::GetCurrentContext();

	invocation.task = std::async([this, msg_box_txt, invocation_id]() {
		const int msg_box_ret_val = 
			MessageBoxA(
				nullptr, msg_box_txt.c_str(), "Async CEF Function Called from JS side", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_YESNOCANCEL
			);
		
		CefPostTask(TID_RENDERER, base::BindOnce(&NanoCefApp::ResolveAsyncCef, this, msg_box_ret_val, invocation_id));
	});
}

void NanoCefApp::ResolveAsyncCef(const int msg_box_ret_val, const uint32_t invocation_id) {

	auto& invocation = _invocations[invocation_id];

	invocation.p_v8_context->Enter();

	switch (msg_box_ret_val) {
	case IDYES:
		invocation.p_accept->ExecuteFunction({}, CefV8ValueList{ CefV8Value::CreateBool(true) });
		break;
	case IDNO:
		invocation.p_accept->ExecuteFunction({}, CefV8ValueList{ CefV8Value::CreateBool(false) });
		break;
	case IDCANCEL:
	default:
		invocation.p_reject->ExecuteFunction({}, CefV8ValueList{ CefV8Value::CreateString("Exception: Cancelled") });
		break;
	}

	invocation.p_v8_context->Exit();
	_invocations.erase(invocation_id);
}
