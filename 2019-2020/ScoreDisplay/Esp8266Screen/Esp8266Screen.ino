#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <SPI.h>
#include <DMD2.h> /* Library de l'écran freetronics dot matrix display 32*16 https://github.com/freetronics/DMD2
  https://www.freetronics.com.au/products/dot-matrix-display-32x16-red */

#include <fonts/SystemFont5x7.h> // Définition de la police d'écriture et sa taille
#define pin_A 16 // Définition des pins de l'écran sur la wemos d1 mini
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15

#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1
SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display

const char* ssid = "Panneau2018";// Panneau2018G2 ou Panneau2018
const char* password = "mogetator";//mogetator2 ou mogetator

unsigned int localPort = 8000;//port de communication aec l'esp
byte packetBuffer[3024];
String request = ""; //Chaine de caractère pour stocker le message udp recu.
WiFiUDP Udp;

int buttonPin = 4; // Momentary switch input pin, GPIO4 = D2 on the wemos d1 mini
int buttonState; // Store the actually trusted state of the button
int lastButtonState = LOW; // the previous reading from the input pin
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 150;    // the debounce time; increase if the output flickers
String estimatedScore = "85";
String ScreenName = "Tor";

void setup() {
  WiFi.disconnect();
  WiFi.softAPdisconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.reconnect();

  Serial.begin(9600);
  delay(100);
  Serial.println();
  Serial.println("Wifi Access Point Started");

  Udp.begin(localPort);
  Serial.println("UDP Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.softAPIP()); // Généralement 192.168.4.1
  Serial.println("/");

  dmd.setBrightness(255); // Choix d'une luminosité
  dmd.selectFont(SystemFont5x7);
  dmd.begin(); // A partir de là l'écran est fonctionnel
  dmd.clearScreen(); // Effacer l'écran

  Serial.println();
  Serial.println("Screen initialized");
  dmd.clearScreen();
  dmd.drawString(0, 0, ScreenName);
  Serial.println(ScreenName);
  pinMode(buttonPin, INPUT);
  Serial.println();
  Serial.println("Starting Input Button Tracking");
  Serial.println();
  delay(500);
}

void loop() {
  int reading = digitalRead(buttonPin); // Read and affect the none trusted state of the GPIO linked to buttonPin
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        Serial.print("The trusted button state is actually: ");
        Serial.println(buttonState); // Print the button state
        Serial.print("And so the displayed estimated score is: ");
        Serial.println(estimatedScore);
        Serial.println();
        dmd.clearScreen();
        dmd.drawString(0, 0, estimatedScore);
      }
    }
  }
  lastButtonState = reading;



  int noBytes = Udp.parsePacket(); // noByte contient un chiffre correspondant à la taille du packet udp recu
  request = ""; // Effacer le précédent message recu
  if (!noBytes) {
    return; // Si il est vide continuer d'attendre, sinon passer à la suite
  }

  Udp.read(packetBuffer, noBytes); // lecture du message udp, affectation au char packetBuffer. noBytes défini la taille max du message

  for (int i = 1; i <= noBytes; i++) // Convertir le message recu en chaine de caractères
  {
    request = request + char(packetBuffer[i - 1]);
  }
  Serial.println(request);
  dmd.clearScreen(); // Effacer l'écran
  dmd.drawString(0, 0, request); // Puis afficher le message recu
}
