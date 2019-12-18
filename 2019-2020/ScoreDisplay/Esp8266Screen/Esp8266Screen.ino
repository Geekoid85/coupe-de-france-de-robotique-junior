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


const char* ssid = "Panneau2018G2";// Panneau2018G2 ou Panneau2018
const char* password = "mogetator2";//mogetator2 ou mogetator

unsigned int localPort = 8000;//port de communication aec l'esp
byte packetBuffer[3024];

String request = ""; //Chaine de caractère pour stocker le message udp recu.
String estimatedScore = "85";
int Button = 4; // Pin ou sont connectés les bouttons en parallèle pour déclancher l'affichage du score estimé (actuellement D2 donc 4)

WiFiUDP Udp;

void setup() {
  WiFi.disconnect();
  WiFi.softAPdisconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.reconnect();

  Serial.begin(9600);
  Serial.println("");
  Serial.println("WiFi Connected");

  Udp.begin(localPort);
  Serial.println("Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.softAPIP()); // Généralement 192.168.4.1
  Serial.println("/");

  dmd.setBrightness(255); // Choix d'une luminosité
  dmd.selectFont(SystemFont5x7);
  dmd.begin(); // A partir de là l'écran est fonctionnel
  dmd.clearScreen(); // Effacer l'écran
  pinMode(Button, INPUT); // Mon bouton est une entrée

  dmd.clearScreen();
  dmd.drawString(0, 0, "00000");
}

void loop() {
Serial.print(Button);
  if (Button == LOW) { // Si j'appuye sur le bouton (pull up) j'affiche le score estimé
    dmd.clearScreen();
    dmd.drawString(0, 0, estimatedScore);
  }
 /* else if (Button == HIGH) {
    dmd.clearScreen();
    dmd.drawString(0, 0, "rezon"); // Pour savoir c'est quel écran
  } */

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
