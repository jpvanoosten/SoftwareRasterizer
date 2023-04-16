#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct HighScore
{
    int         score;
    int         round;
    std::string name;
};

class HighScores
{
public:
    HighScores() = default;

    explicit HighScores( const std::filesystem::path& file );

    /// <summary>
    /// Load the high scores from a file.
    /// </summary>
    /// <param name="file">The file to load the high scores from.</param>
    void load( const std::filesystem::path& file );

    /// <summary>
    /// Save the highscores to a file.
    /// </summary>
    /// <param name="file">The file to save the high scores to.</param>
    void save( const std::filesystem::path& file );

    /// <summary>
    /// Add a high score.
    /// </summary>
    /// <param name="score">The high score to add.</param>
    void addScore( const HighScore& score );

    /// <summary>
    /// Get the current highest score.
    /// </summary>
    /// <returns>The highest score.</returns>
    int getHighScore() const noexcept;

private:
    std::vector<HighScore> scores;
};