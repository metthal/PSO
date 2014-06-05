#ifndef SWARM_H
#define SWARM_H

#include <iostream>
#include <functional>
#include "Particle.h"
#include "Vector.h"
#include <SFML/Graphics.hpp>
#include "Drawable.h"

const double DefaultInertiaCoeff = 0.7;
const double DefaultCognitiveCoeff = 2.05;
const double DefaultSocialCoeff = 2.05;

template <typename CoordSystem> using FitnessFuncType = double (*)(const CoordSystem&, const CoordSystem&);

template <typename CoordSystem, typename VelocityType = CoordSystem, FitnessFuncType<CoordSystem> fitnessFn = CoordSystem::DistanceBetween> class Swarm : public Drawable
{
    typedef std::vector<Particle<CoordSystem, VelocityType>*> ParticleList;

public:
    Swarm() = delete;
    Swarm(const Swarm&) = delete;

    Swarm(const CoordSystem& maxCoords, const VelocityType& maxVelocity) : m_inertiaCoeff(DefaultInertiaCoeff), m_cognitiveCoeff(DefaultCognitiveCoeff), m_socialCoeff(DefaultSocialCoeff),
                                                                           m_maxCoords(maxCoords), m_maxVelocity(maxVelocity)
    {
    }

    ~Swarm()
    {
        for (Particle<CoordSystem, VelocityType>* particle : m_particles)
            delete particle;
    }

    void AddParticle()
    {
        m_particles.push_back(Particle<CoordSystem, VelocityType>::NewRandom(m_maxCoords, m_maxVelocity));
    }

	void AddParticle(const CoordSystem& position)
	{
		m_particles.push_back(Particle<CoordSystem, VelocityType>::New(position, m_maxVelocity));
	}

    void setInertiaCoeff(double inertia)
    {
        m_inertiaCoeff = inertia;
    }

    void setCognitiveCoeff(double cognition)
    {
        m_cognitiveCoeff = cognition;
    }

	void setPointOfInterest(const CoordSystem& poi)
	{
		m_poi = poi;
	}

    void setSocialCoeff(double social)
    {
        m_socialCoeff = social;
    }

    double getInertiaCoeff() const
    {
        return m_inertiaCoeff;
    }

    double getCognitiveCoeff() const
    {
        return m_cognitiveCoeff;
    }

    double getSocialCoeff() const
    {
        return m_socialCoeff;
    }

	const CoordSystem& getPointOfInterest() const
	{
		return m_poi;
	}

	void Prepare()
	{
		if (m_particles.empty())
			throw 1;

		// find the best position in the swarm
		m_bestPos = m_particles[0]->GetNextPosition();
		for (Particle<CoordSystem, VelocityType>* particle : m_particles)
		{
			if (fitnessFn(m_bestPos, m_poi) > fitnessFn(particle->GetNextPosition(), m_poi))
				m_bestPos = particle->GetNextPosition();
		}
	}

    void Recalculate()
    {
		static std::mt19937 randGen(static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));
		for (Particle<CoordSystem, VelocityType>* particle : m_particles)
		{
			double cognitiveMod = (randGen() % 100) / 100.0;
			double socialMod = (randGen() % 100) / 100.0;

			const CoordSystem& pos = particle->GetNextPosition();
			const CoordSystem& bestPos = particle->GetBestPosition();
			const VelocityType& vel = particle->GetVelocity();

			VelocityType newVel = vel * m_inertiaCoeff + (bestPos - pos) * m_cognitiveCoeff * cognitiveMod + (m_bestPos - pos) * m_socialCoeff * socialMod;
			CoordSystem newPos = pos + newVel;

			if (fitnessFn(pos, m_poi) > fitnessFn(newPos, m_poi))
				particle->SetBestPosition(newPos);

			particle->SetNextPosition(newPos);
			particle->SetVelocity(newVel);
		}

		for (Particle<CoordSystem, VelocityType>* particle : m_particles)
		{
			if (fitnessFn(m_bestPos, m_poi) > fitnessFn(particle->GetNextPosition(), m_poi))
				m_bestPos = particle->GetNextPosition();
		}
    }

	void Draw(sf::RenderWindow& window)
	{
		sf::CircleShape poi(6, 100);
		poi.setFillColor(sf::Color::Green);
		poi.setOutlineColor(sf::Color::Black);
		poi.setOutlineThickness(3.0f);
		poi.setPosition(sf::Vector2f(m_poi[0] - 6.0f, m_poi[1] - 6.0f));
		window.draw(poi);

		for (Particle<CoordSystem, VelocityType>* particle : m_particles)
			particle->Draw(window);
	}

	void Update(uint32_t roundTime)
	{
		// interpolate velocity and position
		double interpolationMod = (double)(roundTime / 1000.0);
		for (Particle<CoordSystem, VelocityType>* particle : m_particles)
		{
			CoordSystem prevPos = particle->GetPrevPosition();
			CoordSystem nextPos = particle->GetNextPosition();
			particle->SetRealPosition(prevPos + ((nextPos - prevPos) * interpolationMod));
		}
	}

    friend std::ostream& operator<<(std::ostream& ostream, const Swarm<CoordSystem, VelocityType>& swarm)
    {
        ostream << "SWARM (" << swarm.m_particles.size() << " particles)" << std::endl;
		ostream << "POI: " << swarm.m_poi << std::endl;
		ostream << "BEST: " << swarm.m_bestPos << std::endl;
        for (uint32_t i = 0; i < swarm.m_particles.size(); ++i)
        {
            Particle<CoordSystem, VelocityType>* particle = swarm.m_particles[i];
            ostream << i << " " << *particle;
        }

        return ostream;
    }

private:
    Swarm& operator =(const Swarm&);

    double m_inertiaCoeff;
    double m_cognitiveCoeff;
    double m_socialCoeff;
    ParticleList m_particles;
    CoordSystem m_maxCoords;
    CoordSystem m_bestPos;
	CoordSystem m_poi;
    VelocityType m_maxVelocity;
};

#endif
