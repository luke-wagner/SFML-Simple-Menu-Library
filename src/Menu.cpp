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
#include "roundedRect.h"

Menu::Menu() {
	initialized = false;

	// Set default values
	setType(DYNAMIC);

	// formatting
	setDockingPosition(DEFAULT_DOCKING_POSITION);
	textOriginPoint = DEFAULT_TEXT_ORIGIN_POINT;
	setBounds(0, 0);
	setPadding(DEFAULT_PADDING.x, DEFAULT_PADDING.y);
	setComponentBuffer(DEFAULT_COMPONENT_BUFFER);
	setCompOutlinePadding(DEFAULT_COMP_OUTLINE_PADDING);
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

	defaultTextObj.setCharacterSize(DEFAULT_CHAR_SIZE);
	defaultTextObj.setFont(*backupFontObj);

	componentOutlineObj.setFillColor(sf::Color::Blue); // #check

	background.setFillColor(sf::Color::Transparent);

	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(1);
	outline.setOutlineColor(sf::Color::White);

	// miscellaneous
	menuShown = true;
	componentOutlinesShown = false;
	menuBoundsShown = false;
	setBackgroundColor(sf::Color::Transparent);

	initialized = true;
}

Menu::~Menu()
{
}


void Menu::setType(menuType type) {
	if (!initialized) {
		type = type;
	} else {
		std::cout << "ERROR: trying to set type after menu has been initialized. (Menu::setType())\n";
	}
}

bool Menu::setDockingPosition(uiTools::cornerType corner)
{
	if (dockingPosition == corner) {
		// nothing changes
		return true; // #check
	}

	dockingPosition = corner;

	if (type == DYNAMIC) {
		textOriginPoint = corner;
	}

	if (numElements > 0) {
		mustReformatElements = true; // reformat elements on the next draw call
	}

	return true;
}

bool Menu::setTextOriginPoint(uiTools::cornerType corner)
{
	if (type == DYNAMIC) {
		std::cout << "ERROR: setTextOriginPoint() should only be called on STATIC type menus.\n";
		return false;
	} else {
		textOriginPoint = corner;
		return true;
	}
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

bool Menu::setCompOutlinePadding(float newVal)
{
	// data validation #check
	compOutlinePadding = newVal;

	if (numElements > 0)
		mustReformatElements = true; //#check

	return true;
}

void Menu::setBackgroundColor(sf::Color color)
{
	backgroundColor = color;
	background.setFillColor(backgroundColor);
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

sf::Text* Menu::addMenuItem(sf::RenderWindow& win, const std::string text, const sf::Text* textObj = nullptr) {
	const sf::Text* objToUse = textObj ? textObj : &defaultTextObj; //uses textObj if provided, otherwise defaultTextObj

	int index = -1;
	sf::Text* addedItem = addTextObj(*objToUse, index); // Add the new text object and store its index

	if (addedItem) {
		// set added item properties
		addedItem->setString(text);
		int addedItemHeight = addedItem->getCharacterSize();
		uiTools::setObjectOrigin(*addedItem, textOriginPoint);

		// get last element added
		sf::Text* lastElementAdded = nullptr;
		if (index >= 1) {
			lastElementAdded = textObjs[index - 1];
		}

		// set position of added item and/or move previous menu items based on lastElementAdded
		if (dockingPosition == uiTools::TOP_LEFT || dockingPosition == uiTools::TOP_RIGHT) {
			addedItem->setPosition(calculateTextObjPosition(lastElementAdded, win));
		} else {
			//using nullptr here means calculateTextObjPosition() will return the menu's inner corner
			addedItem->setPosition(calculateTextObjPosition(nullptr, win));
			movePreviousElements(index, win);
		}

		updateBounds(addedItem, addedItemHeight); // update menu bounds

		numElements++;
		return addedItem;
	} else {
		std::cout << "ERROR: Not able to add menu item. Maximum elements already added.\n";
		return nullptr;
	}
}

sf::Text* Menu::findMenuItem(const std::string text)
{
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			// check object's string field
			if (textObjs[i]->getString() == text) {
				return textObjs[i];
			}
		} else {
			break;
		}
	}

	return nullptr;
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
					setBounds(bounds.x, bounds.y - objToRemove->getCharacterSize() - componentBuffer);
				} else {
					setBounds(paddingX * 2, paddingY * 2);
				}

				// delete object and reformat array
				delete textObjs[i];
				textObjs[i] = nullptr;
				numElements--;
				reformatArray(textObjs, len);

				if (widest && textObjs[getWidestItemIndex()] != NULL) {
					sf::Text* widestItemPostDeletion = textObjs[getWidestItemIndex()];
					float newWidth = widestItemPostDeletion->getLocalBounds().width + widestItemPostDeletion->getCharacterSize() *
						compOutlinePadding / 2 + paddingX * 2;
					setBounds(newWidth, bounds.y);
				}

				return true;
			}
		} else {
			return false;
		}
	}
}

bool Menu::removeLastItem()
{
	// find index of last item
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	int index = -1;
	for (int i = 0; i < len; i++) {
		if (textObjs[i] != NULL) {
			index++;
		} else {
			break;
		}
	}

	if (index < 0)
		return false;
	else {
		// remove item
		sf::Text* objToRemove = textObjs[index];
		removeMenuItem(objToRemove);
		return true;
	}	
}

bool Menu::removeFirstItem()
{
	if (textObjs[0] != NULL) {
		return removeMenuItem(textObjs[0]);
		return true;
	} else {
		return false;
	}
}

void Menu::draw(sf::RenderWindow& win)
{
	if (mustReformatElements) {
		reformatElements(win); // #check
		mustReformatElements = false;
	}

	sf::Vector2f windowBounds = { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) };
	if (background.getPosition() != uiTools::cornerTypeToVector(dockingPosition, windowBounds)) {
		// dockingPosition has changed; must adjust bakground and outline positions
		background.setPosition(uiTools::cornerTypeToVector(dockingPosition, windowBounds));
		outline.setPosition(uiTools::cornerTypeToVector(dockingPosition, windowBounds));
	}

	// draw background
	if (type == STATIC || numElements > 0)
		win.draw(background);

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

			if (menuShown) {
				if (componentOutlinesShown) {
					//uiTools::drawOutline(win, t, sf::Color::White); #check

					//scale componentOutlineObj to item size - must use copy to scale bevels properly
					sf::Vector2f itemPos = textObjs[i]->getPosition();
					float paddingAmountX = compOutlinePadding * textObjs[i]->getCharacterSize();
					float paddingAmountY = compOutlinePadding * textObjs[i]->getCharacterSize();
					sf::Vector2f rectSize = { textObjs[i]->getLocalBounds().width + paddingAmountX,
						static_cast<float>(textObjs[i]->getCharacterSize()) + paddingAmountY };

					float offsetX;
					float offsetY;
					switch (dockingPosition) {
					case uiTools::TOP_LEFT:
						offsetX = paddingAmountX / 2;
						offsetY = paddingAmountY / 2;
						break;
					case uiTools::BOTTOM_LEFT:
						offsetX = paddingAmountX / 2;
						offsetY = (paddingAmountY / 2) * (-1);
						break;
					case uiTools::TOP_RIGHT:
						offsetX = (paddingAmountX / 2) * (-1);
						offsetY = paddingAmountY / 2;
						break;
					case uiTools::BOTTOM_RIGHT:
						offsetX = (paddingAmountX / 2) * (-1);
						offsetY = (paddingAmountY / 2) * (-1);
						break;
					}
					sf::Vector2f rectOrigin = uiTools::cornerTypeToVector(textOriginPoint, { textObjs[i]->getLocalBounds().width + paddingAmountX,
						static_cast<float>(textObjs[i]->getCharacterSize()) + paddingAmountY });
					rectOrigin.x += offsetX;
					rectOrigin.y += offsetY;
					
					// configure rounded rect
					sf::ConvexShape copy = createRoundedRect(rectSize, componentOutlineObj.getFillColor(), componentOutlineObj.getOutlineThickness(), componentOutlineObj.getOutlineColor());
					copy.setOrigin(rectOrigin);
					copy.setPosition(itemPos);

					//draw shape
					win.draw(copy);
				}
				win.draw(*textObjs[i]);
			}
		} else {
			break;
		}
	}

	if (menuShown && menuBoundsShown &&
		(type == STATIC || (type == DYNAMIC && bounds.y > paddingY * 2))) {
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

	// update background and outline objects
	background.setSize(bounds);
	background.setOrigin(uiTools::cornerTypeToVector(dockingPosition, bounds));
	outline.setSize(bounds);
	outline.setOrigin(uiTools::cornerTypeToVector(dockingPosition, bounds));
}

void Menu::reformatElements(sf::RenderWindow& win)
{
	// Brute force method #improve
	// Remove all menu items, storing copies in vector
	std::vector<sf::Text*> copies;
	int counter = 0;
	while (textObjs[0] != NULL) {
		sf::Text* copy = new sf::Text(*textObjs[0]);
		removeMenuItem(textObjs[0]);
		copies.push_back(copy);
		counter++;
	}

	// Re-add all items
	for (int i = 0; i < counter; i++) {
		addMenuItem(win, copies[i]->getString(), copies[i]);
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

void Menu::applyCompBufferDiff(int diff) {
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

sf::Text* Menu::addTextObj(const sf::Text& objToUse, int& index) {
	int len = sizeof(textObjs) / sizeof(textObjs[0]);
	for (int x = 0; x < len; x++) {
		if (textObjs[x] == NULL) {
			textObjs[x] = new sf::Text(objToUse);
			index = x;
			return textObjs[x];
		}
	}
	return nullptr;
}

sf::Vector2f Menu::calculateTextObjPosition(sf::Text* lastElementAdded, const sf::RenderWindow& win) {
	sf::Vector2f outerCorner = uiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
	sf::Vector2f innerCorner = getInnerCorner(outerCorner);

	if (lastElementAdded != nullptr) {
		return { innerCorner.x, lastElementAdded->getPosition().y + lastElementAdded->getCharacterSize() + componentBuffer };
	} else {
		return innerCorner;
	}
}

void Menu::movePreviousElements(int index, const sf::RenderWindow& win) {
	sf::Vector2f outerCorner = uiTools::cornerTypeToVector(dockingPosition, { static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) });
	sf::Vector2f innerCorner = getInnerCorner(outerCorner);
	textObjs[index]->setPosition(innerCorner);

	for (int i = index - 1; i >= 0; i--) {
		sf::Text* lastElementAdded = textObjs[i + 1];
		float newY = lastElementAdded->getPosition().y - lastElementAdded->getCharacterSize() - componentBuffer;
		textObjs[i]->setPosition(textObjs[i]->getPosition().x, newY);
	}
}

void Menu::updateBounds(const sf::Text* addedItem, int addedItemHeight) {
	sf::FloatRect addedItemBounds = addedItem->getLocalBounds();
	float newBoundsX = bounds.x;
	float newBoundsY = 0;

	if (addedItemBounds.width + paddingX * 2 + addedItemHeight * compOutlinePadding / 2 > bounds.x) {
		newBoundsX = addedItemBounds.width + paddingX * 2 + addedItemHeight * compOutlinePadding / 2;
	}
	if (numElements == 0) {
		newBoundsY = bounds.y + addedItemHeight + addedItemHeight * compOutlinePadding / 2;
	} else {
		newBoundsY = bounds.y + addedItemHeight + componentBuffer;
	}
	setBounds(newBoundsX, newBoundsY);
}