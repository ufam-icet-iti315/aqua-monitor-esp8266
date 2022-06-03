/**
 * Projeto: AquaMonitor (Monitoramento de aquário para ajudá-lo a entender as necessidades dos peixes)
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Conexão Firebase
#define FIREBASE_HOST ""
// Token Firebase
#define FIREBASE_AUTH ""

// conexão de ponto de acesso wifi
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

WiFiClient client;
FirebaseData firebaseData;

// Sensor NTC MF58
const double VCC = 5;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741; 

void setup() {
  Serial.begin(9600);  /* Define baud rate for serial communication */

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting wifi");
  }
  Serial.print("");
  Serial.println("Conectado ao WiFi");
  Serial.println("WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // put your main code here, to run repeatedly:
  double Vout, Rth, temperature, adc_value; 

  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

/*  Steinhart-Hart Thermistor Equation:
 *  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
 *  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin

  temperature = temperature - 273.15;  // Temperature in degree celsius
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" degree celsius");
  delay(500);

}
