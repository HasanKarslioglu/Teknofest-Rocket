
//Lora Receiver
#define E32_TTL_1W
#include "LoRa_E32.h" 
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(3, 4); //PCB versiyon 4.3 den sonra bu şekilde olmalı
LoRa_E32 lora(&mySerial);
 
#define M0 7
#define M1 6
 
//PARAMETRE AYARLARI
#define Adres 2   //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerden FARKLI olmalı
#define Kanal 20  //Frekans değeri Diğer Modüllerle AYNI olmalı.
//E32 için 0--31 arasında bir sayı giriniz.
//E32 433 için Frekans = 410 + kanal değeri
  
int i = 0; 
struct Signal {
  byte mesaj[4];
} data;
 
void setup() {
  Serial.begin(9600);
  while (!Serial) {
	    ; // wait for serial port to connect. Needed for native USB
    }
	delay(100);

  lora.begin();
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  LoraE32Ayarlar();
  delay(500);
}
 
void loop() {
  while (lora.available() > 1) {
 
    ResponseStructContainer rsc = lora.receiveMessage(sizeof(Signal));
    struct Signal data = *(Signal*)rsc.data;
    rsc.close();
    Serial.println(*(int*)data.mesaj);
  }
}
 
void LoraE32Ayarlar() {
  
  Serial.println("Lora setup -> begin");
  
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  Serial.println("M0 and M1 HIGH");

  ResponseStructContainer c;
  c = lora.getConfiguration();
  Configuration configuration = *(Configuration*)c.data;
 
  //DEĞİŞEBİLEN AYARLAR
  // Üstte #define kısmında ayarlayınız
  configuration.ADDL = lowByte(Adres);
  configuration.ADDH = highByte(Adres);
  configuration.CHAN = Kanal;
 
  //SEÇENEKLİ AYARLAR
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;  //Veri Gönderim Hızı 2,4 varsayılan
  //configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;  //Veri Gönderim Hızı 0,3 En uzak Mesafe
  //configuration.SPED.airDataRate = AIR_DATA_RATE_101_192; //Veri Gönderim Hızı 19,2 En Hızlı
 
 
  //configuration.OPTION.transmissionPower = POWER_20;  //Geönderim Gücü max Varsayılan
  //configuration.OPTION.transmissionPower = POWER_10;  //Geönderim Gücü min
  configuration.OPTION.transmissionPower = POWER_30; // E32 30d modülleri için bunu aktif et
 
  //GELİŞMİŞ AYARLAR
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;
  configuration.OPTION.fec = FEC_0_OFF; 
  //configuration.OPTION.fec = FEC_1_ON;
  configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  //configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
  configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
  configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
   
  Serial.println("Parametre Ayarları Yapıldı");
  Serial.print("Adres ADDL: ");
  Serial.println(configuration.ADDL);
  Serial.print("Adres ADDH: ");
  Serial.println(configuration.ADDH);
  Serial.print("Kanal: ");
  Serial.println( configuration.CHAN);

  // Ayarları KAYDET ve SAKLA
  ResponseStatus rs = lora.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  Serial.println(rs.getResponseDescription());
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  printParameters(configuration);

  Serial.println("Lora setup -> end!");
  c.close();
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


