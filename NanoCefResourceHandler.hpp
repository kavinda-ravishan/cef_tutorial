#pragma once
#include <string>
#include <fstream>
#include <include/cef_resource_handler.h>

class NanoCefResourceHandler: public CefResourceHandler {
IMPLEMENT_REFCOUNTING(NanoCefResourceHandler);

public:
    bool Open(
        CefRefPtr<CefRequest> p_request,
        bool& handle_request,
        CefRefPtr<CefCallback> p_callback) override;

    void GetResponseHeaders(
        CefRefPtr<CefResponse> p_response,
        int64_t& response_length,
        CefString& redirectUrl) override;

    void Cancel() override;

    bool Read(
        void* data_out,
        int bytes_to_read,
        int& bytes_read,
        CefRefPtr<CefResourceReadCallback> p_callback) override;

private:
    std::ifstream _file_stream;
    int64_t _file_size{ 0 };
    std::string _file_mime_type;
};
