#include "user_interface.h"
#include "data_loader.h"
#include "spotify_api.h"
#include "spotify_auth.h"
#include <iostream>
#include <iomanip> // Required for fixed and setprecision
#include <vector>
#include <map>
#include <thread> // Required for this_thread::sleep_for
#include <chrono> // Required for chrono::milliseconds
using namespace std;

void UserInterface::run() {
    loadData();
    displayWelcome();

    string command;
    while(true) {
        cout << "\nEnter a command (artist/song/help/spotify/ml/exit): ";
        getline(cin, command);

        if(!processUserCommand(command)) break;
    }
}

void UserInterface::displayWelcome() {
    cout << "=== Music Recommendation System ===" << endl;
    cout << "Enhanced with Spotify API and Machine Learning" << endl;
    cout << "=============================================" << endl;
}

void UserInterface::displayRecommendations(const RecommendationList& recommendations) {
    if(recommendations.empty()) {
        cout << "No recommendations found." << endl;
        return;
    }

    cout << "\n=== Recommendations ===" << endl;
    for(size_t i = 0; i < recommendations.size(); ++i) {
        const auto& rec = recommendations[i];
        cout << (i + 1) << ". " << rec.artist_name << " - " << rec.song_title << endl;
        cout << "   Score: " << fixed << setprecision(3) << rec.adjusted_score;
        cout << " (Base: " << rec.similarity_score << ")" << endl;
        cout << "   Reason: " << rec.reason << endl;
        cout << endl;
    }
}

void UserInterface::displayHelp() {
    cout << "\n=== Available Commands ===" << endl;
    cout << "artist    - Get artist recommendations" << endl;
    cout << "song      - Get song recommendations" << endl;
    cout << "spotify   - Load data from Spotify API" << endl;
    cout << "ml        - Train/re-train ML models" << endl;
    cout << "help      - Display this help message" << endl;
    cout << "exit      - Exit the program" << endl;
    cout << "=========================" << endl;
}

string UserInterface::getUserInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

bool UserInterface::processUserCommand(const string& command) {
    if(command == "help") {
        displayHelp();
    } else if(command == "exit") {
        return false;
    } else if(command == "song") {
        string song_title = getUserInput("Enter the song title: ");
        handleSongRecommendation(song_title);
    } else if(command == "artist") {
        string artist_name = getUserInput("Enter the artist name: ");
        handleArtistRecommendation(artist_name);
    } else if(command == "spotify") {
        loadSpotifyData();
    } else if(command == "ml") {
        trainMLModels();
    } else {
        cout << "Invalid command. Type 'help' for available commands." << endl;
    }
    return true;
}

void UserInterface::loadData() {
    cout << "Loading data..." << endl;
    
    DataLoader loader;
    bool loaded = loader.loadArtistsFromCSV("data/artists.csv", artists_);
    loaded &= loader.loadSongsFromCSV("data/songs.csv", songs_);
    
    if (loaded) {
        cout << "Loaded " << artists_.size() << " artists and " << songs_.size() << " songs from CSV." << endl;
        
        // Train ML models with loaded data
        trainMLModels();
    } else {
        cout << "Failed to load data from CSV files." << endl;
    }
}

void UserInterface::loadSpotifyData() {
    cout << "\n=== Loading Data from Spotify API ===" << endl;
    
    // First, get access token
    string access_token = getSpotifyAccessToken();
    if (access_token.empty()) {
        cout << "Failed to get Spotify access token." << endl;
        return;
    }
    
    // Initialize Spotify API
    SpotifyAPI spotify_api;
    spotify_api.setAccessToken(access_token);
    
    // Get artist names to search for
    vector<string> artist_names = {
        "Travis Scott", "Drake", "Kendrick Lamar", "J. Cole", "Post Malone",
        "The Weeknd", "Dua Lipa", "Billie Eilish", "Taylor Swift", "Ariana Grande",
        "Bad Bunny", "Ed Sheeran", "Justin Bieber", "BTS", "Blackpink"
    };
    
    cout << "Searching for artists on Spotify..." << endl;
    artists_.clear();
    
    for (const auto& name : artist_names) {
        Artist artist = spotify_api.searchArtist(name);
        if (!artist.id.empty()) {
            artists_[artist.id] = artist;
            cout << "Found: " << artist.name << " (" << artist.genre << ")" << endl;
            
            // Get top tracks for this artist
            vector<Song> tracks = spotify_api.getArtistTopTracks(artist.id);
            for (const auto& track : tracks) {
                songs_[track.id] = track;
            }
        }
        
        // Add delay to respect rate limits
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    
    // Get audio features for all songs
    cout << "Getting audio features for songs..." << endl;
    vector<Song> song_list;
    for (const auto& [id, song] : songs_) {
        song_list.push_back(song);
    }
    spotify_api.populateAudioFeatures(song_list);
    
    // Update songs_ with the enhanced data
    songs_.clear();
    for (const auto& song : song_list) {
        songs_[song.id] = song;
    }
    
    cout << "Loaded " << artists_.size() << " artists and " << songs_.size() << " songs from Spotify." << endl;
    
    // Train ML models with new data
    trainMLModels();
}

string UserInterface::getSpotifyAccessToken() {
    // This is a simplified version - in a real app, you'd want to cache the token
    cout << "Getting Spotify access token..." << endl;
    
    // For now, return a placeholder - you'd implement the actual token retrieval here
    // In practice, you'd call your spotify_auth.cpp functionality
    return "your_access_token_here";
}

void UserInterface::trainMLModels() {
    if (artists_.empty() && songs_.empty()) {
        cout << "No data available to train ML models." << endl;
        return;
    }
    
    cout << "Training machine learning models..." << endl;
    engine_.trainMLModels(artists_, songs_);
    cout << "ML models trained successfully!" << endl;
}

void UserInterface::handleArtistRecommendation(const string& artist_name) {
    cout << "\nGetting recommendations for: " << artist_name << endl;
    auto recs = engine_.recommendSimilarArtists(artist_name, artists_);
    displayRecommendations(recs);
}

void UserInterface::handleSongRecommendation(const string& song_title) {
    cout << "\nGetting recommendations for: " << song_title << endl;
    auto recs = engine_.recommendSimilarSongs(song_title, songs_, artists_);
    displayRecommendations(recs);
}