#include "spotify_api.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

// Constructor
SpotifyAPI::SpotifyAPI() : token_expiry_(0) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

// Destructor
SpotifyAPI::~SpotifyAPI() {
    curl_global_cleanup();
}

// Set access token
void SpotifyAPI::setAccessToken(const string& token) {
    access_token_ = token;
    // Set expiry to 1 hour from now
    token_expiry_ = time(nullptr) + 3600;
}

// Get access token
string SpotifyAPI::getAccessToken() const {
    return access_token_;
}

// Check if token is still valid
bool SpotifyAPI::isTokenValid() const {
    return !access_token_.empty() && time(nullptr) < token_expiry_;
}

// cURL callback function
size_t SpotifyAPI::WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Make API request
string SpotifyAPI::makeAPIRequest(const string& endpoint, const string& query_params) {
    CURL* curl = curl_easy_init();
    string readBuffer;
    
    if (!curl) {
        cerr << "Failed to initialize cURL" << endl;
        return "";
    }
    
    // Build URL
    string url = "https://api.spotify.com/v1" + endpoint;
    if (!query_params.empty()) {
        url += "?" + query_params;
    }
    
    // Set up headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token_).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    // Set cURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    // Perform request
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        cerr << "cURL request failed: " << curl_easy_strerror(res) << endl;
        readBuffer = "";
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    
    return readBuffer;
}

// Parse JSON response
json SpotifyAPI::parseJSONResponse(const string& response) {
    try {
        return json::parse(response);
    } catch (const exception& e) {
        cerr << "Failed to parse JSON: " << e.what() << endl;
        return json();
    }
}

// Search for an artist
Artist SpotifyAPI::searchArtist(const string& artist_name) {
    if (!isTokenValid()) {
        cerr << "Access token is invalid or expired" << endl;
        return Artist();
    }
    
    // Build query parameters
    string query_params = "q=" + artist_name + "&type=artist&limit=1";
    
    // Make request
    string response = makeAPIRequest("/search", query_params);
    json j = parseJSONResponse(response);
    
    if (j.empty() || !j.contains("artists") || !j["artists"].contains("items")) {
        cerr << "Invalid response format or no artists found" << endl;
        return Artist();
    }
    
    auto items = j["artists"]["items"];
    if (items.empty()) {
        cerr << "No artists found for: " << artist_name << endl;
        return Artist();
    }
    
    return parseArtistFromJSON(items[0]);
}

// Parse artist from JSON
Artist SpotifyAPI::parseArtistFromJSON(const json& artist_data) {
    Artist artist;
    
    artist.id = artist_data.value("id", "");
    artist.name = artist_data.value("name", "");
    artist.popularity_score = artist_data.value("popularity", 0) / 100.0; // Convert to 0-1 scale
    
    // Get genres
    if (artist_data.contains("genres") && artist_data["genres"].is_array()) {
        for (const auto& genre : artist_data["genres"]) {
            artist.tags.push_back(genre.get<string>());
        }
    }
    
    // Set primary genre (first one or "Unknown")
    artist.genre = artist.tags.empty() ? "Unknown" : artist.tags[0];
    
    return artist;
}

// Get artist's top tracks
vector<Song> SpotifyAPI::getArtistTopTracks(const string& artist_id) {
    if (!isTokenValid()) {
        cerr << "Access token is invalid or expired" << endl;
        return vector<Song>();
    }
    
    string query_params = "market=US";
    string response = makeAPIRequest("/artists/" + artist_id + "/top-tracks", query_params);
    json j = parseJSONResponse(response);
    
    vector<Song> tracks;
    if (j.contains("tracks") && j["tracks"].is_array()) {
        for (const auto& track_data : j["tracks"]) {
            tracks.push_back(parseSongFromJSON(track_data, artist_id));
        }
    }
    
    return tracks;
}

// Parse song from JSON
Song SpotifyAPI::parseSongFromJSON(const json& track_data, const string& artist_id) {
    Song song;
    
    song.id = track_data.value("id", "");
    song.name = track_data.value("name", "");
    song.artist_id = artist_id;
    song.popularity_score = track_data.value("popularity", 0) / 100.0; // Convert to 0-1 scale
    
    // Initialize features vector (will be populated later)
    song.features = vector<double>(5, 0.0); // 5 default features
    
    return song;
}

// Get audio features for a track
vector<double> SpotifyAPI::getAudioFeatures(const string& track_id) {
    if (!isTokenValid()) {
        cerr << "Access token is invalid or expired" << endl;
        return vector<double>();
    }
    
    string response = makeAPIRequest("/audio-features/" + track_id);
    json j = parseJSONResponse(response);
    
    return parseAudioFeaturesFromJSON(j);
}

// Parse audio features from JSON
vector<double> SpotifyAPI::parseAudioFeaturesFromJSON(const json& features_data) {
    vector<double> features;
    
    // Extract key audio features
    features.push_back(features_data.value("danceability", 0.0));
    features.push_back(features_data.value("energy", 0.0));
    features.push_back(features_data.value("valence", 0.0));
    features.push_back(features_data.value("tempo", 0.0) / 200.0); // Normalize tempo
    features.push_back(features_data.value("acousticness", 0.0));
    
    return features;
}

// Search artists by genre
vector<Artist> SpotifyAPI::searchArtistsByGenre(const string& genre, int limit) {
    if (!isTokenValid()) {
        cerr << "Access token is invalid or expired" << endl;
        return vector<Artist>();
    }
    
    string query_params = "q=genre:" + genre + "&type=artist&limit=" + to_string(limit);
    string response = makeAPIRequest("/search", query_params);
    json j = parseJSONResponse(response);
    
    vector<Artist> artists;
    if (j.contains("artists") && j["artists"].contains("items")) {
        for (const auto& artist_data : j["artists"]["items"]) {
            artists.push_back(parseArtistFromJSON(artist_data));
        }
    }
    
    return artists;
}

// Load multiple artists
vector<Artist> SpotifyAPI::loadMultipleArtists(const vector<string>& artist_names) {
    vector<Artist> artists;
    
    for (const auto& name : artist_names) {
        Artist artist = searchArtist(name);
        if (!artist.id.empty()) {
            artists.push_back(artist);
        }
        // Add delay to respect rate limits
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    return artists;
}

// Populate audio features for songs
void SpotifyAPI::populateAudioFeatures(vector<Song>& songs) {
    for (auto& song : songs) {
        if (!song.id.empty()) {
            song.features = getAudioFeatures(song.id);
        }
        // Add delay to respect rate limits
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

