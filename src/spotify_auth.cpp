// spotify_auth.cpp
// This program demonstrates how to authenticate with the Spotify API using C++
// It requests an access token using the Client Credentials Flow
// and prints the access token to the console.
//
// Dependencies: libcurl, nlohmann/json
// Compile with: g++ -std=c++17 -lcurl -o spotify_auth src/spotify_auth.cpp
//
// Author: ohm

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp> // Download from https://github.com/nlohmann/json

using namespace std;
using json = nlohmann::json;

// Helper function for base64 encoding (needed for Spotify API authentication)
string base64_encode(const string& in) {
    static const string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(b[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

// Callback for cURL to write response data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // Your Spotify API credentials
    string client_id = "d15519af8f38423d989e45a2a9f25387";
    string client_secret = "422c78b305744f2e8c531cce8b1168aa";

    // Step 1: Combine client_id and client_secret with a colon, then base64 encode
    string auth = client_id + ":" + client_secret;
    string auth_base64 = base64_encode(auth);

    // Step 2: Initialize cURL
    CURL* curl;
    CURLcode res;
    string readBuffer; // This will hold the response from Spotify

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Step 3: Set up HTTP headers
        struct curl_slist* headers = NULL;
        // Authorization header: Basic <base64 encoded client_id:client_secret>
        headers = curl_slist_append(headers, ("Authorization: Basic " + auth_base64).c_str());
        // Content-Type header: required for form data
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        // Step 4: Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // POST data: grant_type=client_credentials (required by Spotify)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "grant_type=client_credentials");
        // Set up callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Step 5: Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            // Step 6: Parse the JSON response to get the access token
            try {
                auto j = json::parse(readBuffer);
                string access_token = j["access_token"];
                cout << "Access Token: " << access_token << endl;
            } catch (const std::exception& e) {
                cerr << "Failed to parse JSON or extract access token: " << e.what() << endl;
                cerr << "Raw response: " << readBuffer << endl;
            }
        }

        // Step 7: Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return 0;
} 