// #############################################################################
//
// # Name       : Sensors
// # Version    : 1.9
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 17.10.2015
// 
// # Description: Library for managing Huertomato's sensors
// # In charge of polling all hardware and smoothing values afterwards
// 
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef SENSORS_H
#define SENSORS_H

#include "Settings.h"
#include "Sensor.h"
#include "SensorEC.h"
#include "SensorHumid.h"
#include "SensorLight.h"
#include "SensorPH.h"
#include "SensorTemp.h"
#include "SensorWater.h"
//#include <Arduino.h>
//#include <DHT11.h>
//#include <DallasTemperature.h>


//Pin numbers are previously declared in Huertomato.ino
extern const uint8_t humidIn;
extern const uint8_t lightIn;
extern const uint8_t waterEcho;
extern const uint8_t waterTrigger;

//Contains all other instances of sensor classes
class Sensors {
  public:
	enum Sensor {
		None,
		Temperature,
		Humidity,
		Light,
		Ec,
		Ph,
		Level
	};
    //Constructors
    Sensors(Settings *settings);
	Sensors(const Sensors &other);
	Sensors& operator=(const Sensors &other);
	//Destructor
	~Sensors(); 
    //Getters
    float getTemp() const;
    uint16_t getLight() const;
    uint8_t getHumidity() const;
    uint16_t getEC() const;
    float getPH() const;
    uint8_t getWaterLevel() const;
	//Sets different modes. Should be called when settings counterpart gets called.
	void setSerialDebug(boolean);
	void setReservoir(boolean);
	//Poll sensor and get raw data
	uint16_t getRawWaterLevel();
	uint16_t getRawLightLevel();
	//Alarm tests - True if sensor value is off range.
	boolean ecOffRange();
	boolean phOffRange();
	boolean lvlOffRange();
    //Updates sample arrays with readings from sensors and smoothes data
    void update();
	//Reads once from each sensor, fills the array with this measurement and smoothes
	void fastUpdate();
	
	//Set temp mode. Should be called when it's counterpart in setting does too.
	void setCelsius(boolean);
	//This should be set while calibrating to prevent messing up circuits if update() called
	void calibratingPH(boolean);
	void calibratingEC(boolean);
	//pH circuit commands
	void resetPH();
	void getPHinfo();
	void setPHled(boolean);
	void setPHcontinuous();
	void setPHstandby();
	void setPHfour();
	void setPHseven();
	void setPHten();
	//Adjust pH readings to temperature
	void adjustPHtemp();
	//EC circuit commands
	void resetEC();
	void getECinfo();
	void setECled(boolean);
	void setECcontinuous();
	void setECstandby();
	void setECprobeType();
	void setECdry();
	void setECtenThousand();
	void setECfortyThousand();
	//Adjusts EC sensor readings to temperature
	void adjustECtemp();

  private:
	Settings *_settings;
	
	SensorEC _ec;
	SensorHumid _humidity;
	SensorLight _light;
	SensorPH _ph;
	SensorTemp _temp;
	SensorWater _water;
	
	//Keeps track of reservoir activation
	boolean _reservoir;

};

#endif
