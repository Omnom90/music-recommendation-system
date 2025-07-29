#include "popularity_adjuster.h"
using namespace std;

double PopularityAdjuster::adjustForPopularity(double similarity_score, double popularity_score) {
    double penalty = calculatePopularityPenalty(popularity_score);
    return similarity_score * penalty;
}

double PopularityAdjuster::boostUndergroundArtists(double similarity_score, double popularity_score) {
    if(popularity_score < underground_threshold_) return similarity_score * boost_factor_;
    return similarity_score; // Return original score if not underground
}

double PopularityAdjuster::calculatePopularityPenalty(double popularity_score) {
    return 1.0 - popularity_score;
}

void PopularityAdjuster::setUndergroundThreshold(double threshold) {
    underground_threshold_ = threshold;
}
void PopularityAdjuster::setBoostFactor(double factor) {
    boost_factor_ = factor;
}