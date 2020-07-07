#include <Arduino.h>
#include <EMailSender.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <SimpleTimer.h>

bool debugSerial = 1;
bool debugLCD = 1;

const int analogInPin = A0;
float sensorValue = 0;
float volt;
int i = 0;
int clean_status = 0;

const char *ssid = "Wifi-Roboto";
const char *password = "arDY1234";
String url = "http://192.168.1.4/turbidity/save.php?x=";

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

LiquidCrystal_I2C lcd(0x27, 16, 2);
EMailSender emailSend("ianputrapratama809@gmail.com", "kenponx1976");

SimpleTimer timer;

uint8_t WiFiConnect(const char *nSSID = nullptr, const char *nPassword = nullptr)
{
  static uint16_t attempt = 0;
  serial_show("Connecting to ", 0);
  lcd_show(1, "Connecting to", 0, 0, String(nSSID), 0, 1, 1000);
  if (nSSID)
  {
    WiFi.begin(nSSID, nPassword);
    serial_show(nSSID, 0);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50)
  {
    delay(200);
    serial_show(".", 0);
  }
  ++attempt;
  if (i == 51)
  {
    serial_show("\nConnection: TIMEOUT on attempt: ", 0);
    lcd_show(1, "Connecting:", 0, 0, "Timeout", 0, 1, 1000);
    serial_show(String(attempt), 1);
    if (attempt % 2 == 0)
      serial_show("Check if access point available or SSID and Password", 1);
    return false;
  }

  serial_show("\nConnected", 1);
  serial_show("IP Address: ", 0);
  String ip_address = WiFi.localIP().toString();
  serial_show(ip_address, 1);
  lcd_show(1, "Connected", 0, 0, ip_address, 0, 1, 1000);
  return true;
}

void Awaits()
{
  uint32_t ts = millis();
  while (!connection_state)
  {
    delay(50);
    if (millis() > (ts + reconnect_interval) && !connection_state)
    {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void setup()
{
  Serial.begin(115200);
  lcd.init();
  delay(250);

  serial_show("Turbidity WeMoS D1 Mini", 1);
  lcd_show(1, "Turbidity", 0, 0, "WeMoS D1 Mini", 0, 1, 1000);

  connection_state = WiFiConnect(ssid, password);
  if (!connection_state)
    Awaits();

  timer.setInterval(600000, send_http); // 30000ms = 30s; 10min = 600s = 600000ms
}

void loop()
{
  timer.run();
  read_analog();
  serial_show("T:" + String(sensorValue) + "NTU", 1);
  lcd_show(1, "S:Running", 0, 0, "T:" + String(sensorValue) + "NTU", 0, 1, 1000);

  if (sensorValue <= 20 && (clean_status == 1 || clean_status == 2)) // air bersih
  {
    clean_status = 0;
  }
  if (sensorValue > 20 && sensorValue <= 25 && clean_status == 0) // air cukup keruh
  {
    send_email("cukup keruh");
    clean_status = 1;
  }
  if (sensorValue > 25 && (clean_status == 0 || clean_status == 1)) // air sangat keruh
  {
    send_email("sangat keruh");
    clean_status = 2;
  }

  //read_analog();
  //serial_show("T:" + String(sensorValue) + "NTU", 1);
  //lcd_show(1, "S:Running", 0, 0, "T:" + String(sensorValue) + "NTU", 0, 1, 1000);
  //lcd_show(1, "V:" + String(volt) + "V", 0, 0, "T:" + String(sensorValue) + "NTU", 0, 1, 1000);*/
}

void lcd_show(int clear, String text1, int x1, int y1, String text2, int x2, int y2, int waitms)
{
  if (debugLCD == 1)
  {
    if (clear == 1)
    {
      lcd.clear();
    }
    lcd.backlight();
    lcd.setCursor(x1, y1);
    lcd.print(text1);
    lcd.setCursor(x2, y2);
    lcd.print(text2);
    delay(waitms);
  }
}

void serial_show(String text, int newline)
{
  if (debugSerial == 1)
  {
    if (newline == 1)
    {
      Serial.println(text);
    }
    else
    {
      Serial.print(text);
    }
  }
}

void read_analog()
{
  float totalValue = 0.0;
  volt = 0.0;
  sensorValue = 0.0;
  totalValue = 0.0;
  for (int i = 0; i < 500; i++)
  {
    sensorValue = analogRead(analogInPin) / 162.0734;
    totalValue += sensorValue;
  }

  volt = totalValue / 500;
  if (volt <= 3.15)
  {
    volt = 3.15;
  }
  if (volt >= 3.48)
  {
    volt = 3.48;
  }
  sensorValue = 3.48 - volt;
  sensorValue = (sensorValue / 0.33 * 995) + 5;
  //sensorValue = volt;
}

void send_http()
{
  String x;

  x = String(sensorValue);
  url = url + x;

  if (WiFi.status() == WL_CONNECTED)
  {
    serial_show("Send to Server", 1);
    lcd_show(1, "Send to Server", 0, 0, "", 0, 1, 1000);
    HTTPClient http;

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0)
    {
      String payload = http.getString();
      serial_show(payload, 1);
      lcd_show(1, payload, 0, 0, "", 0, 1, 1000);
    }

    http.end();
  }
}

void send_email(int status)
{
  EMailSender::EMailMessage message;
  if (status == "cukup keruh")
  {
    message.subject = "[Notifikasi] Air Aquarium Cukup Keruh";
    message.message = "Air dalam aquarium cukup keruh, silahkan tambahkan disinfektan air agar kualitas air membaik";
  }
  else
  {
    message.subject = "[Notifikasi] Air Aquarium Sangat Keruh";
    message.message = "Air dalam aquarium sangat keruh, silahkan ganti air aquarium dengan air baru";
  }

  EMailSender::Response resp = emailSend.send("2black0@gmail.com", message);

  String status_email;
  if (String(resp.status) == "1")
  {
    status_email = "OK";
  }
  else
  {
    status_email = "Not OK";
  }

  serial_show("S:Send Email ", 0);
  serial_show("Status:" + status_email, 1);
  lcd_show(1, "S:Email", 0, 0, "Status:" + status_email, 0, 1, 1000);

  //String response_status = String(resp.status);
  //String response_code = String(resp.code);
  //String response_desc = String(resp.desc);
  //serial_show("Response Status:" + response_status, 1);
  //serial_show("Response Code:" + response_code, 1);
  //serial_show("Response Desc:" + response_desc, 1);
  //lcd_show(1, response_status, 0, 0, response_code, 0, 1, 1000);
}