#include "CameraController.hpp"

#include <Graphics/Input.hpp>

#include <algorithm>
#include <iostream>

using namespace Graphics;
using namespace Math;

CameraController::CameraController( const glm::vec3& pos, float pitch, float yaw )
: initialState { pos, pitch, yaw }
{
    // Setup input.
    Input::mapAxis( "camera_forward", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftY = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();  // NOLINT(clang-diagnostic-shadow)

            leftY += state.thumbSticks.leftY;
        }

        const auto keyState = keyboardState.getLastState();

        const float s    = keyState.S ? 1.0f : 0.0f;
        const float w    = keyState.W ? 1.0f : 0.0f;
        const float up   = keyState.Up ? 1.0f : 0.0f;
        const float down = keyState.Down ? 1.0f : 0.0f;

        return std::clamp( leftY + s - w + down - up, -1.0f, 1.0f );
    } );

    Input::mapAxis( "camera_right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();  // NOLINT(clang-diagnostic-shadow)

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a     = keyState.A ? 1.0f : 0.0f;
        const float d     = keyState.D ? 1.0f : 0.0f;
        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d - left + right, -1.0f, 1.0f );
    } );

    Input::mapAxis( "camera_up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftShoulder  = 0.0f;
        float rightShoulder = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();  // NOLINT(clang-diagnostic-shadow)

            leftShoulder += state.buttons.leftShoulder ? 1.0f : 0.0f;
            rightShoulder += state.buttons.rightShoulder ? 1.0f : 0.0f;
        }

        const auto keyState = keyboardState.getLastState();

        const float q = keyState.Q ? 1.0f : 0.0f;
        const float e = keyState.E ? 1.0f : 0.0f;

        return std::clamp( rightShoulder - leftShoulder + e - q, -1.0f, 1.0f );
    } );

    Input::mapAxis( "camera_pitch", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float pitch = 0.0f;

        const auto keyState = keyboardState.getLastState();

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();  // NOLINT(clang-diagnostic-shadow)

            pitch -= state.thumbSticks.rightY * 15.0f;
        }

        if ( !keyState.AltKey && mouseState.rightButton == ButtonState::Held )
        {
            pitch -= static_cast<float>( mouseState.y );
        }
        else if ( keyState.AltKey && mouseState.leftButton == ButtonState::Held )
        {
            pitch += static_cast<float>( mouseState.y );
        }

        return pitch;
    } );

    Input::mapAxis( "camera_yaw", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float yaw = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();  // NOLINT(clang-diagnostic-shadow)

            yaw -= state.thumbSticks.rightX * 15.0f;
        }

        const auto keyState = keyboardState.getLastState();
        if ( !keyState.AltKey && mouseState.rightButton == ButtonState::Held )
        {
            yaw -= static_cast<float>( mouseState.x );
        }
        else if ( keyState.AltKey && mouseState.leftButton == ButtonState::Held )
        {
            yaw += static_cast<float>( mouseState.x );
        }

        return yaw;
    } );

    reset();
}

void CameraController::setPerspective( float fovy, float aspect, float near, float far )
{
    camera.setProjection( glm::radians( fovy ), aspect, near, far );
}

void CameraController::reset()
{
    state = initialState;

    camera.setTranslation( state.pos );
    camera.setRotation( glm::quat { glm::vec3 { glm::radians( state.pitch ), glm::radians( state.yaw ), 0.0f } } );
}

void CameraController::update( float deltaTime )
{
    // Update rotation.
    state.pitch += Input::getAxis( "camera_pitch" ) * rotationSpeed * deltaTime;
    state.yaw += Input::getAxis( "camera_yaw" ) * rotationSpeed * deltaTime;

    // Clamp pitch:
    state.pitch = std::clamp( state.pitch, -90.0f, 90.0f );

    camera.setRotation( glm::quat { glm::vec3 { glm::radians( state.pitch ), glm::radians( state.yaw ), 0.0f } } );

    // Update position.
    const auto forward = Input::getAxis( "camera_forward" );
    const auto right   = Input::getAxis( "camera_right" );
    const auto up      = Input::getAxis( "camera_up" );

    const glm::vec3 deltaPos = glm::vec3 { right, up, forward } * movementSpeed * deltaTime;

    camera.translate( deltaPos );
}

const Camera& CameraController::getCamera() const noexcept
{
    return camera;
}

const glm::mat4& CameraController::getViewMatrix() const noexcept
{
    return camera.getViewMatrix();
}

const glm::mat4& CameraController::getProjectionMatrix() const noexcept
{
    return camera.getProjectionMatrix();
}

const glm::mat4& CameraController::getViewProjectionMatrix() const noexcept
{
    return camera.getViewProjectionMatrix();
}
