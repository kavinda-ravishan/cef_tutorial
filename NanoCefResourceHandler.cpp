#include <filesystem>
#include <include/cef_parser.h>
#include "NanoCefResourceHandler.hpp"

bool NanoCefResourceHandler::Open(
    CefRefPtr<CefRequest> p_request,
    bool& handle_request,
    CefRefPtr<CefCallback> p_callback)
{
    // | handle_request | return | description                                                      |
    // ----------------------------------------------------------------------------------------------
    // |     true       | true   | Handle the request immediately                                   |
    // |     false      | true   | Decide at a later time.Execute callback to continue or cancel    |
    // |     true       | false  | Cancel the request immediately                                   |
    // |     false      | false  | backwards compatibility.the ProcessRequest method will be called |
    handle_request = true;

    CefURLParts url_parts{};
    CefParseURL(p_request->GetURL(), url_parts);
    const auto path = "dist" / std::filesystem::path{CefString(&url_parts.path).ToString()}.relative_path();
    
    _file_size = (int64_t)std::filesystem::file_size(path);

    auto ext = path.extension().string();
    // we need to strip the leading '.'
    if (ext.starts_with('.')) {
        ext.erase(0, 1);
    }
    if (!ext.empty()) {
        _file_mime_type = CefGetMimeType(ext);
    }

    _file_stream = std::ifstream{ path, std::ios::binary };

    return (bool)_file_stream;
}

void NanoCefResourceHandler::GetResponseHeaders(
    CefRefPtr<CefResponse> p_response,
    int64_t& response_length,
    CefString& redirectUrl)
{
    if (!_file_mime_type.empty()) {
        p_response->SetMimeType(_file_mime_type);
    }
    p_response->SetStatus(200);
    response_length = _file_size;
}

void NanoCefResourceHandler::Cancel() {}

bool NanoCefResourceHandler::Read(
    void* data_out,
    int bytes_to_read,
    int& bytes_read,
    CefRefPtr<CefResourceReadCallback> p_callback)
{
    if (!data_out) {
        bytes_read = -2; // error code
        return false;
    }

    _file_stream.read(static_cast<char*>(data_out), bytes_to_read);
    bytes_read = (int)_file_stream.gcount();

    return bytes_read > 0;
}
