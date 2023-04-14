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
    Go,  // Gold
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
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Wh, No, No, No, No, No, No, No, No, No, No, No, No },
        { Wh, Or, No, No, No, No, No, No, No, No, No, No, No },
        { Wh, Or, Cy, No, No, No, No, No, No, No, No, No, No },
        { Wh, Or, Cy, Gr, No, No, No, No, No, No, No, No, No },
        { Wh, Or, Cy, Gr, Re, No, No, No, No, No, No, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, No, No, No, No, No, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, Ma, No, No, No, No, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, Ma, Ye, No, No, No, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, Ma, Ye, Wh, No, No, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, Ma, Ye, Wh, Or, No, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, Ma, Ye, Wh, Or, Cy, No, No },
        { Wh, Or, Cy, Gr, Re, Bl, Ma, Ye, Wh, Or, Cy, Gr, No },
        { Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Re },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
    } },
    // Stage 3
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr, Gr },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Wh, Wh, Wh, Go, Go, Go, Go, Go, Go, Go, Go, Go, Go },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Re, Re, Re, Re, Re, Re, Re, Re, Re, Re, Re, Re, Re },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Go, Go, Go, Go, Go, Go, Go, Go, Go, Go, Wh, Wh, Wh },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma, Ma },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Bl, Bl, Bl, Go, Go, Go, Go, Go, Go, Go, Go, Go, Go },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Cy, Cy, Cy, Cy, Cy, Cy, Cy, Cy, Cy, Cy, Cy, Cy, Cy },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Go, Go, Go, Go, Go, Go, Go, Go, Go, Go, Cy, Cy, Cy },
    } },
    // Stage 4
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, Or, Cy, Gr, Si, Bl, No, Ye, Wh, Or, Cy, Gr, No },
        { No, Cy, Gr, Si, Bl, Ma, No, Wh, Or, Cy, Gr, Si, No },
        { No, Gr, Si, Bl, Ma, Ye, No, Or, Cy, Gr, Si, Bl, No },
        { No, Si, Bl, Ma, Ye, Wh, No, Cy, Gr, Si, Bl, Ma, No },
        { No, Bl, Ma, Ye, Wh, Or, No, Gr, Si, Bl, Ma, Ye, No },
        { No, Ma, Ye, Wh, Or, Cy, No, Si, Bl, Ma, Ye, Wh, No },
        { No, Ye, Wh, Or, Cy, Gr, No, Bl, Ma, Ye, Wh, Or, No },
        { No, Wh, Or, Cy, Gr, Si, No, Ma, Ye, Wh, Or, Cy, No },
        { No, Or, Cy, Gr, Si, Bl, No, Ye, Wh, Or, Cy, Gr, No },
        { No, Cy, Gr, Si, Bl, Ma, No, Wh, Or, Cy, Gr, Si, No },
        { No, Gr, Si, Bl, Ma, Ye, No, Or, Cy, Gr, Si, Bl, No },
        { No, Si, Bl, Ma, Ye, Wh, No, Cy, Gr, Si, Bl, Ma, No },
        { No, Bl, Ma, Ye, Wh, Or, No, Gr, Si, Bl, Ma, Ye, No },
        { No, Ma, Ye, Wh, Or, Cy, No, Si, Bl, Ma, Ye, Wh, No },
    } },
    // Stage 5
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, Ye, No, No, No, No, No, Ye, No, No, No },
        { No, No, No, Ye, No, No, No, No, No, Ye, No, No, No },
        { No, No, No, No, Ye, No, No, No, Ye, No, No, No, No },
        { No, No, No, No, Ye, No, No, No, Ye, No, No, No, No },
        { No, No, No, Si, Si, Si, Si, Si, Si, Si, No, No, No },
        { No, No, No, Si, Si, Si, Si, Si, Si, Si, No, No, No },
        { No, No, Si, Si, Re, Si, Si, Si, Re, Si, Si, No, No },
        { No, No, Si, Si, Re, Si, Si, Si, Re, Si, Si, No, No },
        { No, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, No },
        { No, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, No },
        { No, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, Si, No },
        { No, Si, No, Si, Si, Si, Si, Si, Si, Si, No, Si, No },
        { No, Si, No, Si, No, No, No, No, No, Si, No, Si, No },
        { No, Si, No, Si, No, No, No, No, No, Si, No, Si, No },
        { No, No, No, No, Si, Si, No, Si, Si, No, No, No, No },
        { No, No, No, No, Si, Si, No, Si, Si, No, No, No, No },
    } },
    // Stage 6
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Go, Or, Go, Or, Go, Or, Go, Or, Go, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { Or, No, Or, No, Go, No, Or, No, Go, No, Or, No, Or },
        { Bl, No, Re, No, Gr, No, Cy, No, Gr, No, Re, No, Bl },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
    } },
    // Stage 7
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, Ye, Ye, Ma, No, No, No, No, No },
        { No, No, No, No, Ye, Ye, Ma, Ma, Bl, No, No, No, No },
        { No, No, No, Ye, Ye, Ma, Ma, Bl, Bl, Re, No, No, No },
        { No, No, No, Ye, Ma, Ma, Bl, Bl, Re, Re, No, No, No },
        { No, No, Ye, Ma, Ma, Bl, Bl, Re, Re, Gr, Gr, No, No },
        { No, No, Ma, Ma, Bl, Bl, Re, Re, Gr, Gr, Cy, No, No },
        { No, No, Ma, Bl, Bl, Re, Re, Gr, Gr, Cy, Cy, No, No },
        { No, No, Bl, Bl, Re, Re, Gr, Gr, Cy, Cy, Or, No, No },
        { No, No, Bl, Re, Re, Gr, Gr, Cy, Cy, Or, Or, No, No },
        { No, No, Re, Re, Gr, Gr, Cy, Cy, Or, Or, Wh, No, No },
        { No, No, No, Gr, Gr, Cy, Cy, Or, Or, Wh, No, No, No },
        { No, No, No, Gr, Cy, Cy, Or, Or, Wh, Wh, No, No, No },
        { No, No, No, No, Cy, Or, Or, Wh, Wh, No, No, No, No },
        { No, No, No, No, No, Or, Wh, Wh, No, No, No, No, No },
    } },
    // Stage 8
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, Go, No, Go, No, Go, No, Go, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, Go, No },
        { No, Go, Go, No, Go, No, No, No, Go, No, Go, Go, No },
        { No, No, No, No, No, No, Wh, No, No, No, No, No, No },
        { No, Go, No, No, No, Go, Or, Go, No, No, No, Go, No },
        { No, No, No, Go, No, No, Cy, No, No, Go, No, No, No },
        { No, No, No, No, No, No, Gr, No, No, No, No, No, No },
        { No, No, No, Go, No, No, Re, No, No, Go, No, No, No },
        { No, Go, No, No, No, Go, Bl, Go, No, No, No, Go, No },
        { No, No, No, No, No, No, Ma, No, No, No, No, No, No },
        { No, Go, Go, No, Go, No, No, No, Go, No, Go, Go, No },
        { No, Go, No, No, No, No, No, No, No, No, No, Go, No },
        { No, No, No, Go, No, Go, No, Go, No, Go, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
    } },
    // Stage 9
    { {
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, Go, No, No, No, No, No, Go, No, Go, No },
        { No, Go, Gr, Go, No, No, No, No, No, Go, Gr, Go, No },
        { No, Go, Cy, Go, No, No, No, No, No, Go, Cy, Go, No },
        { No, Go, Go, Go, No, No, No, No, No, Go, Go, Go, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, Ma, Wh, Wh, Wh, Ye, No, No, No, No, No },
        { No, No, No, Ma, Or, Or, Or, Ye, No, No, No, No, No },
        { No, No, No, Ma, Cy, Cy, Cy, Ye, No, No, No, No, No },
        { No, No, No, Ma, Gr, Gr, Gr, Ye, No, No, No, No, No },
        { No, No, No, Ma, Re, Re, Re, Ye, No, No, No, No, No },
        { No, No, No, Ma, Bl, Bl, Bl, Ye, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
    } },
    // Stage 10
    { {
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, No, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, No, No, No, No, Bl, No, No, No, No, No },
        { No, Go, No, No, No, No, Bl, Cy, Bl, No, No, No, No },
        { No, Go, No, No, No, Bl, Cy, Wh, Cy, Bl, No, No, No },
        { No, Go, No, No, Bl, Cy, Wh, Cy, Wh, Cy, Bl, No, No },
        { No, Go, No, Bl, Cy, Wh, Cy, Si, Cy, Wh, Cy, Bl, No },
        { No, Go, No, No, Bl, Cy, Wh, Cy, Wh, Cy, Bl, No, No },
        { No, Go, No, No, No, Bl, Cy, Wh, Cy, Bl, No, No, No },
        { No, Go, No, No, No, No, Bl, Cy, Bl, No, No, No, No },
        { No, Go, No, No, No, No, No, Bl, No, No, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, No, No, No, No, No, No, No, No, No, No, No },
        { No, Go, Go, Go, Go, Go, Go, Go, Go, Go, Go, Go, Go },
    } },
};

}  // namespace Stages
