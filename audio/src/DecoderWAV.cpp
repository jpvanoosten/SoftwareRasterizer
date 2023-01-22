#include <DecoderWAV.hpp>

#include "dr_wav.h"

namespace Audio
{
class DecoderWAVImpl : public Decoder
{
public:
    DecoderWAVImpl() = default;
    ~DecoderWAVImpl() override;

    bool        open( const std::filesystem::path& filePath, std::size_t& sampleCount, uint32_t& channelCount, uint32_t& sampleRate ) override;
    void        seek( std::size_t sampleOffset ) override;
    std::size_t read( std::int16_t* samples, std::size_t maxSamples ) override;

private:
    bool  isValid = false;
    drwav wav {};
};
}  // namespace Audio

using namespace Audio;

DecoderWAVImpl::~DecoderWAVImpl()
{
    drwav_uninit( &wav );
}

bool DecoderWAVImpl::open( const std::filesystem::path& filePath, std::size_t& sampleCount, uint32_t& channelCount, uint32_t& sampleRate )
{
    if ( drwav_init_file_w( &wav, filePath.c_str(), nullptr ) == DRWAV_TRUE )
    {
        sampleCount  = wav.totalPCMFrameCount;
        channelCount = wav.channels;
        sampleRate   = wav.sampleRate;

        isValid = true;

        return true;
    }

    return false;
}

void DecoderWAVImpl::seek( std::size_t sampleOffset )
{
    if ( isValid )
        drwav_seek_to_pcm_frame( &wav, sampleOffset );
};

std::size_t DecoderWAVImpl::read( std::int16_t* samples, std::size_t maxSamples )
{
    if ( isValid )
        return drwav_read_pcm_frames_s16( &wav, maxSamples, samples );

    return 0;
}

DecoderWAV::DecoderWAV()
: impl { std::make_unique<DecoderWAVImpl>() }
{}

bool DecoderWAV::test( const std::filesystem::path& filePath )
{
    if ( filePath.has_extension() && filePath.extension() == ".wav" )
    {
        return true;
    }

    return false;
}

bool DecoderWAV::open( const std::filesystem::path& filePath, std::size_t& sampleCount, uint32_t& channelCount, uint32_t& sampleRate )
{
    return impl->open( filePath, sampleCount, channelCount, sampleRate );
}

void DecoderWAV::seek( std::size_t sampleOffset )
{
    impl->seek( sampleOffset );
}

std::size_t DecoderWAV::read( std::int16_t* samples, std::size_t maxSamples )
{
    return impl->read( samples, maxSamples );
}
