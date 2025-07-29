#pragma once
#include <string>
#include <vector>
using namespace std;

class SpotifyAPI {
public:
    void setAccessToken(const string& token);
        // returns a struct or map with artist info
    Artist searchArtist(const string& artist_name);
        // return a vector with the artists top tracks
    vector<Song> getArtistsTopTracks(const string& artist_name);
        // returns a struct with some audio features
    AudioFeatures getAudioFeatures(const string& track_id);

private:
    string access_token_;
}