#pragma once
#include "types.h"
#include "similarity_calculator.h"
#include "popularity_adjuster.h"
#include "ml_enhancer.h"
using namespace std;

class RecommendationEngine {
public:
    // Constructor
    RecommendationEngine();
    
    // Main recommendation functions
    RecommendationList recommendSimilarArtists(const string& artist_name, 
                                              const ArtistDatabase& artists, 
                                              int num_recommendations = 10);
    
    RecommendationList recommendSimilarSongs(const string& song_title,
                                            const SongDatabase& songs,
                                            const ArtistDatabase& artists,
                                            int num_recommendations = 10);
    
    // Set engine parameters
    void setSimilarityThreshold(double threshold);
    void setMaxPopularity(double max_popularity);
    void enableML(bool enable = true);
    
    // ML training
    void trainMLModels(const ArtistDatabase& artists, const SongDatabase& songs);

private:
    SimilarityCalculator similarity_calc_;
    PopularityAdjuster popularity_adjuster_;
    MLEnhancer ml_enhancer_;
    
    double similarity_threshold_ = 0.1;
    double max_popularity_ = 0.8;
    bool ml_enabled_ = true;
    
    // Helper methods
    RecommendationList filterAndRank(const vector<RecommendationResult>& candidates);
    bool meetsPopularityCriteria(double popularity_score);
};