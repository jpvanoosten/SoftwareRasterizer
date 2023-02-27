#pragma once

#include <filesystem>
#include <format>
#include <fstream>
#include <span>
#include <vector>

namespace Graphics
{
/// <summary>
/// Helper functions for reading/writing files.
/// </summary>
class File final
{
public:
    /// <summary>
    /// Read a file from disk and return the data that was read.
    /// </summary>
    /// <remarks>
    /// Tries to open a file for reading and returns the internal file data.
    /// This function throws an exception if either the file does not exist, the file is not a regular file, the file is empty,
    /// or some other permission error occurred when trying to open the file for reading.
    /// </remarks>
    /// <typeparam name="T">The format of the data to be returned. Typically, this will just be a byte data (default type).</typeparam>
    /// <param name="path">The path the the file to load.</param>
    /// <param name="mode">The stream open mode. Use std::ios::binary to read binary data.</param>
    /// <exception cref="std::invalid_argument">If the file could not be opened for reading or the file is empty.</exception>
    /// <exception cref="std::ios_base::failure">If an error occurred while reading the file.</exception>
    /// <returns>A vector containing the loaded file contents.</returns>
    template<typename T = char>
    static std::vector<T> readFile( const std::filesystem::path& path, std::ios::openmode mode = std::ios::in );

    /// <summary>
    /// Write content to disk.
    /// </summary>
    /// <typeparam name="T">The type of data to write. Typically this is just the byte data (default type).</typeparam>
    /// <param name="path">The path of the file to write.</param>
    /// <param name="data">The data to write.</param>
    /// <param name="mode">The output stream open mode.</param>
    /// <exception cref="std::ios_base::failure">If an error occurred while trying to write the file.</exception>
    template<typename T = char>
    static void writeFile( const std::filesystem::path& path, std::span<T> data, std::ios::openmode mode = std::ios::out );

    File() = delete;
    File( const File&) = delete;
    File( File&& )      = delete;
    ~File()            = delete;
    File& operator=( const File& ) = delete;
    File& operator=( File&& )      = delete;
};

template<typename T>
std::vector<T> File::readFile( const std::filesystem::path& path, std::ios::openmode mode )
{
    if ( auto input = std::ifstream { path, mode } )
    {
        if ( const auto fileSize = file_size( path ); fileSize > 0 )
        {
            std::vector<T> data( fileSize / sizeof( T ) );
            input.read( reinterpret_cast<char*>( data.data() ), data.size() * sizeof( T ) );
            return data;
        }

        throw std::invalid_argument( std::format( "File is empty: {}", path.string() ) );
    }

    throw std::invalid_argument( std::format( "Failed to open file: {}", path.string() ) );
}

template<typename T>
void File::writeFile( const std::filesystem::path& path, std::span<T> data, std::ios::openmode mode )
{
    // Ensure the directories exists.
    auto p = path;
    create_directories( p.remove_filename() );

    auto output = std::ofstream { path, mode };

    // Throw exceptions if anything goes wrong...
    output.exceptions( std::ios::badbit | std::ios::failbit );
    output.write( reinterpret_cast<char*>( data.data() ), data.size() * sizeof( T ) );
}

}  // namespace Graphics
