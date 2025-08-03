#pragma once
#include "types.h"
#include <vector>
#include <map>
#include <random>
using namespace std;

// K-means clustering for music recommendations
class MLEnhancer {
public:
    // Constructor
    MLEnhancer(int num_clusters = 8);
    
    // Train the model with artist/song data
    void trainArtistModel(const vector<Artist>& artists);
    void trainSongModel(const vector<Song>& songs);
    
    // Get enhanced recommendations
    vector<RecommendationResult> enhanceArtistRecommendations(
        const vector<RecommendationResult>& base_recommendations,
        const Artist& input_artist,
        const ArtistDatabase& artists
    );
    
    vector<RecommendationResult> enhanceSongRecommendations(
        const vector<RecommendationResult>& base_recommendations,
        const Song& input_song,
        const SongDatabase& songs
    );
    
    // Get cluster information
    int getArtistCluster(const Artist& artist) const;
    int getSongCluster(const Song& song) const;
    vector<Artist> getArtistsInCluster(int cluster_id) const;
    vector<Song> getSongsInCluster(int cluster_id) const;
    
    // Model information
    bool isArtistModelTrained() const { return artist_model_trained_; }
    bool isSongModelTrained() const { return song_model_trained_; }
    int getNumClusters() const { return num_clusters_; }

private:
    int num_clusters_;
    bool artist_model_trained_;
    bool song_model_trained_;
    
    // K-means centroids
    vector<vector<double>> artist_centroids_;
    vector<vector<double>> song_centroids_;
    
    // Cluster assignments
    map<string, int> artist_clusters_;  // artist_id -> cluster
    map<string, int> song_clusters_;    // song_id -> cluster
    
    // Training data
    vector<Artist> training_artists_;
    vector<Song> training_songs_;
    
    // Helper methods
    vector<vector<double>> extractArtistFeatures(const vector<Artist>& artists);
    vector<vector<double>> extractSongFeatures(const vector<Song>& songs);
    vector<int> kmeansClustering(const vector<vector<double>>& data, int k);
    double calculateDistance(const vector<double>& point1, const vector<double>& point2);
    vector<double> calculateCentroid(const vector<vector<double>>& cluster_points);
    int findNearestCentroid(const vector<double>& point, const vector<vector<double>>& centroids);
    
    // Random number generation
    random_device rd_;
    mt19937 gen_;
}; 