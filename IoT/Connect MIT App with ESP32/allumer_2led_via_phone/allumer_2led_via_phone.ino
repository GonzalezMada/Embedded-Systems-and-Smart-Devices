// Allumer et éteindre des LED à l'aide de boutons via un point d'accès Wi-Fi (SoftAP).

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// Paramètres du réseau Wi-Fi
const char* nomReseau = "Juan";
const char* motDePasse = "123456789";

// Création du serveur Web sur le port 80
WiFiServer serveur(80);

// Définition des broches des LED
#define LED12  12    // LED connectée à la broche 12
#define LED14  14    // LED connectée à la broche 14

String etat = "";
int delaiReconnexion = 30000; // Délai de reconnexion de 30 secondes.

void setup() {
  Serial.begin(115200);

  // Configuration du point d'accès Wi-Fi
  Serial.println();
  Serial.print("Configuration du point d'accès : ");
  Serial.println(nomReseau);

  WiFi.softAP(nomReseau, motDePasse);
  IPAddress adresseIP = WiFi.softAPIP();

  // Affichage de l'adresse IP du serveur Web
  Serial.print("Adresse IP pour accéder au serveur Web : ");
  Serial.print("http://");
  Serial.println(adresseIP);

  // Démarrage du serveur Web
  serveur.begin();
  Serial.println("Serveur Web démarré.");
}

void loop() {

  // Si la connexion est perdue, tenter de réactiver le point d'accès
  // toutes les 30 secondes.
  if ((WiFi.status() != WL_CONNECTED) && (millis() > delaiReconnexion)) {
    Serial.println("Tentative de réactivation du point d'accès Wi-Fi...");
    WiFi.softAP(nomReseau, motDePasse);
    delaiReconnexion = millis() + 30000;
  }

  // Vérification de la connexion d'un nouveau client
  WiFiClient client = serveur.available();

  if (!client) {
    return;
  }

  Serial.print("Nouveau client : ");
  Serial.println(client.remoteIP());

  // Attendre que le client envoie des données
  // while (!client.available()) {
  //   delay(1);
  // }

  /////////////////////////////////////////////////////
  // Lecture des informations envoyées par le client
  String requete = client.readStringUntil('\r');
  Serial.println(requete);

  // Traitement de la requête du client
  if (requete.indexOf("on12") != -1) {
    digitalWrite(LED12, HIGH);
    etat = "LED12 ALLUMEE";
  }

  if (requete.indexOf("off12") != -1) {
    digitalWrite(LED12, LOW);
    etat = "LED12 ETEINTE";
  }

  if (requete.indexOf("on14") != -1) {
    digitalWrite(LED14, HIGH);
    etat = "LED14 ALLUMEE";
  }

  if (requete.indexOf("off14") != -1) {
    digitalWrite(LED14, LOW);
    etat = "LED14 ETEINTE";
  }

  if (requete.indexOf("consulta") != -1) {
    etat = "";

    if (digitalRead(LED12) == HIGH) {
      etat = "LED12 ALLUMEE,";
    } else {
      etat = "LED12 ETEINTE,";
    }

    if (digitalRead(LED14) == HIGH) {
      etat = etat + "LED14 ALLUMEE";
    } else {
      etat = etat + "LED14 ETEINTE";
    }
  }

  //////////////////////////////////////////////
  // Envoi de la réponse de la page Web

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // Ligne vide obligatoire pour séparer l'en-tête du contenu.
  client.println(etat); // Transmission de l'état des LED au client.

  client.flush();
  client.stop();

  Serial.println("Client déconnecté.");
}