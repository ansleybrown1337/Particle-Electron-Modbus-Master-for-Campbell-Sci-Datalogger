/*
Particle Photon/Electron Modbus Master
Created by A.J. Brown and Sam Gallo, 06/06/18 version 1.4.4
This project was designed to allow a particle photon or electron to act as a
modbus master for a Campbell Scientific datalogger (as Slave) set up with 14
registers that hold water content reflectometer data from 3 CS655 sensors.
*/

#include "Ubidots.h"
#include "ModbusMaster.h"
#include "math.h"

#ifndef TOKEN
#define TOKEN "{INSERT API TOKEN HERE}" //
#endif
#ifndef DEVICE_NAME
#define DEVICE_NAME "IIC_1-3" // Put here your device name
#endif

//instantiate ModbusMaster object as slave ID 1
ModbusMaster node(01);
int BaudRate = 9600;

//Activate communication with udibots server
Ubidots ubidots(TOKEN);

//Indicate the number of registers (variables) to read
int num_register = 12;
uint8_t u_result;

//How often would you like readings to be taken?
//If less than 30, multiple readings per hour
//>30 means it will take a reading on that minute (e.g. 59 == 59th minute)
int ReadingTime = 59; // <-- in minutes


	//The # of these floats are determined by the # of registers read
	float Register0;
	float Register1;
	float Register2;
	float Register3;
	float Register4;
	float Register5;
	float Register6;
	float Register7;
	float Register8;
	float Register9;
	float Register10;
	float Register11;
	//float Register12;
	//float Register13;

	//Name of each of your data variables being read
	float VWC_U0;
	float ECa_0;
	float Temp_0;
	float P_0;
	float VWC_U1;
	float ECa_1;
	float Temp_1;
	float P_1;
	float VWC_U2;
	float ECa_2;
	float Temp_2;
	float P_2;
	//float TT_C;
	//float SBT_C;

	int led = D7;
	int Time_old = 2;
	int check;

void setup() {
	ubidots.setDeviceName(DEVICE_NAME);

	// set pinmode of LED
	pinMode(led, OUTPUT);
	// initialize Modbus communication baud rate
	node.begin(BaudRate);
	/*
	//Below code is for RS485 communication and Rx Tx debugging:
		// D7 is the pin used to control the TX enable pin of RS485 driver
	node.enableTXpin(D7);
		// Print TX and RX frames out on Serial. Beware, enabling this messes up
		// the timings for RS485 Transactions, causing them to fail.
	node.enableDebug();

	//initialize serial monitor baud rate
	Serial.begin(9600);
	Serial.println("Starting Modbus Transaction:");
	*/
}

//Subroutine to send data to Udibots server, device will blink if sent
void SendData(){
  //Sending Readings
	digitalWrite(led, HIGH);
	//Name of the variables that you want to see on the Udibots server
	//(Keep names in "" short.  Long names often cause errors.)
	// calculates your actual timestamp in SECONDS
	unsigned long t = ubidots.ntpUnixTime();

	//Sensor 1
  ubidots.add("10cm ECa", ECa_0);
  ubidots.add("10cm Temp", Temp_0);
  ubidots.add("10cm P", P_0);
	ubidots.add("10cm VWC", VWC_U0);
	ubidots.sendAll();

	//Sensor 2
  ubidots.add("50cm ECa", ECa_1);
  ubidots.add("50cm Temp", Temp_1);
  ubidots.add("50cm P", P_1);
	ubidots.add("50cm VWC", VWC_U1);
	ubidots.sendAll();

	//Sensor 3
	ubidots.add("100cm ECa", ECa_2);
	ubidots.add("100cm Temp", Temp_2);
	ubidots.add("100cm P", P_2);
	ubidots.add("100cm VWC", VWC_U2);
	ubidots.sendAll();

	//Temp sensor
	//ubidots.add("Canopy T", TT_C);
	//ubidots.add("IRT Body", SBT_C);
	ubidots.sendAll();

	//Status Checks
	ubidots.add("Status", u_result);
	CellularSignal sig = Cellular.RSSI();
	ubidots.add("RSSI", sig.rssi);
	ubidots.add("Quality", sig.qual);
	ubidots.sendAll();

  digitalWrite(led, LOW);
}


void loop() {
	static uint32_t i;
	uint8_t j, result;
	uint16_t data[11];

	i++;

if( Time.minute() % ReadingTime == 0){
	result = node.readHoldingRegisters(0x00, num_register);
	Serial.println("Reading Registers...");
  u_result = result, HEX;
	// do something with data if read is successful
	if (result == node.ku8MBSuccess) {
		Serial.print("Success, Received data: ");
		//for j = 0, run loop X times to read X register values.
		for (j = 0; j < num_register; j++) {
			data[j] = node.getResponseBuffer(j);
			Serial.print(data[j], DEC);
			Serial.print(" ");
		}
		Serial.println("");
	} else {
		Serial.print("Failed, Response Code: ");
		Serial.print(result, HEX);
		Serial.println(" ");
		delay(500); //if failed, wait for bit longer, before retrying!
	}
	//Defining the registers
	Register0 = (data[0]);
	Register1 = (data[1]);
	Register2 = (data[2]);
	Register3 = (data[3]);
	Register4 = (data[4]);
	Register5 = (data[5]);
	Register6 = (data[6]);
	Register7 = (data[7]);
	Register8 = (data[8]);
	Register9 = (data[9]);
	Register10 = (data[10]);
	Register11 = (data[11]);
	//Register12 = (data[12]);
	//Register13 = (data[13]);
	//Sensor 1
	P_0 = Register3/1000;
	ECa_0 = Register1/1000;
	Temp_0 = Register2/1000;
	VWC_U0 = Register0/1000;
	//Sensor 2
	P_1 = Register7/1000;
	ECa_1 = Register5/1000;
	Temp_1 = Register6/1000;
	VWC_U1 = Register4/1000;
	//Sensor 3
	P_2 = Register11/1000;
	ECa_2 = Register9/1000;
	Temp_2 = Register10/1000;
	VWC_U2 = Register8/1000;
	//Temp sensor
	//TT_C = Register12/1000;
	//SBT_C = Register13/1000;
	delay(100);
	SendData();
	//D6 is arbitrary, we wont be using the wakeup function.
	//Sleep time is set to wake up 4 minutes before next reading
	System.sleep(D6,RISING,ReadingTime*60-240);

}

}
