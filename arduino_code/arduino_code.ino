#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <AFMotor.h>

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     
#define ADAFRUITBLE_RST 9

AF_DCMotor motorLeft(3);
AF_DCMotor motorRight(4);

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

void setup() {                
  
  BTLEserial.setDeviceName("XAN-I"); 

  BTLEserial.begin();
  
  motorLeft.setSpeed(200);
  motorRight.setSpeed(200);

  motorRight.run(RELEASE);
  motorLeft.run(RELEASE);
}

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
uint8_t DIRECTION = RELEASE;
uint8_t CURRENT_DIRECTION = RELEASE;
String SWITCH_SIDE = "NO";

void loop() {
  BTLEserial.pollACI();
  
  checkForState();
 
  if(DIRECTION != CURRENT_DIRECTION){
    CURRENT_DIRECTION = DIRECTION;
  }
  
  if(SWITCH_SIDE == "RIGHT"){
    motorRight.run(FORWARD);
    motorLeft.run(RELEASE);
  }else if(SWITCH_SIDE == "LEFT"){
    motorRight.run(RELEASE);
    motorLeft.run(FORWARD);
  }else{
    motorRight.run(CURRENT_DIRECTION);
    motorLeft.run(CURRENT_DIRECTION);  
  }
}

void checkForState() {
  aci_evt_opcode_t status = BTLEserial.getState();
  
  if (status == ACI_EVT_CONNECTED) {
    if (BTLEserial.available() > 0 ) {
      
      char data_from_iphone = (char)BTLEserial.read();
      
      switch (data_from_iphone) {
      case '1':   
        DIRECTION = FORWARD;
        break;
      case '2':    
        DIRECTION = BACKWARD;
        break;
      case '3':    
        DIRECTION = RELEASE;
        SWITCH_SIDE = "LEFT";
        break;
      case '4':    
        DIRECTION = RELEASE;
        SWITCH_SIDE = "RIGHT";
        break;
      case '5':    
        DIRECTION = RELEASE;
        SWITCH_SIDE = "NO";
        break;
      default:
        DIRECTION = RELEASE;
      }
    }
  }else{
    DIRECTION = RELEASE;
  }
}



