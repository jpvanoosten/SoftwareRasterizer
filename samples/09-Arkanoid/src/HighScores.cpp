#include <HighScores.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

using namespace std::filesystem;

// A regular expression to parse a line in the highscores text file.
static const std::regex re( R"((\d*),(\d*),(\w{3}))" );

HighScores::HighScores( const std::filesystem::path& file )
{
    load( file );
}

void HighScores::load( const std::filesystem::path& file )
{
    if ( exists( file ) && is_regular_file( file ) )
    {
        auto        f      = std::ifstream( file );
        bool        header = true;
        std::string row;

        scores.clear();

        while ( std::getline( f, row ) )
        {
            // Ignore the header row.
            if ( header )
            {
                header = false;
                continue;
            }

            std::smatch match;
            if ( std::regex_match( row, match, re ) )
            {
                HighScore hs;
                hs.score = std::stoi( match[1].str() );
                hs.round = std::stoi( match[2].str() );
                hs.name  = match[3].str();

                scores.push_back( hs );
            }
        }
    }
    else
    {
        std::cerr << "Failed to load high score file: " << file.string() << std::endl;
    }
}

void HighScores::save( const std::filesystem::path& file )
{
    auto f = std::ofstream( file );

    // Write the header row.
    f << "SCORE,ROUND,NAME\n";

    // Write highscores.
    for (const auto& [score, round, name] : scores )
    {
        f << score << "," << round << "," << name << "\n";
    }
}

void HighScores::addScore( const HighScore& score )
{
    scores.push_back( score );
    // Sort scores in descending order.
    std::ranges::sort( scores, []( const HighScore& a, const HighScore& b ) { return a.score < b.score; } );
}

int HighScores::getHighScore() const noexcept
{
    return !scores.empty() ? scores[0].score : 0;
}
