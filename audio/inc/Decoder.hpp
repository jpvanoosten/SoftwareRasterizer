#pragma once

#include "Config.hpp"

#include <filesystem>

namespace Audio
{
/// <summary>
/// Base class for audio decoders.
/// </summary>
class AUDIO_API Decoder
{
public:
    /// <summary>
    /// Open an audio file and retrieve the audio information from the file.
    /// </summary>
    /// <param name="filePath">The path to the audio file.</param>
    /// <param name="sampleCount">The number of audio samples in the file.</param>
    /// <param name="channelCount">The number of audio channels.</param>
    /// <param name="sampleRate">Number of samples per second.</param>
    /// <returns>`true` if the file was opened successfully, `false` otherwise.</returns>
    virtual bool open( const std::filesystem::path& filePath, std::size_t& sampleCount, uint32_t& channelCount, uint32_t& sampleRate ) = 0;

    /// <summary>
    /// Adjust the read position in the audio file.
    /// If you want to seek to a particular time in the file, use:
    /// `timeInSeconds * sampleRate * channelCount`.
    /// </summary>
    /// <param name="sampleOffset">The sample offset to seek to.</param>
    virtual void seek( std::size_t sampleOffset ) = 0;

    /// <summary>
    /// Read audio samples from the open file.
    /// </summary>
    /// <param name="samples">An array to fill with samples.</param>
    /// <param name="maxSamples">The maximum number of samples to read.</param>
    /// <returns>The number of samples actually read.</returns>
    virtual std::size_t read( std::int16_t* samples, std::size_t maxSamples ) = 0;

protected:
    Decoder()                            = default;
    Decoder( const Decoder& )            = default;
    Decoder( Decoder&& )                 = default;
    virtual ~Decoder()                   = default;
    Decoder& operator=( const Decoder& ) = default;
    Decoder& operator=( Decoder&& )      = default;
};
}  // namespace Audio
