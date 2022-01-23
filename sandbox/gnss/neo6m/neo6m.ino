#include <Arduino_AVRSTL.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <ctime>

constexpr int RX = 3;
constexpr int TX = 4;

unsigned long   timer = millis();
SoftwareSerial  serial(TX, RX);
Adafruit_GPS    gps(&serial);

std::string get_time(const Adafruit_GPS &gps)
{
    tm local;
    local.tm_sec = gps.seconds;   
    local.tm_min = gps.minute;   
    local.tm_hour = gps.hour;   
    local.tm_mon = gps.month;
    local.tm_year = gps.year;

    char buffer[32];
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", &local);
    return std::string(buffer); 
}  

void setup() {
  Serial.begin(115200);
  gps.begin(9600);
}


void loop() {
  gps.read();
  if (gps.newNMEAreceived()) {
    if (!gps.parse(gps.lastNMEA()))
      return;
  }
  
  if (millis() - timer > 3000) {
    timer = millis();
    Serial.println(get_time(gps).c_str());
  }
}
