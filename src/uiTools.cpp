#include <iostream>
#include "uiTools.h"

namespace myUiTools {
	sf::Vector2f cornerTypeToVector(cornerType corner, sf::Vector2f bounds)
	{
		switch (corner) {
		case TOP_RIGHT:
			return { bounds.x, 0 };
			break;
		case TOP_LEFT:
			return { 0, 0 };
			break;
		case BOTTOM_LEFT:
			return { 0, bounds.y };
			break;
		case BOTTOM_RIGHT:
			return { bounds.x, bounds.y };
			break;
		default:
			std::cout << "ERROR: Passed invalid corner type (myUiTools::cornerTypeToVector())\n";
			return sf::Vector2f();
			break;
		}
	}

	void setObjectOrigin(sf::Text& obj, const cornerType originPoint) {
		sf::FloatRect objBounds = obj.getLocalBounds(); // get object bounds

		// necessary for text objs because of built in padding
		sf::Vector2f offset = { objBounds.left, objBounds.top };

		sf::Vector2f newOrigin = cornerTypeToVector(originPoint, { objBounds.width, objBounds.height }) + offset;
		obj.setOrigin(newOrigin);
	}

	void drawOutline(sf::RenderWindow& win, const sf::Text& objToDraw, sf::Color color) {
		sf::FloatRect bounds = objToDraw.getLocalBounds();
		sf::FloatRect globalBounds = objToDraw.getGlobalBounds();

		int height = static_cast<float>(objToDraw.getCharacterSize());
		sf::RectangleShape outline(sf::Vector2f(bounds.width, height));
		outline.setOrigin(bounds.left, bounds.top);
		outline.setPosition(sf::Vector2f(globalBounds.left, globalBounds.top));
		outline.setOutlineColor(color);
		outline.setOutlineThickness(1);
		outline.setFillColor(sf::Color::Transparent);

		win.draw(outline);
	}
}