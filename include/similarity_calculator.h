#pragma once
#include "types.h"
#include <vector>
using namespace std;

class SimilarityCalculator {
public:

    // calculating the similarity between two feature vectors
    double calculateCosineSimilarity(const vector<double>& vec1, const vector<double>& vec2);
    double calculateEuclideanDistance(const vector<double>& vec1, const vector<double>& vec2);

    // calculating the similarity between two artists
    double calculateArtistSimilarity(const Artist& artist1, const Artist& artist2);

    // calculatring the siilarity between two songs
    double calculateSongSimilarity(const Song& song1, const Song& song2);

    // converting the distance into a similarity score between 0 and 1
    double distanceToSimilarity(double distance);

private:
    // helper methods/functions
    double dotProduct(const vector<double>& vec1, const vector<double>& vec2);
    double magnitude(const vector<double>& vec);
};