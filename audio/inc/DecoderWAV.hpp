#pragma once

#include "Decoder.hpp"

#include <memory>

namespace Audio
{
class DecoderWAVImpl;

class AUDIO_API DecoderWAV : public Decoder
{
public:
    DecoderWAV();
    DecoderWAV( const DecoderWAV& )     = delete;
    DecoderWAV( DecoderWAV&& ) noexcept = default;
    ~DecoderWAV() override              = default;

    DecoderWAV& operator=( const DecoderWAV& )     = delete;
    DecoderWAV& operator=( DecoderWAV&& ) noexcept = default;

    /// <summary>
    /// Test to see if this decoder can decode the given file.
    /// </summary>
    /// <param name="filePath">The path to the audio file to decode.</param>
    /// <returns>`true` if this decoder can handle the given file, `false` otherwise.</returns>
    static bool test( const std::filesystem::path& filePath );

    /// <summary>
    /// Open an audio file and retrieve the audio information from the file.
    /// </summary>
    /// <param name="filePath">The path to the audio file.</param>
    /// <param name="sampleCount">The number of audio samples in the file.</param>
    /// <param name="channelCount">The number of audio channels.</param>
    /// <param name="sampleRate">Number of samples per second.</param>
    /// <returns>`true` if the file was opened successfully, `false` otherwise.</returns>
    bool open( const std::filesystem::path& filePath, std::size_t& sampleCount, uint32_t& channelCount, uint32_t& sampleRate ) override;

    /// <summary>
    /// Adjust the read position in the audio file.
    /// If you want to seek to a particular time in the file, use:
    /// `timeInSeconds * sampleRate * channelCount`.
    /// </summary>
    /// <param name="sampleOffset">The sample offset to seek to.</param>
    void seek( std::size_t sampleOffset ) override;

    /// <summary>
    /// Read audio samples from the open file.
    /// </summary>
    /// <param name="samples">An array to fill with samples.</param>
    /// <param name="maxSamples">The maximum number of samples to read.</param>
    /// <returns>The number of samples actually read.</returns>
    std::size_t read( std::int16_t* samples, std::size_t maxSamples ) override;

private:
    std::unique_ptr<DecoderWAVImpl> impl;
};
}  // namespace Audio
