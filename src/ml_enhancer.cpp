#include "ml_enhancer.h"
#include "feature_extractor.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;

// Constructor
MLEnhancer::MLEnhancer(int num_clusters) 
    : num_clusters_(num_clusters), 
      artist_model_trained_(false), 
      song_model_trained_(false),
      gen_(rd_()) {
}

// Train artist model with K-means clustering
void MLEnhancer::trainArtistModel(const vector<Artist>& artists) {
    if (artists.empty()) {
        cerr << "No artists provided for training" << endl;
        return;
    }
    
    training_artists_ = artists;
    
    // Extract features from all artists
    vector<vector<double>> features = extractArtistFeatures(artists);
    
    // Perform K-means clustering
    vector<int> cluster_assignments = kmeansClustering(features, num_clusters_);
    
    // Store cluster assignments
    artist_clusters_.clear();
    for (size_t i = 0; i < artists.size(); ++i) {
        artist_clusters_[artists[i].id] = cluster_assignments[i];
    }
    
    // Calculate centroids for each cluster
    artist_centroids_.clear();
    artist_centroids_.resize(num_clusters_);
    
    for (int cluster = 0; cluster < num_clusters_; ++cluster) {
        vector<vector<double>> cluster_points;
        for (size_t i = 0; i < features.size(); ++i) {
            if (cluster_assignments[i] == cluster) {
                cluster_points.push_back(features[i]);
            }
        }
        if (!cluster_points.empty()) {
            artist_centroids_[cluster] = calculateCentroid(cluster_points);
        }
    }
    
    artist_model_trained_ = true;
    cout << "Artist model trained with " << artists.size() << " artists in " << num_clusters_ << " clusters" << endl;
}

// Train song model with K-means clustering
void MLEnhancer::trainSongModel(const vector<Song>& songs) {
    if (songs.empty()) {
        cerr << "No songs provided for training" << endl;
        return;
    }
    
    training_songs_ = songs;
    
    // Extract features from all songs
    vector<vector<double>> features = extractSongFeatures(songs);
    
    // Perform K-means clustering
    vector<int> cluster_assignments = kmeansClustering(features, num_clusters_);
    
    // Store cluster assignments
    song_clusters_.clear();
    for (size_t i = 0; i < songs.size(); ++i) {
        song_clusters_[songs[i].id] = cluster_assignments[i];
    }
    
    // Calculate centroids for each cluster
    song_centroids_.clear();
    song_centroids_.resize(num_clusters_);
    
    for (int cluster = 0; cluster < num_clusters_; ++cluster) {
        vector<vector<double>> cluster_points;
        for (size_t i = 0; i < features.size(); ++i) {
            if (cluster_assignments[i] == cluster) {
                cluster_points.push_back(features[i]);
            }
        }
        if (!cluster_points.empty()) {
            song_centroids_[cluster] = calculateCentroid(cluster_points);
        }
    }
    
    song_model_trained_ = true;
    cout << "Song model trained with " << songs.size() << " songs in " << num_clusters_ << " clusters" << endl;
}

// Extract features from artists
vector<vector<double>> MLEnhancer::extractArtistFeatures(const vector<Artist>& artists) {
    FeatureExtractor fe;
    vector<vector<double>> features;
    
    for (const auto& artist : artists) {
        features.push_back(fe.extractArtistFeatures(artist));
    }
    
    return features;
}

// Extract features from songs
vector<vector<double>> MLEnhancer::extractSongFeatures(const vector<Song>& songs) {
    FeatureExtractor fe;
    vector<vector<double>> features;
    
    for (const auto& song : songs) {
        features.push_back(fe.extractSongFeatures(song));
    }
    
    return features;
}

// K-means clustering algorithm
vector<int> MLEnhancer::kmeansClustering(const vector<vector<double>>& data, int k) {
    if (data.empty() || k <= 0) {
        return vector<int>();
    }
    
    int n_points = data.size();
    int n_features = data[0].size();
    
    // Initialize centroids randomly
    vector<vector<double>> centroids(k, vector<double>(n_features));
    uniform_int_distribution<> dis(0, n_points - 1);
    
    for (int i = 0; i < k; ++i) {
        int random_idx = dis(gen_);
        centroids[i] = data[random_idx];
    }
    
    // Main K-means loop
    vector<int> assignments(n_points, 0);
    bool converged = false;
    int max_iterations = 100;
    int iteration = 0;
    
    while (!converged && iteration < max_iterations) {
        converged = true;
        
        // Assign points to nearest centroid
        for (int i = 0; i < n_points; ++i) {
            int nearest_cluster = findNearestCentroid(data[i], centroids);
            if (assignments[i] != nearest_cluster) {
                assignments[i] = nearest_cluster;
                converged = false;
            }
        }
        
        // Update centroids
        for (int cluster = 0; cluster < k; ++cluster) {
            vector<vector<double>> cluster_points;
            for (int i = 0; i < n_points; ++i) {
                if (assignments[i] == cluster) {
                    cluster_points.push_back(data[i]);
                }
            }
            
            if (!cluster_points.empty()) {
                vector<double> new_centroid = calculateCentroid(cluster_points);
                if (new_centroid != centroids[cluster]) {
                    centroids[cluster] = new_centroid;
                    converged = false;
                }
            }
        }
        
        iteration++;
    }
    
    return assignments;
}

// Calculate Euclidean distance between two points
double MLEnhancer::calculateDistance(const vector<double>& point1, const vector<double>& point2) {
    if (point1.size() != point2.size()) {
        return numeric_limits<double>::max();
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < point1.size(); ++i) {
        double diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    
    return sqrt(sum);
}

// Calculate centroid of a cluster
vector<double> MLEnhancer::calculateCentroid(const vector<vector<double>>& cluster_points) {
    if (cluster_points.empty()) {
        return vector<double>();
    }
    
    int n_features = cluster_points[0].size();
    vector<double> centroid(n_features, 0.0);
    
    for (const auto& point : cluster_points) {
        for (size_t i = 0; i < n_features; ++i) {
            centroid[i] += point[i];
        }
    }
    
    for (double& value : centroid) {
        value /= cluster_points.size();
    }
    
    return centroid;
}

// Find nearest centroid for a point
int MLEnhancer::findNearestCentroid(const vector<double>& point, const vector<vector<double>>& centroids) {
    double min_distance = numeric_limits<double>::max();
    int nearest_cluster = 0;
    
    for (size_t i = 0; i < centroids.size(); ++i) {
        double distance = calculateDistance(point, centroids[i]);
        if (distance < min_distance) {
            min_distance = distance;
            nearest_cluster = i;
        }
    }
    
    return nearest_cluster;
}

// Enhance artist recommendations using ML
vector<RecommendationResult> MLEnhancer::enhanceArtistRecommendations(
    const vector<RecommendationResult>& base_recommendations,
    const Artist& input_artist,
    const ArtistDatabase& artists) {
    
    if (!artist_model_trained_) {
        return base_recommendations; // Return original if model not trained
    }
    
    vector<RecommendationResult> enhanced = base_recommendations;
    
    // Get input artist's cluster
    FeatureExtractor fe;
    vector<double> input_features = fe.extractArtistFeatures(input_artist);
    int input_cluster = findNearestCentroid(input_features, artist_centroids_);
    
    // Boost recommendations from the same cluster
    for (auto& rec : enhanced) {
        // Find the artist in our database
        auto it = find_if(artists.begin(), artists.end(),
            [&](const auto& pair) { return pair.second.name == rec.artist_name; });
        
        if (it != artists.end()) {
            int rec_cluster = getArtistCluster(it->second);
            
            // Boost score if in same cluster
            if (rec_cluster == input_cluster) {
                rec.adjusted_score *= 1.2; // 20% boost
                rec.reason += " (Same cluster)";
            }
        }
    }
    
    // Sort by enhanced score
    sort(enhanced.begin(), enhanced.end(),
        [](const auto& a, const auto& b) { return a.adjusted_score > b.adjusted_score; });
    
    return enhanced;
}

// Enhance song recommendations using ML
vector<RecommendationResult> MLEnhancer::enhanceSongRecommendations(
    const vector<RecommendationResult>& base_recommendations,
    const Song& input_song,
    const SongDatabase& songs) {
    
    if (!song_model_trained_) {
        return base_recommendations; // Return original if model not trained
    }
    
    vector<RecommendationResult> enhanced = base_recommendations;
    
    // Get input song's cluster
    FeatureExtractor fe;
    vector<double> input_features = fe.extractSongFeatures(input_song);
    int input_cluster = findNearestCentroid(input_features, song_centroids_);
    
    // Boost recommendations from the same cluster
    for (auto& rec : enhanced) {
        // Find the song in our database
        auto it = find_if(songs.begin(), songs.end(),
            [&](const auto& pair) { return pair.second.name == rec.song_title; });
        
        if (it != songs.end()) {
            int rec_cluster = getSongCluster(it->second);
            
            // Boost score if in same cluster
            if (rec_cluster == input_cluster) {
                rec.adjusted_score *= 1.2; // 20% boost
                rec.reason += " (Same cluster)";
            }
        }
    }
    
    // Sort by enhanced score
    sort(enhanced.begin(), enhanced.end(),
        [](const auto& a, const auto& b) { return a.adjusted_score > b.adjusted_score; });
    
    return enhanced;
}

// Get artist's cluster
int MLEnhancer::getArtistCluster(const Artist& artist) const {
    auto it = artist_clusters_.find(artist.id);
    return (it != artist_clusters_.end()) ? it->second : -1;
}

// Get song's cluster
int MLEnhancer::getSongCluster(const Song& song) const {
    auto it = song_clusters_.find(song.id);
    return (it != song_clusters_.end()) ? it->second : -1;
}

// Get artists in a specific cluster
vector<Artist> MLEnhancer::getArtistsInCluster(int cluster_id) const {
    vector<Artist> cluster_artists;
    
    for (const auto& artist : training_artists_) {
        if (getArtistCluster(artist) == cluster_id) {
            cluster_artists.push_back(artist);
        }
    }
    
    return cluster_artists;
}

// Get songs in a specific cluster
vector<Song> MLEnhancer::getSongsInCluster(int cluster_id) const {
    vector<Song> cluster_songs;
    
    for (const auto& song : training_songs_) {
        if (getSongCluster(song) == cluster_id) {
            cluster_songs.push_back(song);
        }
    }
    
    return cluster_songs;
} 