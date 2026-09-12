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
	const CefString func_name{ "CefJSFunc" };
	p_v8_context->GetGlobal()->SetValue(func_name, CefV8Value::CreateFunction(func_name, this), V8_PROPERTY_ATTRIBUTE_NONE);
}

bool NanoCefApp::Execute(const CefString& name, CefRefPtr<CefV8Value> p_object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& p_retval, CefString& exception) {

	const int ret = MessageBoxA(nullptr, arguments[0]->GetStringValue().ToString().c_str(), "Sample CEF Function Called from JS side", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_YESNOCANCEL);

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
		exception = "Cancelled";
		break;
	}

	p_retval = CefV8Value::CreateBool(ret_val);

	return true;
}
