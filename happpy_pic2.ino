//เรียกห้องสมุด
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//เปิดขา-ตัวแปร
Adafruit_AM2315 am2315;

const int sensor_pin = A0;
#define Relay 2

//OLED
//OLED define
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("AM2315 Test!");

  if (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }
  pinMode(Relay, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display(); //Display logo
  //   delay(dht.getMinimumSamplingPeriod());
  //  delay(1000);
  display.clearDisplay();
}

float temperature, humidity;
float moisture_percentage;

void loop() {

  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    return;
  }
  Serial.print("Temp *C: "); Serial.print(temperature);
  Serial.print("  Hum %: "); Serial.println(humidity);

  moisture_percentage = ( 100.00 - ( (analogRead(0) / 1023.00) * 100.00 ) );
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  if (temperature >= 32)
  {
    digitalWrite(Relay, HIGH);
    delay(2000);
  } if (moisture_percentage >= 70 or temperature <= 27) {
    digitalWrite(Relay, LOW);
    delay(2000);
  }

  //
  display.clearDisplay();
  oledDisplayHeader();

  oledDisplay(3, 5, 28, humidity, "%");
  oledDisplay(2, 70, 16, temperature, "C");
  oledDisplay(2, 70, 44, moisture_percentage, "%");

  display.display();
}
void oledDisplayHeader() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Humidity");
  display.setCursor(60, 0);
  display.print("Temperature");
}
void oledDisplay(int size, int x, int y, float value, String unit) {
  int charLen = 12;
  int xo = x + charLen * 3.2;
  int xunit = x + charLen * 3.6;
  int xval = x;
  display.setTextSize(size);
  display.setTextColor(WHITE);

  if (unit == "%")
  {
    display.setCursor(x, y);
    display.print(value, 0);
    display.print(unit);
  } else
  {
    if (value > 99)
    {
      xval = x;
    } else
    {
      xval = x + charLen;
    }
    display.setCursor(xval, y);
    display.print(value, 0);
    display.drawCircle(xo, y + 2, 2, WHITE); // print degree symbols (  )
    display.setCursor(xunit, y);
    display.print(unit);
  }

}
