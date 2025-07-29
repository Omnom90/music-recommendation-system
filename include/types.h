#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Artist {
    string name;
    string genre;
    double popularity_score; //0.0 - 1.0
    vector<string> tags;
    string id;
};

struct Song {
    string name;
    string artist_id;
    vector<double> features; // musical features
    double popularity_score; //0.0 - 1.0
    string id;
};

struct RecommendationResult {
    string artist_name;
    string song_title;
    double similarity_score; // 0.0 - 1.0
    double adjusted_score; // 0.0 - 1.0
    string reason; // why this song / artist was recommended
};

using ArtistDatabase = map<string, Artist>;
using SongDatabase = map<string, Song>;
using RecommendationList = vector<RecommendationResult>;