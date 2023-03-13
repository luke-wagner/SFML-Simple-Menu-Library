/** --------------------------------------------------------
* @file    menu.h
* @author  Luke Wagner
* @version 1.0
*
* SFML Menu Class
*   Contains a menu object class definition for use with SFML
*
* ------------------------------------------------------------ */
#pragma once

#include <unordered_map>
#include "uiTools.h"

class Menu {

public:
	enum menuType { DYNAMIC, STATIC };

	// Object arrays for storing and pulling UI elements
	sf::Text* textObjs[30]; // #check - make private?

	sf::Text defaultTextObj;

	// ======================================================
	// Constructors
	/**
	* Default constructor for the Menu class
	*/
	Menu();

	// Add other constructors? #improve

	// ======================================================
	// Mutators
	/**
	* Sets the menu to either DYNAMIC or STATIC
	*
	*   @param type - the menu type to set to
	*/
	void setType(menuType type);

	// ------------------------------------------------------
	/**
	* Sets the menu's docking position relative to the window
	*
	*   @param corner - which corner to dock to
	*	@return true if changed succesfully
	*/
	bool setDockingPosition(uiTools::cornerType corner);

	// ------------------------------------------------------
	/**
	* Sets which corner to start drawing text objects at relative the menu
	*
	*   @param corner - which corner to start drawing text objects at
	*	@return true if changed succesfully
	*/
	bool setTextOriginPoint(uiTools::cornerType corner);

	// ------------------------------------------------------
	/**
	* Sets how much space (padding) is given between the edges of the window
	* and the start of the menu.
	*
	*   @param x - amount of horizontal space
	*   @param y - amount of vertical space
	*	@return true if changed succesfully
	*/
	bool setPadding(float x, float y);

	// ------------------------------------------------------
	/**
	* Sets the spacing between menu elements and reformats existing elements
	* to use new component buffer if necessary.
	*
	*   @param newVal - the new component buffer value
	*	@return true if changed succesfully
	*/
	bool setComponentBuffer(int newVal);

	// ======================================================
	// Appearance
	/**
	* Causes menu to be displayed to the window as part of Menu::draw()
	*/
	void showMenu();

	// ------------------------------------------------------
	/**
	* Stops menu from being displayed to the window in Menu::draw()
	*/
	void hideMenu();

	// ------------------------------------------------------
	/**
	* Toggles the menu on/off
	*/
	void toggleMenuShown();

	// ------------------------------------------------------
	/**
	* Causes menu outline to be displayed to the window as part of Menu::draw()
	*/
	void showMenuBounds();

	// ------------------------------------------------------
	/**
	* Stops menu outline from being displayed to the window in Menu::draw()
	*/
	void hideMenuBounds();

	// ------------------------------------------------------
	/**
	* Toggles the menu bounds from being shown on/off
	*/
	void toggleMenuBounds();

	// ------------------------------------------------------
	/**
	* Causes menu item outlines to be displayed to the window as part of Menu::draw()
	*/
	void showComponentOutlines();

	// ------------------------------------------------------
	/**
	* Stops menu item outlines from being displayed to the window in Menu::draw()
	*/
	void hideComponentOutlines();

	// ------------------------------------------------------
	/**
	* Toggles the component outlines from being shown on/off
	*/
	void toggleComponentOutlines();

	// ======================================================
	// Miscallaneous

	// only for testing #remove
	void printSomething();

	// ------------------------------------------------------
	/**
	* Adds new menu item relative to other existing items. Currently only supported for sf::Text objects.
	* Creates new sf::Text object and stores it in textObjs[], will re-use font objects if possible
	*
	*   @param win - a reference to the window object
	*	@param text - what string should the text object hold
	*	@return pointer to added object if added sucessfully. Otherwise returns a NULL pointer
	*/
	sf::Text* addMenuItem(sf::RenderWindow& win, const std::string text);

	// ------------------------------------------------------
	/**
	* Adds new menu item relative to other existing items. Currently only supported for sf::Text objects.
	* Creates new sf::Text object and stores it in textObjs[], will re-use font objects if possible
	*
	*   @param win - a reference to the window object
	*	@param text - what string should the text object hold
	*	@param textObj - the text object to use to add the menu item
	*	@return pointer to added object if added sucessfully. Otherwise returns a NULL pointer
	*/
	sf::Text* addMenuItem(sf::RenderWindow& win, const std::string text, const sf::Text& textObj);

	// ------------------------------------------------------
	/**
	* Removes an item from the menu. Gets a pointer to the object to remove and deletes
	* it from textObjs[] and frees up allocated memory. Important: if a pointer to this
	* item was saved in addMenuItem(), it should be set to null, to prevent trying
	* to access memory it can't access anymore.
	*
	*   @param objToRemove - a pointer to the object to remove
	*	@return true if deletion was succesful
	*/
	bool removeMenuItem(sf::Text* objToRemove);

	// ------------------------------------------------------
	/**
	* Removes the last item in the textObjs array
	*
	*   @return true if removed sucessfully
	*/
	bool removeLastItem();

	// ------------------------------------------------------
	/**
	* Draws all menu items to the window - only function that must be called EVERY frame.
	* Also does some basic reformatting of objects in the menu
	* so that they all remain within the bounds of the window.
	*
	*   @param win - the window on which we’ll draw the menu components
	*/
	void draw(sf::RenderWindow& win);

private:
	menuType type;	//either dynamic or static

	// ------------------------------------------------------
	// Constants
	// formatting
	const uiTools::cornerType DEFAULT_DOCKING_POSITION = uiTools::TOP_LEFT;
	const uiTools::cornerType DEFAULT_TEXT_ORIGIN_POINT = uiTools::TOP_LEFT;
	const sf::Vector2f DEFAULT_PADDING = { 25, 25 };
	const int DEFAULT_COMPONENT_BUFFER = 10;

	// misc
	const std::string BACKUP_FONT_NAME = "arial.ttf"; // used for creating backupFontObj

	// ------------------------------------------------------
	// Variables
	bool menuShown;	//when enabled, menu is drawn every frame to the window (false by default)
	bool componentOutlinesShown;	//when enabled, draws outlines around UI elements to the screen
	bool menuBoundsShown;	//draws an outline around the whole menu

	// formatting
	uiTools::cornerType dockingPosition;	//where to start drawing elements relative to the window
	uiTools::cornerType textOriginPoint;	//which corner to start drawing text objects at.
	sf::Vector2f bounds;	//width and height of the menu
	float paddingX;	//amount of space between edge of menu and start of UI
	float paddingY;	//amount of space between edge of menu and start of UI elements
	int componentBuffer;	//space between menu components
	int numElements;	//number of total menu items
	bool mustReformatElements;	//if true, menu items are reformatted at start of draw()

	std::unordered_map<std::string, sf::Font*> fonts;
	sf::Font* backupFontObj;	//used when user doesn't specify what font to use

	sf::RectangleShape outline;	//the object for drawing the bounds of the menu

	// misc
	sf::Color backgroundColor;	//set to transparent by default

	// ------------------------------------------------------
	// Functions
	/**
	* Gets the coordinates of the menu's inner corner from the outer corner
	*
	*   @param outerCorner - the coordinates of the menu's outer corner
	*	@return Vector2f - the coordinates of the menu's inner corner
	*/
	sf::Vector2f getInnerCorner(sf::Vector2f outerCorner);

	// ------------------------------------------------------
	/**
	* Sets the menu's bounds and resizes its outline object
	*
	*   @param x - the new width of the menu
	*   @param y - the new height of the menu
	*/
	void setBounds(float x, float y);

	// ------------------------------------------------------
	/**
	* Reformats existing elements to use new padding; called in setPadding()
	*
	*   @param diffPaddingX - the change in horizontal padding
	*   @param diffPadingY - the change in vertical padding
	*/
	void applyPaddingDiff(float diffPaddingX, float diffPaddingY);

	// ------------------------------------------------------
	/**
	* Reformats existing elements to use the new component buffer; called in setCompBuffer()
	*
	*   @param diff - the new value versus the old one
	*/
	void applyCompBufferDiff(int diff);

	// ------------------------------------------------------
	/**
	* Important: INEFFICIENT. Reformats elements within the menu by deleting
	* each menu item and readding it then formatting the menu again.
	* Essentially "recreates" the menu
	*
	*   @param win - a reference to the window object
	*/
	void reformatElements(sf::RenderWindow& win);

	// ------------------------------------------------------
	/**
	* Reorders an array of objects to "fill in any holes" that were
	* created by deleting items from the array. Where this function finds
	* a nullptr (hole), all following items are moved back by 1 spot.
	*
	*   @param array - the array to reformat
	*	@param len - the length of the array
	*/
	void reformatArray(sf::Text* array[], int len);

	// ------------------------------------------------------
	/**
	* Contains most of the functionality for Menu::addMenuItem()
	*
	*   @param win - a reference to the window object
	*	@param text - what the added text object should display
	*	@param objToUse - the sf::Text object to use as a template for formatting
	*/
	sf::Text* menuAdditionOperations(sf::RenderWindow& win, std::string text, const sf::Text& objToUse);

	// ------------------------------------------------------
	/**
	* Gets the index of the last non-null entry in the textObjs array
	*
	*   @return int - the index of the sf::Text object in textObjs[]
	*/
	int getLastIndex();

	// ------------------------------------------------------
	/**
	* Loops over the textObjs array and finds the index of the widest item.
	* If there is a tie, simply returns the index of one of these objects.
	*
	*	@return int - the index of the item
	*/
	int getWidestItemIndex();
};