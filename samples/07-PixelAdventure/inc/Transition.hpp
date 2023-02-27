#pragma once

#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>

#include <filesystem>

class Transition final
{
public:
    Transition()  = default;
    ~Transition() = default;

    Transition( const std::filesystem::path& fileName );

    Transition( const Transition& ) = default;
    Transition( Transition&& )      = default;

    Transition& operator=( const Transition& copy );
    Transition& operator=( Transition&& other ) noexcept;

    /// <summary>
    /// Set the ratio of the transition.
    /// 0 - beginning, 1 - end.
    /// </summary>
    /// <param name="ratio">The ratio [0...1] to set the transition to.</param>
    void setRatio( float ratio );

    /// <summary>
    /// Draw the transition to the image.
    /// </summary>
    /// <param name="image"></param>
    void draw( sr::Image& image ) const;

private:
    std::shared_ptr<sr::Image> transition;
    sr::Sprite                 sprite;

    float                          time     = 0.0f;
    const float                    maxScale = 3.2f;
    std::vector<Math::Transform2D> transforms;
};