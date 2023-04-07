#include "ListenerImpl.hpp"
#include <Audio/Listener.hpp>

using namespace Audio;

Listener::Listener( std::shared_ptr<ListenerImpl> impl )
: impl { std::move( impl ) }
{}

Listener::Listener()                                 = default;
Listener::Listener( const Listener& )                = default;
Listener::Listener( Listener&& ) noexcept            = default;
Listener& Listener::operator=( const Listener& )     = default;
Listener& Listener::operator=( Listener&& ) noexcept = default;

void Listener::reset()
{
    impl.reset();
}

Listener& Listener::operator=( nullptr_t ) noexcept
{
    impl = nullptr;
    return *this;
}

bool Listener::operator==( nullptr_t ) const noexcept
{
    return impl == nullptr;
}

bool Listener::operator!=( nullptr_t ) const noexcept
{
    return impl != nullptr;
}

void Listener::setPosition( const glm::vec3& pos )
{
    impl->setPosition( pos );
}

glm::vec3 Listener::getPosition() const
{
    return impl->getPosition();
}

void Listener::setDirection( const glm::vec3& dir )
{
    impl->setDirection( dir );
}

glm::vec3 Listener::getDirection() const
{
    return impl->getDirection();
}

void Listener::setUp( const glm::vec3& up )
{
    impl->setUp( up );
}

glm::vec3 Listener::getUp() const
{
    return impl->getUp();
}

void Listener::setCone( float innerConeAngle, float outerConeAngle, float outerGain )
{
    impl->setCone( innerConeAngle, outerConeAngle, outerGain );
}

void Listener::getCone( float& innerConeAngle, float& outerConeAngle, float& outerGain ) const
{
    impl->getCone( innerConeAngle, outerConeAngle, outerGain );
}

Listener::operator bool() const noexcept
{
    return impl != nullptr;
}

std::shared_ptr<ListenerImpl> Listener::get() const noexcept
{
    return impl;
}
