#include "user_interface.h"
#include "data_loader.h"
#include <iostream>
using namespace std;

void UserInterface::run() {
    loadData();
    displayWelcome();

    string command;
    while(true) {
        cout << "Enter a command (artist/song/help/exit): ";
        getline(cin, command);

        if(!processUserCommand(command)) break;
    }
}

void UserInterface::displayWelcome() {
    cout << "Welcome to the Music Recommender System!" << endl;
}

void UserInterface::displayRecommendations(const RecommendationList& recommendations) {
    if(recommendations.empty()) {
        cout << "No recommendations found." << endl;
        return;
    }

    for(const auto& rec : recommendations) {
        cout << rec.artist_name << " - " << rec.song_title << " (Score: " << rec.similarity_score << ")\n";
    }
}

void UserInterface::displayHelp() {
    cout << "Available commands:\n";
    cout << "artist - Get artist recommendations\n";
    cout << "song - Get song recommendations\n";
    cout << "help - Display this help message\n";
    cout << "exit - Exit the program\n";
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
    } else {
        cout << "Invalid command. Type 'help' for available commands." << endl;
    }
    return true;
}

void UserInterface::loadData() {
    DataLoader loader;

    loader.loadArtistsFromCSV("data/artists.csv", artists_);
    loader.loadSongsFromCSV("data/songs.csv", songs_);
}

void UserInterface::handleArtistRecommendation(const string& artist_name) {
    auto recs = engine_.recommendSimilarArtists(artist_name, artists_);
    displayRecommendations(recs);
}

void UserInterface::handleSongRecommendation(const string& song_title) {
    auto recs = engine_.recommendSimilarSongs(song_title, songs_, artists_);
    displayRecommendations(recs);
}