/** --------------------------------------------------------
* @file    roundedRect.h
* @author  Luke Wagner
* @version 1.0
*
* Definition for createRoundedRect function
*   Contains definition for createRoundedRect(), which provides a way to easily create rounded rectangles for SFML
*	Returns the roundedRect as an sf::ConvexShape
*
* ------------------------------------------------------------ */
#pragma once

#include <SFML/Graphics.hpp>

sf::ConvexShape createRoundedRect(const sf::Vector2f& size, const sf::Color& fillColor = sf::Color::White, float outlineThickness = 0, const sf::Color& outlineColor = sf::Color::Transparent, float bevelAmount = 0.2, unsigned int pointsPerCorner = 5);