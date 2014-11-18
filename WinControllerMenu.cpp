#include "WinControllerMenu.h"

WinControllerMenu::WinControllerMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinControllerMenu::WinControllerMenu(const WinControllerMenu &other) : Window(other) { }
	
WinControllerMenu& WinControllerMenu::operator=(const WinControllerMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinControllerMenu::~WinControllerMenu() {}
	
Window::Screen WinControllerMenu::getType() const {
	return Window::ControllerSettings;
}

void WinControllerMenu::print() {
	_soundActive = _settings->getSound();
	_serialActive = _settings->getSerialDebug();
	
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	
	//Print bulletpoints
	_lcd->setFont(various_symbols);
	for (int i = 0; i < nControllerButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yFiveLines+_bigFontSize*_yFactor5lines*i);
	}
	
	//Make menu buttons
	//Before 3 there are flow buttons
	for (int i = 0; i < nControllerButtons - _nFlowButtons; i++) {
		controllerButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yFiveLines+_bigFontSize*_yFactor5lines*i,controllerButtonText[i]);
	}
	
	_lcd->setFont(hallfetica_normal);
	
	//Sound ON/OFF
	if (_soundActive)
		_lcd->print(onStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[_nFlowButtons]),_yFiveLines+_bigFontSize*_yFactor5lines*3);
	else
		_lcd->print(offStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[_nFlowButtons]),_yFiveLines+_bigFontSize*_yFactor5lines*3);
	//Serial Debug ON/OFF
	if (_serialActive)
		_lcd->print(onStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[_nFlowButtons+1]),_yFiveLines+_bigFontSize*_yFactor5lines*4);
	else
		_lcd->print(offStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[_nFlowButtons+1]),_yFiveLines+_bigFontSize*_yFactor5lines*4);
}

//Draws entire screen Controller Settings
void WinControllerMenu::draw() { 
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinControllerMenu);
	addFlowButtons(true,false,true,controllerButtons);
	print();
	_buttons.drawButtons();
}
 
//Redraws only controller settings text from inner temp vars
//Used when +- signs are pressed
void WinControllerMenu::update() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setFont(hallfetica_normal);
	
	//Sound ON/OFF
	if (_soundActive)
		_lcd->print(onStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[3]),_yFiveLines+_bigFontSize*2*3);
	else
		_lcd->print(offStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[3]),_yFiveLines+_bigFontSize*2*3);
	//Serial Debug ON/OFF
	if (_serialActive)
		_lcd->print(onStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[4]),_yFiveLines+_bigFontSize*2*4);
	else
		_lcd->print(offStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(controllerButtonText[4]),_yFiveLines+_bigFontSize*2*4);
}

Window::Screen WinControllerMenu::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == controllerButtons[0]) { return MainMenu; }
	//Exit
	else if (buttonIndex == controllerButtons[2]) { return MainScreen; }
	//Time & Date
	else if (buttonIndex == controllerButtons[3]) { return TimeDate; }
	//Sensor polling
	else if (buttonIndex == controllerButtons[4]) { return SensorPolling; }
	//SD Card
	else if (buttonIndex == controllerButtons[5]) { return SDCard; }
	//Sound toggle
	else if (buttonIndex == controllerButtons[6]) {
		_soundActive = !_soundActive;
		_settings->setSound(_soundActive);
		update();
		//Serial debugging toggle
	} else if (buttonIndex == controllerButtons[7]) {
		_serialActive = !_serialActive;
		_settings->setSerialDebug(_serialActive);
		update();
	}
	return None;
}
