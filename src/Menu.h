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
#include "uiTools.h" // for cornerType type #check - make separate file?

class Menu {

public:
	enum menuType { DYNAMIC, STATIC };	//#check

	bool menuShown;	//when enabled, menu is drawn every frame to the window (false by default)
	bool showComponentOutlines;	//when enabled, draws outlines around UI elements to the screen
	bool showMenuBounds;	//#check

	std::string defaultFontName;	//same as in defaultTextTemplate

	// Object arrays for storing and pulling UI elements
	// Max number of each type is hardcoded in
	// Experiment with a tuple format (ELEMENT, SHOWN/DYNAMIC)? #check
	sf::Text* textObjs[30]; // #check - make private?

	sf::Text defaultTextObj;

	sf::RectangleShape outline;

	// ======================================================
	// Constructors
	/**
	* Default constructor for the Menu class
	*/
	Menu();

	// ------------------------------------------------------
	/**
	* Not yet implemented
	*/
	Menu(cornerType dockingPosition, cornerType textOriginPoint, float paddingX, float paddingY);

	// ------------------------------------------------------
	/**
	* Not yet implemented
	*/
	Menu(cornerType dockingPosition, cornerType textOriginPoint, float paddingX, float paddingY, sf::Text defaultTextObj);

	// ------------------------------------------------------
	/**
	* Not yet implemented
	*/
	Menu(cornerType dockingPosition, cornerType textOriginPoint, float paddingX, float paddingY, int componentBuffer);

	// ------------------------------------------------------
	/**
	* Not yet implemented
	*/
	Menu(cornerType dockingPosition, cornerType textOriginPoint, float paddingX, float paddingY, sf::Color backgroundColor);

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
	bool setDockingPosition(cornerType corner);

	// ------------------------------------------------------
	/**
	* Sets which corner to start drawing text objects at relative the menu
	*
	*   @param corner - which corner to start drawing text objects at
	*	@return true if changed succesfully
	*/
	bool setTextOriginPoint(cornerType corner);

	bool setPadding(float x, float y);

	// ======================================================
	// Miscallaneous

	// only for testing
	void printSomething();

	// ------------------------------------------------------
	/**
	* Draws new menu item relative to other existing items. Currently only supported for sf::Text objects.
	* Creates new sf::Text object and stores it in textObjs[], will re-use font objects if possible
	*
	*   @param win - the window on which we’ll draw the menu components
	*	@param text - what string should the text object hold
	*	@return pointer to added object if added sucessfully. Otherwise returns a NULL pointer
	*/
	sf::Text* addMenuItem(sf::RenderWindow& win, const std::string text);

	// ------------------------------------------------------
	/**
	* Draws new menu item relative to other existing items. Currently only supported for sf::Text objects.
	* Creates new sf::Text object and stores it in textObjs[], will re-use font objects if possible
	*
	*   @param win - the window on which we’ll draw the menu components
	*	@param text - what string should the text object hold
	*	@param textObj - the text object to use to add the menu item
	*	@return pointer to added object if added sucessfully. Otherwise returns a NULL pointer
	*/
	sf::Text* addMenuItem(sf::RenderWindow& win, const std::string text, const sf::Text& textObj);

	// ------------------------------------------------------
	/**
	* Toggles the menu on/off
	*/
	void toggleShown();

	// ------------------------------------------------------
	/**
	* Draws all items to the window - only function that must be called EVERY frame.
	* Does some basic reformatting of objects in the menu
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
	const cornerType DEFAULT_DOCKING_POSITION = TOP_LEFT;
	const cornerType DEFAULT_TEXT_ORIGIN_POINT = TOP_LEFT; // #check - change depending on where window is docked?
	const int DEFAULT_COMPONENT_BUFFER = 10; // #check

	// misc
	const std::string BACKUP_FONT_NAME = "arial.ttf"; // used for creating backupFontObj

	// ------------------------------------------------------
	// Variables
	// formatting
	cornerType dockingPosition;	//where to start drawing elements relative to the window
	cornerType textOriginPoint;	//which corner to start drawing text objects at.
	sf::Vector2f bounds; //width and height of the menu
	float paddingX;	//amount of space between edge of menu and start of UI
	float paddingY;	//amount of space between edge of menu and start of UI elements
	int componentBuffer;	//space between menu components
	int numElements;	//number of components currently in the debug menu. Starts at 0 at the beginning of draw()
	bool outlinePosSet;	//used to determine whether to set the oultine position in draw()
	bool mustReformatElements;

	std::unordered_map<std::string, sf::Font*> fonts;
	sf::Font* backupFontObj;	//used when user doesn't specify font to use

	// misc
	sf::Color backgroundColor;	//set to transparent by default

	// gets the inner corner of the menu from the outer corner
	sf::Vector2f getInnerCorner(sf::Vector2f outerCorner);

	void setBounds(float x, float y);

	// reformat existing elements to use new padding; called in setPadding()
	void applyNewPadding(float diffPaddingX, float diffPaddingY);

	// reformat elements within the menu
	void reformatElements(sf::RenderWindow& win);

	sf::Text* menuAdditionOperations(sf::RenderWindow& win, std::string text, const sf::Text& objToUse);

	// gets the index of the last non-null entry in the textObjs array
	int getLastIndex();

	// deprecated?
	void outputInvalidCornerTypeMessage(std::string functionName);
};