#include <iostream>
#include "Swarm.h"
#include <SFML/System.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Particle Swarm Optimization");

    Vector2f maxCoords(800.0f, 600.0f);
    Vector2f maxVelo(1.0f, 1.0f);
    Swarm<Vector2f, Vector2f> swarm(maxCoords, maxVelo);
	swarm.setPointOfInterest(Vector2f::Random(Vector2f(10.0f, 10.0f), Vector2f(790.0f, 590.0f)));

	for (uint8_t i = 0; i < 25; ++i)
		swarm.AddParticle();

	swarm.Prepare();
	swarm.Recalculate();

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return 0;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					swarm.setPointOfInterest(Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y));
			}
		}

		window.clear(sf::Color(81, 151, 255));

		uint32_t timeElapsed = clock.getElapsedTime().asMilliseconds();
		swarm.Update(timeElapsed);
		swarm.Draw(window);
		if (timeElapsed >= 1000)
		{
			swarm.Recalculate();
			clock.restart();
		}

		//std::cout << swarm << std::endl;
		window.display();

		sf::sleep(sf::milliseconds(16));
	}
}
