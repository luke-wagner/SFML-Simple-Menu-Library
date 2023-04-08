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
	enum menuType { DYNAMIC, STATIC }; // #check - move outside of class?

	sf::Text defaultTextObj;
	sf::ConvexShape componentOutlineObj;

	// ======================================================
	// Constructors + Destructor
	/**
	* Default constructor for the Menu class
	*/
	Menu();

	// Add other constructors? #improve

	~Menu();

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
	* Sets which corner to start drawing text objects at relative the menu.
	* Only works for STATIC type menus.
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

	// ------------------------------------------------------
	/**
	* Sets the spacing between menu elements and the edges of their backgrounds
	*
	*   @param newVal - the new padding value
	*	@return true if changed succesfully
	*/
	bool setCompOutlinePadding(float newVal);

	// ------------------------------------------------------
	/**
	* Sets the background color of the menu
	*
	*   @param color - the color to set to
	*/
	void setBackgroundColor(sf::Color color);

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
	/**
	* Adds new menu item relative to other existing items. Currently only supported for sf::Text objects.
	* Creates new sf::Text object and stores it in textObjs[], will re-use font objects if possible
	*
	*   @param win - a reference to the window object
	*	@param text - what string should the text object hold
	*	@param textObj - the text object (template) to use to add the menu item; uses defaultTextObj by default
	*	@return pointer to added object if added sucessfully. Otherwise returns a NULL pointer
	*/
	sf::Text* addMenuItem(sf::RenderWindow& win, const std::string text, const sf::Text* textObj = nullptr);

	// ------------------------------------------------------
	/**
	* Looks for a menu item in textObjs[] with a matching string field.
	* Returns a pointer to the first object that matches these descriptions
	*
	*   @param text - the string of the object we wish to find
	*	@return pointer to the object if found; if not return nullptr
	*/
	sf::Text* findMenuItem(const std::string text);

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
	* Removes the first item in the textObjs array
	*
	*   @return true if removed sucessfully
	*/
	bool removeFirstItem();

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
	// ------------------------------------------------------
	// Constants
	// formatting
	const uiTools::cornerType DEFAULT_DOCKING_POSITION = uiTools::TOP_LEFT;
	const uiTools::cornerType DEFAULT_TEXT_ORIGIN_POINT = uiTools::TOP_LEFT;
	const sf::Vector2f DEFAULT_PADDING = { 25, 25 };
	const int DEFAULT_COMPONENT_BUFFER = 10;
	const int DEFAULT_CHAR_SIZE = 20;
	const float DEFAULT_COMP_OUTLINE_PADDING = 1;	//percentage relative to component height

	// misc
	const std::string BACKUP_FONT_NAME = "arial.ttf"; // used for creating backupFontObj
	// NOTE: To change deafult font: must also change constructors

	// ------------------------------------------------------
	// Variables
	menuType type;	//either dynamic or static
	bool initialized; //set to true at end of constructors
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
	float compOutlinePadding;	//space between menu components and their outline objects
	int numElements;	//number of total menu items
	bool mustReformatElements;	//if true, menu items are reformatted at start of draw()

	std::unordered_map<std::string, sf::Font*> fonts;
	sf::Font* backupFontObj;	//used when user doesn't specify what font to use

	sf::RectangleShape background; //the object for drawing the menu's background
	sf::RectangleShape outline;	//the object for drawing the bounds of the menu

	// misc
	sf::Color backgroundColor;	//set to transparent by default

	// object arrays for storing and pulling UI elements
	sf::Text* textObjs[30];

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

	// ------------------------------------------------------
	// Helper Functions
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

	/**
	* Helper for addMenuItem(). Adds a new text object to the textObjs array. 
	* Calls default constructor and does not change any of the new item's properties.
	*   @param objToUse - the text object to be added.
	*   @param index - reference to the index where the text object is added.
	*   @return pointer to the added text object or nullptr if the array is full.
	*/
	sf::Text* addTextObj(const sf::Text& objToUse, int& index);

	/**
	* Helper for addMenuItem(). Calculates the position of the text object based on the last element added.
	*   @param lastElementAdded - pointer to the last added text object or nullptr if there is no previous element.
	*   @param win - Reference to the RenderWindow object.
	*   @return sf::Vector2f containing the calculated position of the text object.
	*/
	sf::Vector2f calculateTextObjPosition(sf::Text* lastElementAdded, const sf::RenderWindow& win);

	/**
	* Helper for addMenuItem(). Moves previous elements up by one "space" when 
	* dockingPosition is BOTTOM_LEFT or BOTTOM_RIGHT.
	*   @param index - the index of the current added text object in the textObjs array.
	*   @param win - reference to the RenderWindow object.
	*/
	void movePreviousElements(int index, const sf::RenderWindow& win);

	/**
	* Helper for addMenuItem(). Updates the menu bounds based on the added item.
	*   @param addedItem - pointer to the recently added text object.
	*/
	void updateBounds(const sf::Text* addedItem, int addedItemHeight);
};