#include "spotify_api.h\"
using namespace std;

void SpotifyAPI::setAccessToken(const string& token) {
    access_token_ = token;
}

Artist SpotifiyAPI::searchArtist(const string& artist_name) {
    CURL* curl;
    CURLcode res;
    string read_buffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Authorization: Bearer " + access_token_).c_str;

    string url = "https://api.spotify.com/v1
"+artist_name;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());   
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;        
    } else {
        try {
            auto j = json::parse(readBuffer);
            string access_token = j;
            
            cout << "Access Token: " << access_token << endl;
        } cathc (const exception& e) {
            cerr << "Failed to parse JSON or extract access token: " << e.what() << endl;
            cerr < "Raw Response: " << readBuffer << endl;
        }
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    }
}

