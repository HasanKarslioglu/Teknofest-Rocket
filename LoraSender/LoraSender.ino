
//Lora Sender

#include "LoRa_E32.h" 
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(3, 4); //PCB versiyon 4.3 den sonra bu şekilde olmalı
LoRa_E32 lora(&mySerial);
 
#define M0 7
#define M1 6
 
//PARAMETRE AYARLARI
#define Adres 1   //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerden FARKLI olmalı
#define Kanal 20  //Frekans değeri Diğer Modüllerle AYNI olmalı.
//E32 için 0--31 arasında bir sayı giriniz.
//E32 433 için Frekans = 410 + kanal değeri
 
#define GonderilecekAdres 2 //Mesajın gönderileceği LoRa nın adresi
 
struct Signal {
  char text[15];
  char konum[15];
  byte intSayi[4];
  byte floatSayi[10];
  bool btn1;
};

Signal data;
 
 
void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  Serial.println("M0 and M1 Output");

  Serial.begin(9600);
  lora.begin();
  Serial.println("Serial -> begin, lora -> begin");
 
  LoraE32Ayarlar();
 
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  Serial.println("M0 and M1 Low");
 
  delay(500);
  
}
 
void loop() {
 
  data.btn1 = false;
  strcpy(data.text, "Teknofest");
  *(int*)(data.intSayi) = 1453;
  *(float*)(data.floatSayi) = 14.1415;
  strcpy(data.konum, "ML102");
 
  ResponseStatus rs = lora.sendFixedMessage(highByte(GonderilecekAdres), lowByte(GonderilecekAdres), Kanal, &data, sizeof(Signal));
  Serial.println(rs.getResponseDescription());
  delay(2000);
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
 
 
  configuration.OPTION.transmissionPower = POWER_20;  //Geönderim Gücü max Varsayılan
  //configuration.OPTION.transmissionPower = POWER_10;  //Geönderim Gücü min
  //configuration.OPTION.transmissionPower = POWER_30; // E32 30d modülleri için bunu aktif et
 
  //GELİŞMİŞ AYARLAR
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;
  configuration.OPTION.fec = FEC_0_OFF;
  //configuration.OPTION.fec = FEC_1_ON;
  configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  //configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
  configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
  configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
 
  // Ayarları KAYDET ve SAKLA
  ResponseStatus rs = lora.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}