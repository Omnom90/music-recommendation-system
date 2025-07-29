#pragma once
#include "types.h"
using namespace std;

class PopularityAdjuster {
public:
    // adjust similarity score based on popularity
    double adjustForPopularity(double similarity_score, double popularity_score);

    // boost underground or indie artists
    double boostUndergroundArtists(double similarity_score, double popularity_score);

    // calcualte popularity penalty factor
    double calculatePopularityPenalty(double popularity_score);

    // set adjustment parameters
    void setUndergroundThreshold(double threshold);
    void setBoostFactor(double factor);

private:
    double underground_threshold_ = 0.3; // artists below this will get a booster score
    double boost_factor_ = 1.5; // this is how much the artist will be boosted
};