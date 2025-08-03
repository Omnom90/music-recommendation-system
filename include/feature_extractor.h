#pragma once
#include "types.h"
#include <vector>
#include <string>
using namespace std;

class FeatureExtractor {
public:
    // Extract features from artists and songs
    vector<double> extractArtistFeatures(const Artist& artist);
    vector<double> extractSongFeatures(const Song& song);
    
    // Normalize feature vectors
    vector<double> normalizeFeatures(const vector<double>& features);
    
    // Get feature names for debugging
    vector<string> getFeatureNames() const;

private:
    // Helper methods for specific feature extraction
    double extractGenreFeatures(const string& genre);
    double extractPopularityFeatures(double popularity_score);
    double extractTagFeatures(const vector<string>& tags);
    double extractGenreDiversity(const vector<string>& tags);
    double extractUndergroundFactor(double popularity_score);
};