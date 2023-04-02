#pragma once

/// <summary>
/// There are three different kinds of bricks: colored bricks, silver bricks, and gold bricks.
/// Bricks of different colors make up the majority of the walls in each area.
/// They only require one hit to defeat, but they vary in point value from one another.
/// Silver and Gold bricks, on the other hand, cannot be destroyed with one hit.
/// In fact, Gold bricks cannot be destroyed at all. They are indestructible,
/// and therefore not counted against you towards your attempt to clear an area of bricks.
/// Silver bricks take more than one hit to destroy. In the beginning, they only require two hits to destroy,
/// but the number of hits it takes to remove them increases by one every eight stages.
/// To calculate the number of points Silver bricks are worth, multiply 50 by the stage number.
/// </summary>
namespace Stages
{
// Bricks
enum B
{
    No,  // None
    Wh,  // White
    Or,  // Orange
    Cy,  // Cyan
    Gr,  // Green
    Re,  // Red
    Bl,  // Blue
    Ma,  // Magenta
    Ye,  // Yellow
    Si,  // Silver
    Go,  // Gold.
};

/// <summary>
/// A Stage consists of (a maximum) of 18 rows of 13 bricks.
/// </summary>
struct Stage
{
    B b[18][13];
};

// There are 32 stages:
inline Stage stages[] = {
    // Stage 1
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si },
        { Re, Re, Re, Re, Re, Re, Re, Re, Re, Re, Re, Re, Re },
        { Ye, Ye, Ye, Ye, Ye, Ye, Ye, Ye, Ye, Ye, Ye, Ye, Ye },
        { Bl, Bl, Bl, Bl, Bl, Bl, Bl, Bl, Bl, Bl, Bl, Bl, Bl },
        { Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma },
        { Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
    } },
    // Stage 2
};

}  // namespace Levels
