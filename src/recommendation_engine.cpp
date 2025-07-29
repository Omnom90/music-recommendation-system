#include "recommendation_engine.h"
#include <algorithm>
using namespace std;

// Recommend similar artists
RecommendationList RecommendationEngine::recommendSimilarArtists(const string& artist_name, 
                                                                const ArtistDatabase& artists, 
                                                                int num_recommendations) {
    RecommendationList results;
    auto it = find_if(artists.begin(), artists.end(),
        [&](const auto& pair) { return pair.second.name == artist_name; });
    if (it == artists.end()) return results;
    const Artist& input_artist = it->second;

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
    if (results.size() > static_cast<size_t>(num_recommendations))
        results.resize(num_recommendations);
    return results;
}

// Recommend similar songs
RecommendationList RecommendationEngine::recommendSimilarSongs(const string& song_title,
                                                              const SongDatabase& songs,
                                                              const ArtistDatabase& artists,
                                                              int num_recommendations) {
    RecommendationList results;
    auto it = find_if(songs.begin(), songs.end(),
        [&](const auto& pair) { return pair.second.name == song_title; });
    if (it == songs.end()) return results;
    const Song& input_song = it->second;

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
    if (results.size() > static_cast<size_t>(num_recommendations))
        results.resize(num_recommendations);
    return results;
}

void RecommendationEngine::setSimilarityThreshold(double threshold) {
    similarity_threshold_ = threshold;
}

void RecommendationEngine::setMaxPopularity(double max_popularity) {
    max_popularity_ = max_popularity;
}

bool RecommendationEngine::meetsPopularityCriteria(double popularity_score) {
    return popularity_score <= max_popularity_;
}