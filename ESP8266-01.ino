#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Cambie las credenciales a continuación, para que su ESP8266 se conecte a su router
const char* ssid = "REEMPLÁZAME POR EL NOMBRE DE RED WIFI";
const char* password = "REEMPLAZAME POR LA CONTRASEÑA DEL WIFI";

// Cambie la variable a su dirección IP de Raspberry Pi, para que se conecte a su broker MQTT
const char* mqtt_server = "REEMPLÁZAME POR LA DIRECCIÓN IP DE LA RASPBERRY PI";

WiFiClient espClient;
PubSubClient client(espClient);

// Inicializando las variables para cada led y GPIO correspondiente a la ESP8266-01
const int LEDGPIO0 = 0;
const int LEDGPIO1 = 1;
const int LEDGPIO2 = 2;
const int LEDGPIO3 = 3;

// No cambie la función a continuación. Esta función conecta su ESP8266-01 a su router
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

/* Esta función se ejecuta cuando algún dispositivo publica un mensaje sobre un tema al que está suscrito su ESP8266-01.
 Esta funcion tiene por defecto el programa correspondiente para controlar 4 LEDs.
 Opcionalmente cambie la función a continuación para agregar lógica a su programa, de modo que cuando un dispositivo publique un mensaje sobre un tema que
 tu ESP8266 está suscrito, puedes hacer algo */
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

   if(topic=="casa/habitacion/LED"){
      Serial.print("Changing GPIO 0 to ");
      if(messageTemp == "1"){
        digitalWrite(LEDGPIO0, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "0"){
        digitalWrite(LEDGPIO0, LOW);
        Serial.print("Off");
      }
  }
     if(topic=="casa/comedor/LED"){
      Serial.print("Changing GPIO 1 to ");
      if(messageTemp == "1"){
        digitalWrite(LEDGPIO1, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "0"){
        digitalWrite(LEDGPIO1, LOW);
        Serial.print("Off");
      }
  }
  if(topic=="casa/cocina/LED"){
      Serial.print("Changing GPIO 2 to ");
      if(messageTemp == "1"){
        digitalWrite(LEDGPIO2, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "0"){
        digitalWrite(LEDGPIO2, LOW);
        Serial.print("Off");
      }
  }
     if(topic=="casa/sala/LED"){
      Serial.print("Changing GPIO 3 to ");
      if(messageTemp == "1"){
        digitalWrite(LEDGPIO3, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "0"){
        digitalWrite(LEDGPIO3, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}
// Esta función vuelve a conectar su ESP8266 a su broker MQTT
// Cambie la función a continuación si desea suscribirse a más topicos con su ESP8266-01
void reconnect() {
  // Bucle hasta que nos volvamos a conectar
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Intenta conectar
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      //Se suscribe a diferentes topicos
      client.subscribe("casa/habitacion/LED");
      client.subscribe("casa/comedor/LED");  
      client.subscribe("casa/cocina/LED");  
      client.subscribe("casa/sala/LED");
    } else {
      Serial.print("faiLED, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// La función de configuración establece la configuracion de los GPIOS como salida
// Establece su broker mqtt y establece la función de devolución de llamada (callback)
// La función de devolución de llamada (callback) es el que recibe mensajes y realmente controla los LED
void setup() {
  pinMode(LEDGPIO0, OUTPUT);
  pinMode(LEDGPIO1, OUTPUT);
  pinMode(LEDGPIO2, OUTPUT);
  pinMode(LEDGPIO3, OUTPUT);
  //La siguiente linea te mostrara los mensajes de conexión pero te inhabilitara los pines 1 y 3
  //Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

//Se asegura de que la esp8266-01 este conectada al broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
