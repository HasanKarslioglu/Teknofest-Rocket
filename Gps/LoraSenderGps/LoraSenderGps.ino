#define E32_TTL_1W
#include "LoRa_E32.h" 
#include <SoftwareSerial.h>
#include <TinyGPS.h>
 
//PARAMETRE AYARLARI
#define LoraRX 3 //Lora üzerinde 4
#define LoraTX 4 //Lora üzerinde 3
//#define GpsRX 8 //Gps üzerinde 9
//#define GpsTX 9 //Gps üzerinde 8
#define LoraM0 7
#define LoraM1 6
#define Adres 1   
#define Kanal 20 
#define GonderilecekAdres 2 

SoftwareSerial LoraSerial(LoraRX, LoraTX);

SoftwareSerial gpsSerial(8,9);

LoRa_E32 lora(&LoraSerial);
TinyGPS gps;

void printParameters(struct Configuration configuration);

struct Signal {
  byte packetNo[1];
  byte sat[1];
  byte lat[10];
  byte lng[10];
} data;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
	delay(100);

  gpsSerial.begin(9600);
  delay(100);

  lora.begin();
  pinMode(LoraM0, OUTPUT);
  pinMode(LoraM0, OUTPUT);
  LoraE32Ayarlar();
  delay(200);
}

byte i = 0; 

void loop() {

  int sensorValue = analogRead(A0); // A0 pininden analog veri oku
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);

  unsigned long start = millis();
  do {
    Serial.println(gpsSerial.available());
    if(gpsSerial.available()){
      Serial.write(gpsSerial.read());
    }
    if(Serial.available()){
      gpsSerial.write(Serial.read());
    }
  } while(millis() - start < 1000);

  ResponseStatus rs = lora.sendFixedMessage(highByte(GonderilecekAdres), lowByte(GonderilecekAdres), Kanal, &data, sizeof(Signal));
  Serial.println(rs.getResponseDescription());
  //smartdelay(1000);

  //*(byte*)data.sat = gps.satellites();
  //float lat, lng;
  //gps.f_get_position(&lat, &lng);
  //*(float*)data.sat = lat;
  //*(float*)data.sat = lng;
  //*(byte*)data.packetNo = ++i;

  //Serial.print("Packet No: "); Serial.print(*(byte*)data.packetNo);
  //Serial.print("/ Sat: "); Serial.print(*(byte*)data.sat);
  //Serial.print("/ Lat: "); Serial.print(lat, 6);
  //Serial.print("/ Lng: "); Serial.println(lng, 6);
}

static void smartdelay(unsigned long ms){
  
}
 
void LoraE32Ayarlar() {
  digitalWrite(LoraM0, HIGH);
  digitalWrite(LoraM0, HIGH);
  Serial.println("Ayarlar Başlatıldı!");

  ResponseStructContainer c = lora.getConfiguration();
  Configuration configuration;
  memcpy(&configuration, c.data, sizeof(Configuration));
  
  configuration.ADDL = lowByte(Adres);
  configuration.ADDH = highByte(Adres);
  configuration.CHAN = Kanal;
  
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
  configuration.OPTION.transmissionPower = POWER_30;
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;
  configuration.OPTION.fec = FEC_0_OFF;
  configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
  configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
 
  ResponseStatus rs = lora.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  printParameters(configuration);
  c.close();
  digitalWrite(LoraM0, LOW);
  digitalWrite(LoraM0, LOW);
  
  Serial.println("Ayarlar Bitti!");
}

void printParameters(struct Configuration configuration) {
  digitalWrite(LoraM0, HIGH);
  digitalWrite(LoraM0, HIGH);
	
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD : "));  Serial.print(configuration.HEAD, BIN);Serial.print(" ");Serial.print(configuration.HEAD, DEC);Serial.print(" ");Serial.println(configuration.HEAD, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, BIN);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, BIN);
	Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte  : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRate());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRate());
	Serial.print(F("OptionTrans        : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
	Serial.print(F("OptionPullup       : "));  Serial.print(configuration.OPTION.ioDriveMode, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getIODroveModeDescription());
	Serial.print(F("OptionWakeup       : "));  Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
	Serial.print(F("OptionFEC          : "));  Serial.print(configuration.OPTION.fec, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFECDescription());
	Serial.print(F("OptionPower        : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
	Serial.println("----------------------------------------");

  digitalWrite(LoraM0, LOW);
  digitalWrite(LoraM0, LOW);
}
