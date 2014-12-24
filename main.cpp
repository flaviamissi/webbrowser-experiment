#include <curl/curl.h>
#include <iostream>


int main() {
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, "http://flaviamissi.com.br");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode resp = curl_easy_perform(handle);
    if(resp != CURLE_OK)
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(resp) << std::endl;

    curl_global_cleanup();
    return 0;
}
