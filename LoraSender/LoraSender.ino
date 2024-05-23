//Lora Sender
#define E32_TTL_1W
#include "LoRa_E32.h" 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4);
LoRa_E32 lora(&mySerial);

//PARAMETRE AYARLARI
#define M0 7
#define M1 6
#define Adres 1   
#define Kanal 20
#define GonderilecekAdres 2
 
void printParameters(struct Configuration configuration);

int i = 0; 
struct Signal {
  byte paketNo[4];
} data;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
	
  delay(200);
  lora.begin();
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  LoraE32Ayarlar();
  delay(200);
}

void loop() {

  *(int*)data.paketNo = i;
  i++;
  ResponseStatus rs = lora.sendFixedMessage(highByte(GonderilecekAdres), lowByte(GonderilecekAdres), Kanal, &data, sizeof(Signal));
  Serial.print("Paket No: "); Serial.print(*(int*)data.paketNo); Serial.print("\t"); Serial.println(rs.getResponseDescription());
  delay(50);
}
 
void LoraE32Ayarlar() {
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  Serial.println("Configuration Started!");

  ResponseStructContainer c = lora.getConfiguration();
  Configuration configuration;
  memcpy(&configuration, c.data, sizeof(Configuration));
  
  configuration.ADDL = lowByte(Adres);
  configuration.ADDH = highByte(Adres);
  configuration.CHAN = Kanal;

  configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;
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
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  Serial.println("Configuration Ended!");
}

void printParameters(struct Configuration configuration) {
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);

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

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

