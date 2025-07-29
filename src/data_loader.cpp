#include "data_loader.h"
#include <fstream>
#include <sstream>

bool DataLoader::loadArtistsFromCSV(const string& filename, ArtistDatabase& artists) {
    ifstream file(filename);

    if(!file.is_open()) return false;

    string line;
    bool first_line = true; // Skip header

    while(getline(file, line)) {
        if (first_line) {
            first_line = false;
            continue;
        }
        Artist artist = parseArtistFromCSV(line);
        artists[artist.id] = artist;
    }
    return true;
}

bool DataLoader::loadSongsFromCSV(const string& filename, SongDatabase& songs) {
    ifstream file(filename);

    if(!file.is_open()) return false;

    string line;
    bool first_line = true; // Skip header

    while(getline(file, line)) {
        if (first_line) {
            first_line = false;
            continue;
        }
        Song song = parseSongFromCSV(line);
        songs[song.id] = song;
    }
    return true;
}

Artist DataLoader::parseArtistFromCSV(const string& line) {
    Artist artist;
    stringstream ss(line);
    string item;

    getline(ss, artist.id, ',');
    getline(ss, artist.name, ',');
    getline(ss, artist.genre, ',');
    getline(ss, item, ',');
    try {
        artist.popularity_score = stod(item);
    } catch (const std::invalid_argument& e) {
        artist.popularity_score = 0.0; // Default value
    }

    getline(ss, item, ',');
    stringstream tagss(item);
    while(getline(tagss, item, ';')) {
        artist.tags.push_back(item);
    }
    return artist;
}

Song DataLoader::parseSongFromCSV(const string& line) {
    Song song;
    stringstream ss(line);
    string item;

    getline(ss, song.id, ',');
    getline(ss, song.name, ',');
    getline(ss, song.artist_id, ',');
    getline(ss, item, ',');
    try {
        song.popularity_score = stod(item);
    } catch (const std::invalid_argument& e) {
        song.popularity_score = 0.0; // Default value
    }

    getline(ss, item, ',');
    stringstream fss(item);
    while(getline(fss, item, ';')) {
        try {
            song.features.push_back(stod(item));
        } catch (const std::invalid_argument& e) {
            song.features.push_back(0.0); // Default value
        }
    }
    return song;
}