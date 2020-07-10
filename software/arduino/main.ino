// library yang dipakai
#include <Arduino.h>
#include <EMailSender.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <SimpleTimer.h>

// waktu pengiriman ke server 60000ms = 60s = 1m
#define timersend 60000

// ganti sesuai yang dipakai
const char *ssid = "Wifi-Roboto";                                     // ganti wifi
const char *password = "arDY1234";                                    // ganti password
String url = "http://192.168.1.4/turbidity/save.php?x=";              // ganti ip komputer
EMailSender emailSend("ianputrapratama809@gmail.com", "kenponx1976"); // ganti email sama password

// variable yang dipakai
bool debugSerial = 1;
bool debugLCD = 1;
const int analogInPin = A0;
float sensorValue = 0;
int i = 0;
int clean_status = 0;
uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

// definisi instanse
LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleTimer timer;

// fungsi connect dgn wifi
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

// fungsi menunggu koneksi wifi
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

// fungsi setup
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

  timer.setInterval(timersend, send_http);
}

// fungsi utama
void loop()
{
  timer.run();
  read_analog();
  serial_show("T:" + String(sensorValue) + "NTU", 1);
  lcd_show(1, "S:Running", 0, 0, "T:" + String(sensorValue) + "NTU", 0, 1, 1000);

  if (sensorValue <= 20 && (clean_status == 1 || clean_status == 2)) // kondisi air bersih <=20
  {
    clean_status = 0;
  }
  if (sensorValue > 20 && sensorValue <= 25 && clean_status == 0) // kondisi air cukup keruh >20 dan <=25
  {
    send_http();
    send_email(1);
    clean_status = 1;
  }
  if (sensorValue > 25 && (clean_status == 0 || clean_status == 1)) // kondisi air sangat keruh >25
  {
    send_http();
    send_email(2);
    clean_status = 2;
  }
}

// fungsi untuk menampilkan ke lcd
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

// fungsi untuk mengirim data serial
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

// fungsi untuk baca data analog
void read_analog()
{
  float totalValue = 0;
  float value;
  sensorValue = 0;

  for (int i = 0; i < 2500; i++)
  {
    value = analogRead(analogInPin);
    totalValue += value;
    value = 0;
  }

  sensorValue = totalValue / 2500;
  sensorValue = 500 - ((sensorValue - 250) * 1.596774);
  if (sensorValue <= 5)
  {
    sensorValue = 5;
  }
  else if (sensorValue >= 500)
  {
    sensorValue = 500;
  }
  else
  {
    sensorValue = sensorValue;
  }
}

// fungsi untuk kirim ke database
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

// fungsi untuk kirim email
void send_email(int status)
{
  EMailSender::EMailMessage message;
  if (status == 1)
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
}