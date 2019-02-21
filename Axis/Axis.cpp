/********
 * Fichier: Axis_Status.cpp
 * Auteurs: M.-A Martel
 * Date: 06 Février 2019 (creation)
 * Description: Implementation des methodes des classes decrites dans
 *    forme.h. Les methodes de la classe Forme ne doivent pas etre
 *    modifiees. Ce fichier fait partie de la distribution de Graphicus.
********/

#include "Axis.h"
using namespace std;

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

Axis::Axis(uint8_t AxisID, uint32_t baud)
{
	ID		= AxisID;
	uint8_t get_id[16];
	uint8_t scan_cnt = 0;

	// **** Initialisation de la communication avec les moteurs ****
	result = dxl.init(DEVICE_NAME, 57600);
	if (result == false)
	{
	Serial.println(log);
	Serial.println("Failed to init");
	}
	else
	{
	Serial.print("Succeed to init : ");
	Serial.println(baud);  
	}

	// **** Scan du reseau pour lire les moteurs ****
	uint8_t range = 10;
	result = dxl.scan(get_id, &scan_cnt, range);
	if (result == false)
	{
	Serial.println(log);
	Serial.println("Failed to scan");
	}
	else
	{
	Serial.print("Find ");
	Serial.print(scan_cnt);
	Serial.println(" Dynamixels");
	}

	for (int cnt = 0; cnt < scan_cnt; cnt++)
	{
		Serial.print("id : ");
		Serial.print(get_id[cnt]);
		Serial.print(" model name : ");
		Serial.println(dxl.getModelName(get_id[cnt]));
	}
}

Axis::~Axis()
{}

int Axis::readRegister(String regName)
{
	int32_t data = 0;


	int result = dxl.itemRead(ID, regName.c_str(), &data, &log);

	if (result == false)
        {
          Serial.println(log);
         // Serial.println("Failed to read");
        }
        else
        {
          Serial.println(log);
          //Serial.print("read data : ");
          //Serial.println(data);
        }

	return data;
}

void Axis::writeRegister(String regName, int32_t value)
{
	int result = dxl.writeRegister(ID, regName.c_str(), value, &log);
	if (result == false)
	{
		Serial.println(log);
		Serial.println("Failed to write");
	}
	else
	{
		Serial.println(log);
		Serial.print("Succeed to write data : ");
		Serial.println(value);
	}
}

int Axis::getPosition()
{
	Sts_ActualPosition = convertValue2Angle(readRegister("Present_Position"));

	if(Sts_ActualPosition>360)
	{
		float temp = (Sts_ActualPosition/360)-(int(Sts_ActualPosition/360));
		Sts_ActualPosition = (temp*360);
	}

	Serial.println("Actual Position is : ");
	Serial.println(Sts_ActualPosition);

	return Sts_ActualPosition;
}

int Axis::getCurrent()
{
	Sts_ActualCurrent = dxl.convertValue2Current(readRegister("Present_Current"));

	Serial.println("Actual Current is : ");
	Serial.println(Sts_ActualCurrent);
	Serial.println(" mA");

	return Sts_ActualCurrent;
}

int Axis::getTorque()
{
	Sts_ActualTorque = dxl.convertValue2Load(readRegister("Present_Current"));

	Serial.println("Actual Torque is : ");
	Serial.println(Sts_ActualTorque);
	Serial.println(" %%");

	return Sts_ActualTorque;
}

int Axis::getVelocity()
{
	Sts_ActualVelocity = dxl.convertValue2Velocity(ID,readRegister("Present_Velocity"));

	Serial.println("Actual Velocity is : ");
	Serial.println(Sts_ActualVelocity);
	Serial.println(" RPM");

	return Sts_ActualVelocity;
}

int Axis::getMovingStatus()
{
	Sts_Moving = readRegister("Moving");

	if(Sts_Moving)
	{
		Serial.println("Motor is Running");
	}
	else
	{
		Serial.println("Motor is stopped");
	}
	

	return Sts_Moving;
}

float Axis::convertValue2Angle(int value)
{ 
    return (value*360/4095); 
}

int Axis::convertAngle2Value(float angle)
{ 
    return (angle*4095/360); 
}