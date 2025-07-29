#pragma once
#include "types.h"
#include <string>
using namespace std;

class DataLoader {
public:
    // loading artits and songs from csv files
    bool loadArtistsFromCSV(const string& filename, ArtistDatabase& artists);
    bool loadSongsFromCSV(const string& filename, SongDatabase& songs);

    // loading data from json files
    bool loadFromJSON(const string& filename, ArtistDatabase& artists, SongDatabase& songs);

    // validating the loaded data
    bool validateData(const ArtistDatabase& artists, const SongDatabase& songs);

private:
    // helper methods and functions for parsing

    Artist parseArtistFromCSV(const string& line);
    Song parseSongFromCSV(const string& line);
};