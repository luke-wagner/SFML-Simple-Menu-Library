/** --------------------------------------------------------
* @file    menu.cpp
* @author  Luke Wagner
* @version 1.0
*
* SFML Menu Class
*   Contains a menu class implementation for use with SFML
*
* ------------------------------------------------------------ */

#include <iostream>
#include "menu.h"
#include "uiTools.h"

Menu::Menu() {
	// Set default values
	setType(DYNAMIC);

	// formatting
	setDockingPosition(DEFAULT_DOCKING_POSITION);
	setTextOriginPoint(DEFAULT_TEXT_ORIGIN_POINT);
	setBounds(0, 0);
	setPadding(0, 0);
	componentBuffer = DEFAULT_COMPONENT_BUFFER;
	numElements = 0;
	outlinePosSet = false;

	// objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		textObjs[i] = NULL;
	}

	defaultFontName = BACKUP_FONT_NAME;
	backupFontObj = new sf::Font;
	fonts.insert(std::make_pair(BACKUP_FONT_NAME, backupFontObj));

	if (!backupFontObj->loadFromFile(BACKUP_FONT_NAME)) {
		std::cout << "ERROR: Error loading default Menu font\n";
	}

	defaultTextObj.setCharacterSize(20); // #check - make consts?
	defaultTextObj.setFont(*backupFontObj);

	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(1);
	outline.setOutlineColor(sf::Color::White);

	// miscellaneous
	menuShown = false;
	showComponentOutlines = false;
	showMenuBounds = false;
	backgroundColor = sf::Color::Transparent;
}


void Menu::setType(menuType type) {
	this->type = type;
}

bool Menu::setDockingPosition(cornerType corner)
{
	switch (corner) {
	case TOP_RIGHT:
		dockingPosition = TOP_RIGHT;
		break;
	case TOP_LEFT:
		dockingPosition = TOP_LEFT;
		break;
	case BOTTOM_LEFT:
		dockingPosition = BOTTOM_LEFT;
		break;
	case BOTTOM_RIGHT:
		dockingPosition = BOTTOM_RIGHT;
		break;
	default:
		outputInvalidCornerTypeMessage(__func__);
		return false;
	}

	if (type == DYNAMIC) {
		setTextOriginPoint(corner);
	}

	outlinePosSet = false;	//next draw call must change outline position
	return true;
}

bool Menu::setTextOriginPoint(cornerType corner)
{
	switch (corner) {
	case TOP_RIGHT:
		textOriginPoint = TOP_RIGHT;
		break;
	case TOP_LEFT:
		textOriginPoint = TOP_LEFT;
		break;
	case BOTTOM_LEFT:
		textOriginPoint = BOTTOM_LEFT;
		break;
	case BOTTOM_RIGHT:
		textOriginPoint = BOTTOM_RIGHT;
		break;
	default:
		outputInvalidCornerTypeMessage(__func__);
		return false;
	}

	return true;
}

bool Menu::setPadding(float x, float y) {
	// data validation
	if (x >= 0 && y >= 0) {
		float prevPaddingX = paddingX;
		float prevPaddingY = paddingY;
		paddingX = x;
		paddingY = y;

		float diffPaddingX = paddingX - prevPaddingX;
		float diffPaddingY = paddingY - prevPaddingY;

		if (type == DYNAMIC) {
			setBounds(bounds.x + diffPaddingX * 2, bounds.y + diffPaddingY * 2);
		}

		if (numElements > 0) {
			// reformat existing elements
			applyNewPadding(diffPaddingX, diffPaddingY);
		}

		return true;
	} else {
		return false;
	}
}

void Menu::printSomething()
{
	std::cout << "This is a line of text";
}

sf::Text* Menu::addMenuItem(sf::RenderWindow& win, std::string text) {
	return menuAdditionOperations(win, text, defaultTextObj);
}

sf::Text* Menu::addMenuItem(sf::RenderWindow& win, std::string text, const sf::Text& textObj) {
	return menuAdditionOperations(win, text, textObj);
	/*
	// loop over textObjs
	bool foundPlace = false;
	int index = 0;

	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int x = 0; x < len; x++) {
		if (!foundPlace) {
			if (textObjs[x] == NULL) {
				// add object and save its index
				textObjs[x] = new sf::Text(textObj);
				foundPlace = true;
				index = x;
			}
		} else {
			break;
		}
	}

	if (foundPlace) {
		sf::Text* addedItem = textObjs[index];
		addedItem->setString(text);

		// alignment
		myUiTools::setObjectOrigin(*addedItem, textOriginPoint); // must be done after string property is set

		if (dockingPosition == TOP_LEFT || dockingPosition == TOP_RIGHT) {
			// set new obj position relative to dockingPosition and other elements
			sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);
			sf::Vector2f objDrawPosition = { innerCorner.x, innerCorner.y + (textObj.getCharacterSize() + componentBuffer) * numElements };
			addedItem->setPosition(objDrawPosition);
		} else {
			// set new element at same spot every time and move previous elements up by one "space"
			sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);
			addedItem->setPosition(innerCorner);

			// move previous elements up
			for (int i = index - 1; i >= 0; i--) {
				float newY = textObjs[i + 1]->getPosition().y - textObjs[i + 1]->getCharacterSize() - componentBuffer;
				textObjs[i]->setPosition(textObjs[i]->getPosition().x, newY);
			}
		}

		// if new element's width is greater than the bounds of the menu, resize the bounds
		if (addedItem->getLocalBounds().width + paddingX > bounds.x)
			setBounds(addedItem->getLocalBounds().width + paddingX * 2, bounds.y + textObj.getCharacterSize() + componentBuffer);
		else
			setBounds(bounds.x, bounds.y += textObj.getCharacterSize() + componentBuffer);

		numElements++;

		return addedItem;
	} else {
		std::cout << "ERROR: Not able to add menu item. Maximum elements already added.\n";
		return nullptr;
	}
	*/
}

/*
sf::Text* Menu::addMenuItem(sf::RenderWindow& win, std::string text, const sf::Text& textObj) {
	// loop over textObjs
	bool foundPlace = false;
	int index = 0;

	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int x = 0; x < len; x++) {
		if (!foundPlace) {
			if (textObjs[x] == NULL) {
				// add object and save its index
				textObjs[x] = new sf::Text;
				foundPlace = true;
				index = x;
			}
		} else {
			break;
		}
	}

	sf::Text* addedItem = textObjs[index];

	if (foundPlace) {
		// Set text object properties
		addedItem->setCharacterSize(textTemplate.textHeight);

		// look for font object
		// #check - only stores default font at the moment

		auto it = fonts.find(textTemplate.fontName);

		if (it != fonts.end()) {
			// found font
			addedItem->setFont(*(it->second));
		} else {
			// try to create font obj
			sf::Font* f = new sf::Font;
			if (!f->loadFromFile(textTemplate.fontName)) {
				std::cout << "ERROR: Error loading font! (Menu::addMenuItem()). Using backup font\n";
				auto it = fonts.find(BACKUP_FONT_NAME);
				addedItem->setFont(*(it->second));
			} else {
				auto i = fonts.insert(std::make_pair(textTemplate.fontName, f));
				addedItem->setFont(*f);
			}
		}

		addedItem->setString(text);

		myUiTools::setObjectOrigin(*addedItem, textOriginPoint); // must be done after string property is set

		if (dockingPosition == TOP_LEFT || dockingPosition == TOP_RIGHT) {
			// set new obj position relative to dockingPosition and other elements
			sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);
			sf::Vector2f objDrawPosition = { innerCorner.x, innerCorner.y + (textTemplate.textHeight + componentBuffer) * numElements };
			addedItem->setPosition(objDrawPosition);
		} else {
			// set new element at same spot every time and move previous elements up by one "space"
			sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);
			addedItem->setPosition(innerCorner);

			// move previous elements up
			for (int i = index - 1; i >= 0; i--) {
				std::string contents = textObjs[i]->getString();
				float newY = textObjs[i + 1]->getPosition().y - textObjs[i + 1]->getCharacterSize() - componentBuffer;
				textObjs[i]->setPosition(textObjs[i]->getPosition().x, newY);
			}
		}

		// if new element's width is greater than the bounds of the menu, resize the bounds
		if (addedItem->getLocalBounds().width + paddingX > bounds.x)
			setBounds(addedItem->getLocalBounds().width + paddingX * 2, bounds.y + textTemplate.textHeight + componentBuffer);
		else
			setBounds(bounds.x, bounds.y += textTemplate.textHeight + componentBuffer);

		numElements++;

		return addedItem;
	} else {
		std::cout << "ERROR: Not able to add menu item. Maximum elements already added.\n";
		return nullptr;
	}
}
*/

void Menu::draw(sf::RenderWindow& win)
{
	// text objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			// Reformat any elements that would be out of bounds
			// check first
			if (dockingPosition == TOP_RIGHT || dockingPosition == BOTTOM_RIGHT) {
				sf::FloatRect localBounds = textObjs[i]->getLocalBounds();
				if (localBounds.left + localBounds.width != textObjs[i]->getOrigin().x) {
					// object's horizontal size has changed, must realigin in menu
					// reset object origin
					if (textOriginPoint == TOP_RIGHT)
						myUiTools::setObjectOrigin(*textObjs[i], TOP_RIGHT);
					else
						myUiTools::setObjectOrigin(*textObjs[i], BOTTOM_RIGHT);
					sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
					sf::Vector2f innerCorner = getInnerCorner(outerCorner);
					textObjs[i]->setPosition(innerCorner.x, textObjs[i]->getPosition().y);
				}
			}

			sf::Text t = *textObjs[i];
			win.draw(t);
			if (showComponentOutlines)
				myUiTools::drawOutline(win, t, sf::Color::White);
			if (showMenuBounds) {
				if (!outlinePosSet)
					outline.setPosition(myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) }));
				win.draw(outline);
			}
		}
	}
}

void Menu::toggleShown() {
	if (!menuShown) {
		menuShown = true;
	} else {
		menuShown = false;
	}
}

sf::Vector2f Menu::getInnerCorner(sf::Vector2f outerCorner) {
	switch (dockingPosition) {
	case TOP_RIGHT:
		return { outerCorner.x - paddingX, outerCorner.y + paddingY };
	case TOP_LEFT:
		return { outerCorner.x + paddingX, outerCorner.y + paddingY };
	case BOTTOM_LEFT:
		return { outerCorner.x + paddingX, outerCorner.y - paddingY };
	case BOTTOM_RIGHT:
		return { outerCorner.x - paddingX, outerCorner.y - paddingY };
	default:
		outputInvalidCornerTypeMessage(__func__);
	}

	return sf::Vector2f();
}

void Menu::setBounds(float x, float y) {
	// data validation? #check
	bounds.x = x;
	bounds.y = y;

	// update outline object
	outline.setSize(bounds);
	outline.setOrigin(myUiTools::cornerTypeToVector(dockingPosition, bounds));
}

void Menu::applyNewPadding(float diffPaddingX, float diffPaddingY) {
	// text objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			sf::Vector2f objPosition = textObjs[i]->getPosition();
			switch (dockingPosition) {
			case TOP_RIGHT:
				textObjs[i]->setPosition(objPosition.x - diffPaddingX, objPosition.y + diffPaddingY);
				objPosition = textObjs[i]->getPosition();
				break;
			case TOP_LEFT:
				textObjs[i]->setPosition(objPosition.x + diffPaddingX, objPosition.y + diffPaddingY);
				break;
			case BOTTOM_LEFT:
				textObjs[i]->setPosition(objPosition.x + diffPaddingX, objPosition.y - diffPaddingY);
				break;
			case BOTTOM_RIGHT:
				textObjs[i]->setPosition(objPosition.x - diffPaddingX, objPosition.y - diffPaddingY);
				break;
			default:
				outputInvalidCornerTypeMessage(__func__);
			}
		}
	}
}

sf::Text* Menu::menuAdditionOperations(sf::RenderWindow& win, std::string text, const sf::Text& objToUse) {
	// loop over textObjs
	bool foundPlace = false;
	int index = 0;

	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int x = 0; x < len; x++) {
		if (!foundPlace) {
			if (textObjs[x] == NULL) {
				// add object and save its index
				textObjs[x] = new sf::Text(objToUse);
				foundPlace = true;
				index = x;
			}
		} else {
			break;
		}
	}

	if (foundPlace) {
		sf::Text* addedItem = textObjs[index];
		addedItem->setString(text);

		// get useful properties
		int addedItemHeight = addedItem->getCharacterSize();

		// alignment
		myUiTools::setObjectOrigin(*addedItem, textOriginPoint); // must be done after string property is set

		if (dockingPosition == TOP_LEFT || dockingPosition == TOP_RIGHT) {
			// set new obj position relative to dockingPosition and other elements
			sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);

			sf::Text* lastElementAdded = getLast();
			sf::Vector2f objDrawPosition;
			if (lastElementAdded != NULL) {
				objDrawPosition = { innerCorner.x, lastElementAdded->getPosition().y + lastElementAdded->getCharacterSize() + componentBuffer };
			} else {
				objDrawPosition = { innerCorner.x, innerCorner.y };
			}
			addedItem->setPosition(objDrawPosition);
		} else {
			// set new element at same spot every time and move previous elements up by one "space"
			sf::Vector2f outerCorner = myUiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);
			addedItem->setPosition(innerCorner);

			// move previous elements up
			for (int i = index - 1; i >= 0; i--) {
				std::string s = textObjs[i]->getString();
				float newY = textObjs[i + 1]->getPosition().y - textObjs[i + 1]->getCharacterSize() - componentBuffer;
				textObjs[i]->setPosition(textObjs[i]->getPosition().x, newY);
			}
		}

		// if new element's width is greater than the bounds of the menu, resize the bounds
		sf::FloatRect addedItemBounds = addedItem->getLocalBounds();
		if (addedItemBounds.width + paddingX > bounds.x)
			setBounds(addedItemBounds.width + paddingX * 2, bounds.y + addedItemHeight + componentBuffer);
		else
			setBounds(bounds.x, bounds.y += addedItemHeight + componentBuffer);

		numElements++;

		return addedItem;
	} else {
		std::cout << "ERROR: Not able to add menu item. Maximum elements already added.\n";
		return nullptr;
	}
}

sf::Text* Menu::getLast() {
	// can be optimized? - #check

	// stepwise solution
	int counter = 0;
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			counter++;
		} else {
			break;
		}
	}

	if (counter != 1) {
		return textObjs[counter - 2];
	} else {
		return nullptr;
	}
}

void Menu::outputInvalidCornerTypeMessage(std::string functionName) {
	std::cout << "ERROR: Passed invalid corner type ("
		<< functionName << "())" << std::endl;
}