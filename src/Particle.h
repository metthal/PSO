#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"
#include "Drawable.h"

template <typename CoordSystem, typename VelocityType = CoordSystem> class Particle : public Drawable
{
public:
    ~Particle() {}

	static Particle<CoordSystem, VelocityType>* New(const CoordSystem& position, const VelocityType& maxVelocity)
	{
		Particle<CoordSystem, VelocityType>* particle = new Particle<CoordSystem, VelocityType>;
		particle->m_nextPos = position;
		particle->m_realPos = particle->m_nextPos;
		particle->m_bestPos = particle->m_nextPos;
		particle->m_vel = VelocityType::Random(-maxVelocity, maxVelocity);
		return particle;
	}

    static Particle<CoordSystem, VelocityType>* NewRandom(const CoordSystem& maxCoords, const VelocityType& maxVelocity)
    {
		return Particle<CoordSystem, VelocityType>::New(CoordSystem::Random(CoordSystem(), maxCoords), maxVelocity);
    }

    const CoordSystem& GetPrevPosition() const
    {
        return m_prevPos;
    }

	const CoordSystem& GetRealPosition() const
	{
		return m_realPos;
	}

	const CoordSystem& GetNextPosition() const
	{
		return m_nextPos;
	}

    const CoordSystem& GetBestPosition() const
    {
        return m_bestPos;
    }

    const VelocityType& GetVelocity() const
    {
        return m_vel;
    }

	void SetNextPosition(const CoordSystem& nextPos)
	{
		m_prevPos = m_nextPos;
		m_nextPos = nextPos;
	}

	void SetRealPosition(const CoordSystem& realPos)
	{
		m_realPos = realPos;
	}

	void SetBestPosition(const CoordSystem& bestPos)
	{
		m_bestPos = bestPos;
	}

	void SetVelocity(const VelocityType& vel)
	{
		m_vel = vel;
	}

    friend std::ostream& operator<<(std::ostream& ostream, const Particle<CoordSystem, VelocityType>& particle)
    {
		ostream << "p" << particle.m_realPos << " b" << particle.m_bestPos << " v" << particle.m_realVel << std::endl;
        return ostream;
    }

	void Draw(sf::RenderWindow& window)
	{
		sf::CircleShape shape(3, 100);
		shape.setPosition(sf::Vector2f(GetRealPosition()[0] - 3.0f, GetRealPosition()[1] - 3.0f));
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(2.0f);
		window.draw(shape);

		sf::Vertex velVector[] =
		{
			sf::Vertex(sf::Vector2f(GetRealPosition()[0], GetRealPosition()[1])),
			sf::Vertex(sf::Vector2f(GetRealPosition()[0] + GetVelocity()[0], GetRealPosition()[1] + GetVelocity()[1]))
		};
		velVector[0].color = sf::Color::Red;
		velVector[1].color = sf::Color::Red;
		window.draw(velVector, 2, sf::Lines);
	}

private:
    Particle() : m_prevPos(), m_realPos(), m_nextPos(), m_bestPos(), m_vel() {}

    Particle(const Particle&) = delete;
    Particle& operator =(const Particle&);

    CoordSystem m_prevPos, m_realPos, m_nextPos;
    CoordSystem m_bestPos;
    VelocityType m_vel;
};

#endif
