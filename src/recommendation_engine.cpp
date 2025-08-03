#include "recommendation_engine.h"
#include <algorithm>
#include <iostream>
using namespace std;

// Constructor
RecommendationEngine::RecommendationEngine() : ml_enhancer_(8) {
    // Initialize with 8 clusters for K-means
}

// Train ML models with current data
void RecommendationEngine::trainMLModels(const ArtistDatabase& artists, const SongDatabase& songs) {
    if (!ml_enabled_) return;
    
    // Convert maps to vectors for training
    vector<Artist> artist_list;
    vector<Song> song_list;
    
    for (const auto& [id, artist] : artists) {
        artist_list.push_back(artist);
    }
    
    for (const auto& [id, song] : songs) {
        song_list.push_back(song);
    }
    
    // Train models
    if (!artist_list.empty()) {
        ml_enhancer_.trainArtistModel(artist_list);
    }
    
    if (!song_list.empty()) {
        ml_enhancer_.trainSongModel(song_list);
    }
}

// Recommend similar artists
RecommendationList RecommendationEngine::recommendSimilarArtists(const string& artist_name, 
                                                                const ArtistDatabase& artists, 
                                                                int num_recommendations) {
    RecommendationList results;
    
    // Find the input artist
    auto it = find_if(artists.begin(), artists.end(),
        [&](const auto& pair) { return pair.second.name == artist_name; });
    
    if (it == artists.end()) {
        cout << "Artist not found: " << artist_name << endl;
        return results;
    }
    
    const Artist& input_artist = it->second;

    // Generate base recommendations
    for (const auto& [id, artist] : artists) {
        if (artist.name == artist_name) continue;
        
        double sim = similarity_calc_.calculateArtistSimilarity(input_artist, artist);
        double adj = popularity_adjuster_.adjustForPopularity(sim, artist.popularity_score);
        
        if (meetsPopularityCriteria(artist.popularity_score) && adj > similarity_threshold_) {
            results.push_back({artist.name, "", sim, adj, "Similar artist"});
        }
    }
    
    // Sort by adjusted score
    sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.adjusted_score > b.adjusted_score;
    });
    
    // Limit results
    if (results.size() > static_cast<size_t>(num_recommendations)) {
        results.resize(num_recommendations);
    }
    
    // Apply ML enhancement if enabled and trained
    if (ml_enabled_ && ml_enhancer_.isArtistModelTrained()) {
        results = ml_enhancer_.enhanceArtistRecommendations(results, input_artist, artists);
    }
    
    return results;
}

// Recommend similar songs
RecommendationList RecommendationEngine::recommendSimilarSongs(const string& song_title,
                                                              const SongDatabase& songs,
                                                              const ArtistDatabase& artists,
                                                              int num_recommendations) {
    RecommendationList results;
    
    // Find the input song
    auto it = find_if(songs.begin(), songs.end(),
        [&](const auto& pair) { return pair.second.name == song_title; });
    
    if (it == songs.end()) {
        cout << "Song not found: " << song_title << endl;
        return results;
    }
    
    const Song& input_song = it->second;

    // Generate base recommendations
    for (const auto& [id, song] : songs) {
        if (song.name == song_title) continue;
        
        double sim = similarity_calc_.calculateSongSimilarity(input_song, song);
        double adj = popularity_adjuster_.adjustForPopularity(sim, song.popularity_score);
        
        if (meetsPopularityCriteria(song.popularity_score) && adj > similarity_threshold_) {
            results.push_back({"", song.name, sim, adj, "Similar song"});
        }
    }
    
    // Sort by adjusted score
    sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.adjusted_score > b.adjusted_score;
    });
    
    // Limit results
    if (results.size() > static_cast<size_t>(num_recommendations)) {
        results.resize(num_recommendations);
    }
    
    // Apply ML enhancement if enabled and trained
    if (ml_enabled_ && ml_enhancer_.isSongModelTrained()) {
        results = ml_enhancer_.enhanceSongRecommendations(results, input_song, songs);
    }
    
    return results;
}

// Set similarity threshold
void RecommendationEngine::setSimilarityThreshold(double threshold) {
    similarity_threshold_ = threshold;
}

// Set maximum popularity
void RecommendationEngine::setMaxPopularity(double max_popularity) {
    max_popularity_ = max_popularity;
}

// Enable/disable ML enhancement
void RecommendationEngine::enableML(bool enable) {
    ml_enabled_ = enable;
}

// Check if popularity meets criteria
bool RecommendationEngine::meetsPopularityCriteria(double popularity_score) {
    return popularity_score <= max_popularity_;
}