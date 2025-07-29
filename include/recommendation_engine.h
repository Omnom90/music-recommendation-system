#pragma once
#include "types.h"
#include "similarity_calculator.h"
#include "popularity_adjuster.h"
using namespace std;

class RecommendationEngine {
public:
    // main recommendation function
    RecommendationList recommendSimilarArtists(const string& artist_name, const ArtistDatabase& artists, int num_recommendations = 10);
    
    RecommendationList recommendSimilarSongs(const string& song_title, const SongDatabase& songs,  const ArtistDatabase& artists,int num_recommendations = 10);
    

    // set engine parameters
    void setSimilarityThreshold(double threshold);
    void setMaxPopularity(double max_popularity);

private: 
    SimilarityCalculator similarity_calc_;
    PopularityAdjuster popularity_adjuster_;
    double similarity_threshold_ = 0.1;
    double max_popularity_ = 0.8;

    // helper methods and functions
    RecommendationList filterAndRank(const vector<RecommendationResult>& candidates);
    bool meetsPopularityCriteria(double popularity_score);
};