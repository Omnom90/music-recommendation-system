#include "feature_extractor.h"
#include <cmath>
using namespace std;

vector<double> FeatureExtractor::extractArtistFeatures(const Artist& artist) {
    vector<double> features;
    
    // push back the various features an artist will have

    features.push_back(extractGenreFeatures(artist.genre));

    features.push_back(extractPopularityFeatures(artist.popularity_score));

    features.push_back(extractTagFeatures(artist.tags));

    return features;
}

vector<double> FeatureExtractor::extractSongFeatures(const Song& song) {
    vector<double> features = song.features;
    
    // Note: We'll need to get artist features separately since Song doesn't contain Artist
    // For now, just return the song features
    return normalizeFeatures(features);
}

vector<double> FeatureExtractor::normalizeFeatures(const vector<double>& features) {
    vector<double> normal = features;
    double sum = 0.0;

    for(double i : features) sum += i*i;
    double magnitude = sqrt(sum);
    if(magnitude == 0) return normal;

    for(double& i : normal) i /= magnitude;
    return normal;
}

double FeatureExtractor::extractGenreFeatures(const string& genre) {
    if(genre == "Rock") return 1.0;
    if(genre == "Pop") return 2.0;
    if(genre == "Hip-Hop") return 3.0;
    if(genre == "Jazz") return 4.0;
    if(genre == "Classical") return 5.0;
    if(genre == "Electronic") return 6.0;
    if(genre == "Country") return 7.0;
    if(genre == "R&B") return 8.0;
    if(genre == "Latin") return 9.0;
    if(genre == "Indie") return 10.0;
    if(genre == "Folk") return 11.0;
    if(genre == "Soul") return 12.0;
    if(genre == "Funk") return 13.0;
    if(genre == "Disco") return 14.0;
    if(genre == "Reggae") return 15.0;
    if(genre == "Blues") return 16.0;
    if(genre == "Punk") return 17.0;
    if(genre == "Metal") return 18.0;
    if(genre == "Rap") return 19.0;
    if(genre == "Singer-Songwriter") return 20.0;
    return 0.0; // Default case
}

double FeatureExtractor::extractPopularityFeatures(double popularity_score) {
    return popularity_score;
}

double FeatureExtractor::extractTagFeatures(const vector<string>& tags) {
    return static_cast<double>(tags.size());
}