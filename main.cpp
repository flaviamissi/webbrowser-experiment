#include <curl/curl.h>
#include <iostream>
#include "converter.cpp"

// libcurl write callback function
static int writer(char *data, size_t size, size_t nmemb, std::string *writer_data) {
    if (writer_data == NULL)
        return 0;
    writer_data->append(data, size * nmemb);
    return size * nmemb;
}

class Request {
    CURL *handle;
    char error_buffer[CURL_ERROR_SIZE];
    std::string data_buffer;
    public:
        Request();
        std::string get(std::string);
};

Request::Request() {
    handle = curl_easy_init();
    CURLcode code;

    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
    if (code != CURLE_OK) {
        std::cerr << "Failed to set error buffer " << code << std::endl;
    }

    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    if (code != CURLE_OK) {
        std::cerr << "Failed to set redirect option " << error_buffer << std::endl;
    }

    code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK) {
        std::cerr << "Failed to set writer " << error_buffer << std::endl;
    }

    code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data_buffer);
    if (code != CURLE_OK) {
        std::cerr << "Failed to set write data " << error_buffer << std::endl;
    }
}

std::string Request::get(std::string url) {
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    CURLcode resp = curl_easy_perform(handle);
    if(resp != CURLE_OK)
        std::cerr << "Request failed: " << curl_easy_strerror(resp) << std::endl;

    return data_buffer;
}

int main(int argc, const char* argv[]) {
    if (argc <= 1) {
        std::cout << "Wrong number of arguments, make sure you are passing the desired URL to open." << std::endl;
        return 1;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    Request request;
    std::string url = argv[1];
    std::cout << "Requesting " << url << "..." << std::endl;
    std::string result = request.get(url);

    curl_global_cleanup();

    result = HTMLToANSI(result);
    std::cout << result << std::endl;

    return 0;
}
