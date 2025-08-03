#pragma once
#include "types.h"
#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class SpotifyAPI {
public:
    // Constructor and destructor
    SpotifyAPI();
    ~SpotifyAPI();
    
    // Authentication
    void setAccessToken(const string& token);
    string getAccessToken() const;
    
    // API methods
    Artist searchArtist(const string& artist_name);
    vector<Song> getArtistTopTracks(const string& artist_id);
    vector<double> getAudioFeatures(const string& track_id);
    vector<Artist> searchArtistsByGenre(const string& genre, int limit = 50);
    
    // Batch operations
    vector<Artist> loadMultipleArtists(const vector<string>& artist_names);
    void populateAudioFeatures(vector<Song>& songs);
    
    // Utility methods
    bool isTokenValid() const;
    void refreshTokenIfNeeded();

private:
    string access_token_;
    time_t token_expiry_;
    
    // Helper methods
    string makeAPIRequest(const string& endpoint, const string& query_params = "");
    json parseJSONResponse(const string& response);
    Artist parseArtistFromJSON(const json& artist_data);
    Song parseSongFromJSON(const json& track_data, const string& artist_id);
    vector<double> parseAudioFeaturesFromJSON(const json& features_data);
    
    // cURL callback
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp);
};