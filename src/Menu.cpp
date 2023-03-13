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
#include "fonts.h"

Menu::Menu() {
	// Set default values
	setType(DYNAMIC);

	// formatting
	setDockingPosition(DEFAULT_DOCKING_POSITION);
	setTextOriginPoint(DEFAULT_TEXT_ORIGIN_POINT);
	setBounds(0, 0);
	setPadding(DEFAULT_PADDING.x, DEFAULT_PADDING.y);
	setComponentBuffer(DEFAULT_COMPONENT_BUFFER);
	numElements = 0;
	mustReformatElements = false;

	// objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		textObjs[i] = NULL;
	}

	backupFontObj = new sf::Font;
	fonts.insert(std::make_pair(BACKUP_FONT_NAME, backupFontObj));

	if (!backupFontObj->loadFromMemory(fonts::arial, fonts::arial_len)) {
		std::cout << "ERROR: Error loading default Menu font\n";
	}

	defaultTextObj.setCharacterSize(20); // #check - make consts?
	defaultTextObj.setFont(*backupFontObj);

	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(1);
	outline.setOutlineColor(sf::Color::White);

	// miscellaneous
	menuShown = true;
	componentOutlinesShown = false;
	menuBoundsShown = false;
	backgroundColor = sf::Color::Transparent;
}


void Menu::setType(menuType type) {
	this->type = type;
}

bool Menu::setDockingPosition(uiTools::cornerType corner)
{
	if (dockingPosition == corner) {
		// nothing changes
		return true; // #check
	}

	switch (corner) {
	case uiTools::TOP_RIGHT:
		dockingPosition = uiTools::TOP_RIGHT;
		break;
	case uiTools::TOP_LEFT:
		dockingPosition = uiTools::TOP_LEFT;
		break;
	case uiTools::BOTTOM_LEFT:
		dockingPosition = uiTools::BOTTOM_LEFT;
		break;
	case uiTools::BOTTOM_RIGHT:
		dockingPosition = uiTools::BOTTOM_RIGHT;
		break;
	}

	if (type == DYNAMIC) {
		setTextOriginPoint(corner);
	}

	if (numElements > 0) {
		mustReformatElements = true; // reformat elements on the next draw call
	}

	return true;
}

bool Menu::setTextOriginPoint(uiTools::cornerType corner)
{
	switch (corner) {
	case uiTools::TOP_RIGHT:
		textOriginPoint = uiTools::TOP_RIGHT;
		break;
	case uiTools::TOP_LEFT:
		textOriginPoint = uiTools::TOP_LEFT;
		break;
	case uiTools::BOTTOM_LEFT:
		textOriginPoint = uiTools::BOTTOM_LEFT;
		break;
	case uiTools::BOTTOM_RIGHT:
		textOriginPoint = uiTools::BOTTOM_RIGHT;
		break;
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
			applyPaddingDiff(diffPaddingX, diffPaddingY);
		}

		return true;
	} else {
		return false;
	}
}

bool Menu::setComponentBuffer(int newVal)
{
	// data validation
	if (newVal >= 0) {
		int oldVal = componentBuffer;
		componentBuffer = newVal;

		// move elements up/down
		if (newVal != oldVal && numElements > 0)
			applyCompBufferDiff(newVal - oldVal);

		return true;
	} else
		return false;
}

void Menu::showMenu()
{
	menuShown = true;
}

void Menu::hideMenu()
{
	menuShown = false;
}

void Menu::showMenuBounds()
{
	menuBoundsShown = true;
}

void Menu::hideMenuBounds()
{
	menuBoundsShown = false;
}

void Menu::toggleMenuBounds()
{
	if (menuBoundsShown) {
		menuBoundsShown = false;
	} else {
		menuBoundsShown = true;
	}
}

void Menu::showComponentOutlines()
{
	componentOutlinesShown = true;
}

void Menu::hideComponentOutlines()
{
	componentOutlinesShown = false;
}

void Menu::toggleComponentOutlines()
{
	if (componentOutlinesShown) {
		componentOutlinesShown = false;
	} else {
		componentOutlinesShown = true;
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
}

bool Menu::removeMenuItem(sf::Text* objToRemove)
{
	if (objToRemove == NULL) {
		// trying to remove an object that may have already been deleted
		return false;
	}

	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			if (textObjs[i] == objToRemove) {
				// adjust other menu items' positions
				if (dockingPosition == uiTools::TOP_RIGHT || dockingPosition == uiTools::TOP_LEFT) {
					for (int j = i + 1; j < len; j++) {
						if (textObjs[j] != NULL) {
							sf::Vector2f currentPos = textObjs[j]->getPosition();
							textObjs[j]->setPosition(currentPos.x, currentPos.y - textObjs[i]->getCharacterSize() - componentBuffer);
						} else {
							break;
						}
					}
				} else {
					for (int j = i - 1; j >= 0; j--) {
						if (textObjs[j] != NULL) {
							sf::Vector2f currentPos = textObjs[j]->getPosition();
							textObjs[j]->setPosition(currentPos.x, currentPos.y + textObjs[i]->getCharacterSize() + componentBuffer);
						} else {
							break;
						}
					}
				}
				
				// change menu bounds
				bool widest = false;
				if (getWidestItemIndex() == i) {
					// objToRemove is the widest item in the menu
					widest = true;
				}
				if (numElements > 1) {
					setBounds(bounds.x, bounds.y - textObjs[i]->getCharacterSize() - componentBuffer);
				} else {
					setBounds(paddingX * 2, paddingY * 2);
				}

				// delete object and reformat array
				delete textObjs[i];
				textObjs[i] = NULL;
				numElements--;
				reformatArray(textObjs, len);

				if (widest && textObjs[getWidestItemIndex()] != NULL) {
					float newWidth = textObjs[getWidestItemIndex()]->getLocalBounds().width;
					setBounds(newWidth + paddingX * 2, bounds.y);
				}

				return true;
			}
		} else {
			return false;
		}
	}
}

void Menu::draw(sf::RenderWindow& win)
{
	if (mustReformatElements) {
		reformatElements(win); // #check
		mustReformatElements = false;
	}

	// text objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			// Reformat any elements that would be out of bounds
			// check first
			if (dockingPosition == uiTools::TOP_RIGHT || dockingPosition == uiTools::BOTTOM_RIGHT) {
				sf::FloatRect localBounds = textObjs[i]->getLocalBounds();
				if (localBounds.left + localBounds.width != textObjs[i]->getOrigin().x) {
					// object's horizontal size has changed, must realigin in menu
					// reset object origin
					if (textOriginPoint == uiTools::TOP_RIGHT)
						uiTools::setObjectOrigin(*textObjs[i], uiTools::TOP_RIGHT);
					else
						uiTools::setObjectOrigin(*textObjs[i], uiTools::BOTTOM_RIGHT);

					sf::Vector2f outerCorner = uiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
					sf::Vector2f innerCorner = getInnerCorner(outerCorner);
					textObjs[i]->setPosition(innerCorner.x, textObjs[i]->getPosition().y);
				}
			}

			sf::Text t = *textObjs[i];
			if (menuShown) {
				win.draw(t);
				if (componentOutlinesShown)
					uiTools::drawOutline(win, t, sf::Color::White);
			}
		} else {
			break;
		}
	}

	if (menuShown && menuBoundsShown && 
		(type == STATIC || (type == DYNAMIC && bounds.y > paddingY * 2))) {
		sf::Vector2f windowBounds = { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) };
		if (outline.getPosition() != uiTools::cornerTypeToVector(dockingPosition, windowBounds)) {
			// dockingPosition has changed; must adjust outline position
			outline.setPosition(uiTools::cornerTypeToVector(dockingPosition, windowBounds));
		}
		win.draw(outline);
	}
}

void Menu::toggleMenuShown() {
	if (!menuShown) {
		menuShown = true;
	} else {
		menuShown = false;
	}
}

sf::Vector2f Menu::getInnerCorner(sf::Vector2f outerCorner) {
	switch (dockingPosition) {
	case uiTools::TOP_RIGHT:
		return { outerCorner.x - paddingX, outerCorner.y + paddingY };
	case uiTools::TOP_LEFT:
		return { outerCorner.x + paddingX, outerCorner.y + paddingY };
	case uiTools::BOTTOM_LEFT:
		return { outerCorner.x + paddingX, outerCorner.y - paddingY };
	case uiTools::BOTTOM_RIGHT:
		return { outerCorner.x - paddingX, outerCorner.y - paddingY };
	}

	return sf::Vector2f();
}

void Menu::setBounds(float x, float y) {
	// data validation? #check
	bounds.x = x;
	bounds.y = y;

	// update outline object
	outline.setSize(bounds);
	outline.setOrigin(uiTools::cornerTypeToVector(dockingPosition, bounds));
}

void Menu::applyPaddingDiff(float diffPaddingX, float diffPaddingY) {
	// text objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			sf::Vector2f objPosition = textObjs[i]->getPosition();
			switch (dockingPosition) {
			case uiTools::TOP_RIGHT:
				textObjs[i]->setPosition(objPosition.x - diffPaddingX, objPosition.y + diffPaddingY);
				objPosition = textObjs[i]->getPosition();
				break;
			case uiTools::TOP_LEFT:
				textObjs[i]->setPosition(objPosition.x + diffPaddingX, objPosition.y + diffPaddingY);
				break;
			case uiTools::BOTTOM_LEFT:
				textObjs[i]->setPosition(objPosition.x + diffPaddingX, objPosition.y - diffPaddingY);
				break;
			case uiTools::BOTTOM_RIGHT:
				textObjs[i]->setPosition(objPosition.x - diffPaddingX, objPosition.y - diffPaddingY);
				break;
			}
		}
	}
}

void Menu::applyCompBufferDiff(int diff)
{
	// text objects
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	int counter = 1;
	for (int i = 1; i < len; i++) {
		if (textObjs[i] != NULL) {
			counter++;
			sf::Vector2f currentPos = textObjs[i]->getPosition();

			if (dockingPosition == uiTools::TOP_LEFT || dockingPosition == uiTools::TOP_RIGHT) {
				textObjs[i]->setPosition(currentPos.x, currentPos.y + i * diff);
			}
		} else {
			if (dockingPosition == uiTools::BOTTOM_LEFT || dockingPosition == uiTools::BOTTOM_RIGHT) {
				for (int i = counter - 1; i >= 0; i--) {
					sf::Vector2f currentPos = textObjs[i]->getPosition();
					textObjs[i]->setPosition(currentPos.x, currentPos.y - (counter - i - 1) * diff);
				}
			}

			break;
		}
	}
}

void Menu::reformatElements(sf::RenderWindow& win)
{
	// Brute force method #improve
	// Remove each item, creating copy as we go
	// add each item back in using addMenuItem()

	// #improve - implement a removeMenuItem() function
	// temporary solution: reset menu properties before loop
	numElements = 0;
	setBounds(paddingX * 2, paddingY * 2); // not (0,0) because padding must be accounted for before adding items

	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			sf::Text copy = sf::Text(*textObjs[i]);
			delete textObjs[i];
			textObjs[i] = NULL;
			addMenuItem(win, copy.getString(), copy);
		} else {
			break;
		}
	}
}

void Menu::reformatArray(sf::Text* array[], int len)
{
	// expensive - #improve
	for (int i = 0; i < len; i++) {
		if (array[i] == NULL) {
			for (int j = i + 1; j < len; j++) {
				array[j - 1] = array[j];
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

		// Alignment
		uiTools::setObjectOrigin(*addedItem, textOriginPoint); // must be done after string property is set

		if (dockingPosition == uiTools::TOP_LEFT || dockingPosition == uiTools::TOP_RIGHT) {
			// set new obj position relative to dockingPosition and other elements
			sf::Vector2f outerCorner = uiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);

			sf::Text* lastElementAdded = NULL;
			if (index >= 1) {
				lastElementAdded = textObjs[index - 1];
			}
			sf::Vector2f objDrawPosition;
			if (lastElementAdded != NULL) {
				objDrawPosition = { innerCorner.x, lastElementAdded->getPosition().y + lastElementAdded->getCharacterSize() + componentBuffer };
			} else {
				objDrawPosition = { innerCorner.x, innerCorner.y };
			}
			addedItem->setPosition(objDrawPosition);
		} else {
			// set new element at same spot every time and move previous elements up by one "space"
			sf::Vector2f outerCorner = uiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
			sf::Vector2f innerCorner = getInnerCorner(outerCorner);
			addedItem->setPosition(innerCorner);

			// move previous elements up
			int counter = 0;
			for (int i = index - 1; i >= 0; i--) {
				sf::Text* lastElementAdded = textObjs[i + 1];
				float newY = lastElementAdded->getPosition().y - lastElementAdded->getCharacterSize() - componentBuffer;
				textObjs[i]->setPosition(textObjs[i]->getPosition().x, newY);
				counter++;
			}
		}

		// Resize menu bounds
		sf::FloatRect addedItemBounds = addedItem->getLocalBounds();
		float newBoundsX = bounds.x;
		float newBoundsY = 0;

		// if new element's width is greater than the bounds of the menu, change bounds.x
		if (addedItemBounds.width + paddingX * 2 > bounds.x) {
			newBoundsX = addedItemBounds.width + paddingX * 2;
		}
		// change bounds.y
		if (numElements == 0) {
			newBoundsY = bounds.y + addedItemHeight;
		} else {
			newBoundsY = bounds.y + addedItemHeight + componentBuffer;
		}
		setBounds(newBoundsX, newBoundsY);

		numElements++;
		return addedItem;
	} else {
		std::cout << "ERROR: Not able to add menu item. Maximum elements already added.\n";
		return nullptr;
	}
}

int Menu::getLastIndex() {
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

	return counter - 1;
}

int Menu::getWidestItemIndex()
{
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	float greatestWidth = 0;
	int index = 0;
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			float itemWidth = textObjs[i]->getLocalBounds().width;
			if (itemWidth > greatestWidth) {
				greatestWidth = itemWidth;
				index = i;
			}
		} else {
			break;
		}
	}

	return index;
}
