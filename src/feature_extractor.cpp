#include "feature_extractor.h"
#include <cmath>
#include <algorithm>
using namespace std;

vector<double> FeatureExtractor::extractArtistFeatures(const Artist& artist) {
    vector<double> features;
    
    // Extract genre feature (encoded as number)
    features.push_back(extractGenreFeatures(artist.genre));
    
    // Extract popularity feature (0-1 scale)
    features.push_back(extractPopularityFeatures(artist.popularity_score));
    
    // Extract tag features (number of tags, normalized)
    features.push_back(extractTagFeatures(artist.tags));
    
    // Extract genre diversity (how many different genres)
    features.push_back(extractGenreDiversity(artist.tags));
    
    // Extract underground factor (inverse of popularity)
    features.push_back(extractUndergroundFactor(artist.popularity_score));
    
    return normalizeFeatures(features);
}

vector<double> FeatureExtractor::extractSongFeatures(const Song& song) {
    vector<double> features = song.features;
    
    // Add popularity feature
    features.push_back(extractPopularityFeatures(song.popularity_score));
    
    // Add underground factor
    features.push_back(extractUndergroundFactor(song.popularity_score));
    
    return normalizeFeatures(features);
}

vector<double> FeatureExtractor::normalizeFeatures(const vector<double>& features) {
    vector<double> normalized = features;
    double sum = 0.0;

    // Calculate magnitude
    for(double feature : features) {
        sum += feature * feature;
    }
    double magnitude = sqrt(sum);
    
    // Avoid division by zero
    if(magnitude < 1e-10) {
        return normalized;
    }

    // Normalize each feature
    for(double& feature : normalized) {
        feature /= magnitude;
    }
    
    return normalized;
}

double FeatureExtractor::extractGenreFeatures(const string& genre) {
    // Enhanced genre encoding with more genres
    if(genre == "hip-hop" || genre == "rap") return 1.0;
    if(genre == "pop") return 2.0;
    if(genre == "rock") return 3.0;
    if(genre == "electronic" || genre == "edm") return 4.0;
    if(genre == "r&b" || genre == "soul") return 5.0;
    if(genre == "indie" || genre == "alternative") return 6.0;
    if(genre == "jazz") return 7.0;
    if(genre == "classical") return 8.0;
    if(genre == "country") return 9.0;
    if(genre == "folk") return 10.0;
    if(genre == "metal") return 11.0;
    if(genre == "punk") return 12.0;
    if(genre == "reggae") return 13.0;
    if(genre == "blues") return 14.0;
    if(genre == "funk") return 15.0;
    if(genre == "disco") return 16.0;
    if(genre == "latin") return 17.0;
    if(genre == "world") return 18.0;
    if(genre == "experimental") return 19.0;
    if(genre == "ambient") return 20.0;
    
    return 0.0; // Default for unknown genres
}

double FeatureExtractor::extractPopularityFeatures(double popularity_score) {
    return popularity_score; // Already 0-1 scale
}

double FeatureExtractor::extractTagFeatures(const vector<string>& tags) {
    // Normalize number of tags (0-10 scale)
    return min(static_cast<double>(tags.size()) / 10.0, 1.0);
}

double FeatureExtractor::extractGenreDiversity(const vector<string>& tags) {
    // Count unique genres in tags
    vector<string> unique_genres;
    for(const auto& tag : tags) {
        if(find(unique_genres.begin(), unique_genres.end(), tag) == unique_genres.end()) {
            unique_genres.push_back(tag);
        }
    }
    return min(static_cast<double>(unique_genres.size()) / 5.0, 1.0);
}

double FeatureExtractor::extractUndergroundFactor(double popularity_score) {
    // Inverse of popularity (higher for underground artists)
    return 1.0 - popularity_score;
}

vector<string> FeatureExtractor::getFeatureNames() const {
    return {
        "genre_encoding",
        "popularity_score", 
        "tag_count",
        "genre_diversity",
        "underground_factor"
    };
}