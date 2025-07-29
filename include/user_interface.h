#pragma once
#include "types.h"
#include "recommendation_engine.h"
#include <string> 
using namespace std;

class UserInterface {
public:

    // main intereface loop
    void run();

    // display function
    void displayWelcome();
    void displayRecommendations(const RecommendationList& recommendations);
    void displayHelp();

    // input handling
    string getUserInput(const string& prompt);
    bool processUserCommand(const string& command);

private:
    RecommendationEngine engine_;
    ArtistDatabase artists_;
    SongDatabase songs_;

    // helper methods and functions
    void loadData();
    void handleArtistRecommendation(const string& artist_name);
    void handleSongRecommendation(const string& song_title);
};