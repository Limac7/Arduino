#include <Wire.h>

long accX, accY, accZ;
long ziroX, ziroY, ziroZ;
float sensorValue[6] = {0,0,0,0,0,0};
char inbyte = 0;
boolean running = false;


void setup() {
  Serial.begin(9600);
   setupMPU();
  Wire.begin();
}


void loop() {
  recordAccRegisters();
  recordZiroRegisters();
  sendAndroidValues();
   if (Serial.available() > 0)
  {
    inbyte = Serial.read();
    if (inbyte == '0')
    {
      running = false;
    }
    if (inbyte == '1')
    {
   running = true;
    }
  }
  delay(2000);
  
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //Adresa od MPU
  Wire.write(0x6B); //Registar 6B Power
  Wire.write(0b00000000); //Sleep na 0
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x1B); //Registar od žiroskopa
  Wire.write(0x00000000); //  +/- 250
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x1C); //Akcelerometar registar
  Wire.write(0b00000000); //  +/- 2g
  Wire.endTransmission(); 
}

void recordAccRegisters() {
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x3B); //Početak očitanja accelero
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Dohvat registara
  while(Wire.available() < 6);
  accX = Wire.read()<<8|Wire.read(); // dohvaćanje vrijednosti registara
  accY = Wire.read()<<8|Wire.read(); 
  accZ = Wire.read()<<8|Wire.read(); 
  processAccData();
}

void processAccData(){
  sensorValue[3] = accX / 16384.0; //  /16384 zbog 2g po datasheetu
  sensorValue[4] = accY / 16384.0; 
  sensorValue[5] = accZ / 16384.0; 
}

void recordZiroRegisters() {
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x43); 
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); 
  while(Wire.available() < 6);
  ziroX = Wire.read()<<8|Wire.read(); 
  ziroY = Wire.read()<<8|Wire.read(); 
  ziroZ = Wire.read()<<8|Wire.read(); 
  processZiroData();
}

void processZiroData() {
  sensorValue[0]= ziroX / 131.0;  //rotx
  sensorValue[1] = ziroY / 131.0; //roty
  sensorValue[2] = ziroZ / 131.0;//rotz
}

void sendAndroidValues(){
// # zbog android app početak traženog stringa
if(running  == true){
  Serial.print('#');
  //podatci s senzora
  for(int k=0; k<6; k++)
  {
    Serial.print(sensorValue[k]);
    Serial.print('+');
  }
 Serial.print('*'); //kraj stringa
 delay(10); //delay da ne propusti  podatke
}
}


