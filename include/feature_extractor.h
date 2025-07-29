#pragma once
#include "types.h"
#include <vector>
using namespace std;

class FeatureExtractor {
public:
    // extract features from audio files
    vector<double> extractArtistFeatures(const Artist& artist);

    // extract features from audio files
    vector<double> extractSongFeatures(const Song& song);

    // normalize feature vectors
    vector<double> normalizeFeatures(const vector<double>& features);

    // get feature names for debugging
    vector<string> getFeatureNames() const;

private:
    // helper methods and functions for specific feature extraction
    double extractGenreFeatures(const string& genre);
    double extractPopularityFeatures(double popularity_score);
    double extractTagFeatures(const vector<string>& tags);
};