#include <WiFi.h>
#include <PubSubClient.h>

// CONFIGURAÇÃO WI-FI
const char* ssid = "NOME_DA_REDE";
const char* senha = "SENHA_DA_REDE";


// CONFIGURAÇÃO MQTT
const char* broker = "test.mosquitto.org";
const int porta = 1883;

// Tópico de teste
const char* topico = "senai/teste/esp32_01";

// Cliente TCP
WiFiClient espClient;

// Cliente MQTT
PubSubClient mqtt(espClient);


// CONECTAR AO WI-FI
void conectarWiFi() {

  Serial.print("Conectando ao WiFi");

  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi conectado!");

  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
}


// CONECTAR AO MQTT
void conectarMQTT() {

  while (!mqtt.connected()) {

    // Cria um Client ID exclusivo
    String clientId = "ESP32_TESTE_";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    Serial.println();
    Serial.print("Conectando ao broker: ");
    Serial.println(broker);

    Serial.print("Client ID: ");
    Serial.println(clientId);

    if (mqtt.connect(clientId.c_str())) {

      Serial.println("MQTT CONECTADO!");

    } else {

      Serial.print("Falha MQTT. Codigo: ");
      Serial.println(mqtt.state());

      delay(2000);
    }
  }
}



// SETUP
void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println(" TESTE MQTT - ESP32");

  // Wi-Fi
  conectarWiFi();

  // Configura broker
  mqtt.setServer(broker, porta);

  // MQTT
  conectarMQTT();
}


void loop() {

  // Verifica Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi desconectado!");

    conectarWiFi();
  }


  // Verifica MQTT
  if (!mqtt.connected()) {

    Serial.println("MQTT desconectado!");

    conectarMQTT();
  }


  // Mantém MQTT funcionando
  mqtt.loop();


  // PUBLICAÇÃO DE TESTE

  bool resultado = mqtt.publish(
    topico,
    "30"
  );


  if (resultado) {

    Serial.println("PUBLICADO COM SUCESSO");

  } else {

    Serial.println("FALHA NA PUBLICACAO");

    Serial.print("Estado MQTT: ");
    Serial.println(mqtt.state());
  }


  Serial.print("Topico: ");
  Serial.println(topico);

  Serial.println("Mensagem: 30");

  delay(5000);
}