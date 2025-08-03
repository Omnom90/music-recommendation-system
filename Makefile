CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
SRCDIR = src
OBJDIR = build
TARGET = music_recommender

# Source files (exclude spotify_auth.cpp as it has its own main)
SOURCES = $(filter-out $(SRCDIR)/spotify_auth.cpp, $(wildcard $(SRCDIR)/*.cpp))
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -lcurl

# Create object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build Spotify auth test
spotify_auth: src/spotify_auth.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -lcurl

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET) spotify_auth

# Run the program
run: $(TARGET)
	./$(TARGET)

# Test Spotify API
test_spotify: spotify_auth
	./spotify_auth

.PHONY: all clean run test_spotify 