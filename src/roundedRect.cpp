/** --------------------------------------------------------
* @file    roundedRect.h
* @author  Luke Wagner
* @version 1.0
*
* Implementation for createRoundedRect function
*   Contains implementation for createRoundedRect(), which provides a way to easily create rounded rectangles for SFML.
*	Implementaiton written by ChatGPT
*
* ------------------------------------------------------------ */
#include "roundedRect.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>

sf::ConvexShape createRoundedRect(const sf::Vector2f& size, const sf::Color& fillColor, float outlineThickness, const sf::Color& outlineColor, float bevelAmount, unsigned int pointsPerCorner) {
	// Calculate the radius for each corner based on the bevelAmount ratio and rectangle height
	float radius = size.y * bevelAmount;

	// Define the total number of points for the rounded rectangle
	const unsigned int totalPoints = 4 * pointsPerCorner;

	// Configure the ConvexShape object
	sf::ConvexShape roundedRect;
	roundedRect.setPointCount(totalPoints);
	roundedRect.setFillColor(fillColor);
	roundedRect.setOutlineThickness(outlineThickness);
	roundedRect.setOutlineColor(outlineColor);

	// Iterate through each corner
	for (unsigned int i = 0; i < 4; ++i) {
		// Determine the center of the arc for the current corner
		float centerX = (i % 3 == 0) ? (size.x - radius) : radius;
		float centerY = (i < 2) ? (size.y - radius) : radius;

		// Calculate the starting angle for the current corner
		float angleStart = (i * M_PI) / 2;

		// Iterate through each point in the current corner
		for (unsigned int j = 0; j < pointsPerCorner; ++j) {
			// Calculate the angle of the current point
			float angle = angleStart + (j * M_PI) / (2 * (pointsPerCorner - 1));

			// Calculate the x and y coordinates of the current point
			float pointX = centerX + radius * std::cos(angle);
			float pointY = centerY + radius * std::sin(angle);

			roundedRect.setPoint(i * pointsPerCorner + j, sf::Vector2f(pointX, pointY));
		}
	}

	return roundedRect;
}