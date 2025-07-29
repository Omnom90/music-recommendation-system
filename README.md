# Music Recommendation System

A C++ music recommendation system that prioritizes underground and indie artists over mainstream ones, focusing on musical similarity rather than popularity.

## Features

- **Content-based filtering** using artist and song features
- **Popularity penalty system** to boost underground artists
- **Spotify API integration** for real music data
- **Modular architecture** with clean separation of concerns
- **Command-line interface** for easy interaction

## Project Structure

```
music_recommendation_system/
├── src/                          # Source files
│   ├── main.cpp                  # Entry point
│   ├── data_loader.cpp           # Data parsing and loading
│   ├── feature_extractor.cpp     # Feature extraction logic
│   ├── similarity_calculator.cpp # Similarity algorithms
│   ├── recommendation_engine.cpp # Main recommendation logic
│   ├── popularity_adjuster.cpp   # Popularity penalty system
│   ├── user_interface.cpp        # CLI interface
│   └── spotify_api.cpp           # Spotify API integration
├── include/                      # Header files
│   ├── data_loader.h
│   ├── feature_extractor.h
│   ├── similarity_calculator.h
│   ├── recommendation_engine.h
│   ├── popularity_adjuster.h
│   ├── user_interface.h
│   ├── spotify_api.h
│   └── types.h                   # Core data structures
├── data/                         # Sample data files
│   ├── artists.csv
│   └── songs.csv
├── tests/                        # Unit tests
├── docs/                         # Documentation
├── Makefile                      # Build configuration
└── README.md                     # This file
```

## Prerequisites

- **C++17** compatible compiler (g++ 7.0+ or clang++)
- **libcurl** for HTTP requests
- **nlohmann/json** for JSON parsing
- **Spotify Developer Account** (for API access)

## Installation

### 1. Install Dependencies

**On macOS:**
```bash
brew install curl
```

**On Ubuntu/Debian:**
```bash
sudo apt-get install libcurl4-openssl-dev
```

### 2. Download nlohmann/json

Download `json.hpp` from [https://github.com/nlohmann/json](https://github.com/nlohmann/json) and place it in `include/nlohmann/json.hpp`.

### 3. Set Up Spotify API

1. Create a Spotify Developer account at [https://developer.spotify.com](https://developer.spotify.com)
2. Register your application to get Client ID and Client Secret
3. Update the credentials in `src/spotify_auth.cpp`

## Building

```bash
# Build the main application
make

# Build the Spotify authentication test
g++ -std=c++17 -lcurl -I./include -o spotify_auth src/spotify_auth.cpp

# Clean build files
make clean
```

## Usage

### Basic Usage

```bash
# Run the music recommendation system
./music_recommender

# Test Spotify API authentication
./spotify_auth
```

### Commands

Once running, you can use these commands:
- `artist` - Get artist recommendations
- `song` - Get song recommendations  
- `help` - Display help message
- `exit` - Exit the program

### Example

```
Enter a command (artist/song/help/exit): artist
Enter the artist name: Travis Scott
Experimental Artist -  (Score: 0.932439)
Underground Rapper -  (Score: 0.7922)
Unknown Artist -  (Score: 0.865122)
```

## How It Works

1. **Data Loading**: Loads artist and song data from CSV files or Spotify API
2. **Feature Extraction**: Converts artist/song data into numerical feature vectors
3. **Similarity Calculation**: Uses cosine similarity to compare feature vectors
4. **Popularity Adjustment**: Applies penalties to boost underground artists
5. **Recommendation Generation**: Combines similarity and popularity scores
6. **User Interface**: Provides CLI for user interaction

## API Integration

The system integrates with Spotify API to:
- Fetch real artist and song data
- Get audio features (danceability, energy, valence, etc.)
- Access popularity scores and genre information

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is open source and available under the MIT License.

## Future Enhancements

- [ ] Machine learning integration (K-means clustering)
- [ ] Web interface
- [ ] User preference learning
- [ ] Advanced audio analysis
- [ ] Collaborative filtering 