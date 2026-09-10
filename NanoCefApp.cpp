#include "NanoCefApp.hpp"
#include "NanoFileSchemeHandlerFactory.hpp"

CefRefPtr<CefBrowserProcessHandler> NanoCefApp::GetBrowserProcessHandler() {
	return this;
}

void NanoCefApp::OnContextInitialized() {

	CefRegisterSchemeHandlerFactory("http", "disk", new NanoFileSchemeHandlerFactory());
}
