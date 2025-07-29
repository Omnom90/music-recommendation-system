#include "similarity_calculator.h"
#include "feature_extractor.h"
#include <cmath>
using namespace std;

double SimilarityCalculator::calculateCosineSimilarity(const vector<double>& vec1, const vector<double>& vec2) {
    double dot_product = dotProduct(vec1, vec2);
    double magnitude1 = magnitude(vec1);
    double magnitude2 = magnitude(vec2);

    if(magnitude1 == 0 || magnitude2 == 0) return 0.0;

    return dot_product / (magnitude1 * magnitude2);
}

double SimilarityCalculator::calculateEuclideanDistance(const vector<double>& vec1, const vector<double>& vec2) {
    double sum = 0.0;

    for(size_t i = 0; i < vec1.size(); ++i) {
        double diff = vec1[i] - vec2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

double SimilarityCalculator::calculateArtistSimilarity(const Artist& artist1, const Artist& artist2) {
    FeatureExtractor fe;
    vector<double> features1 = fe.extractArtistFeatures(artist1);
    vector<double> features2 = fe.extractArtistFeatures(artist2);

    return calculateCosineSimilarity(features1, features2);
}

double SimilarityCalculator::calculateSongSimilarity(const Song& song1, const Song& song2) {
    return calculateCosineSimilarity(song1.features, song2.features);
}

double SimilarityCalculator::distanceToSimilarity(double distance) {
    return 1.0 /(1.0+distance);
}

double SimilarityCalculator::dotProduct(const vector<double>& vec1, const vector<double>& vec2) {
    double sum = 0.0;

    for(size_t i = 0; i < vec1.size(); ++i) {
        sum += vec1[i] * vec2[i];
    }
    return sum;
}

double SimilarityCalculator::magnitude(const vector<double>& vec) {
    double sum = 0.0;

    for(double i : vec) sum += i*i;
    return sqrt(sum);
}