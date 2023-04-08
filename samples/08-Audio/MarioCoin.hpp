#pragma once

#include <Audio/Waveform.hpp>

#include <vector>

/// <summary>
/// Defines a point along a curve.
/// </summary>
struct Point
{
    float t;  ///< The time (or x) value.
    float v;  ///< The value at time (t).
};

class MarioCoin
{
public:
    MarioCoin();

    /// <summary>
    /// Update the mario coin sound effect.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds)</param>
    void update( float deltaTime );

    /// <summary>
    /// Start playing the sound effect.
    /// </summary>
    void play();

    /// <summary>
    /// Stop playing the sound effect.
    /// </summary>
    void stop();

private:
    static constexpr inline float duration = 0.9f;
    float amplitude = 0.2f;

    float totalTime = 0.0f;
    bool  isPlaying = false;

    Audio::Waveform waveform;
};