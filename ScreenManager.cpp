#include <SFML/Graphics.hpp>
#include "ScreenManager.h"

void ScreenManager::initScreen() {
	font.loadFromFile("arial.ttf");
}
void ScreenManager::updateScreen(vector<std::shared_ptr<Particle>> _particles) {
	for (shared_ptr<Particle> p : _particles) {
		sf::CircleShape circle(getScreenLength(p->radius));
		sf::Color color = p->color;
		if (p->color == sf::Color::Magenta) {
			std::cout << "Magenta"<< std::endl;
		}
		circle.setFillColor(color);

		// Set position with spacing between circles
		vector<double> pos = getScreenCoordinates(*p);
		circle.setPosition(pos[0], pos[1]);

		window->draw(circle);
	}
}

vector<double> ScreenManager::getScreenCoordinates(Particle p) {
	return { (SCR_WIDTH / 4.0) + (p.position[0] * zoomScale) , (3.0 * SCR_HEIGHT / 4.0) - (p.position[1] * zoomScale) };
}

double ScreenManager::getScreenLength(double len) {
	return len * zoomScale;
}

Vector2r ScreenManager::getAnalyticCoordinates(sf::Vector2i scrcoor) {
	Real x = (scrcoor.x - (SCR_WIDTH / 4.0)) / zoomScale;
	Real y = ((3 * SCR_HEIGHT / 4.0) - scrcoor.y) / zoomScale;
	return {x, y};
}

void ScreenManager::printToHeader(std::string message) {
	sf::Text text;
	text.setFont(font);
	text.setString(message);
	text.setCharacterSize(25); // Font size in pixels
	text.setFillColor(sf::Color::White); // Text color
	text.setPosition(20, 20); // Set position on screen
	window->draw(text);
}