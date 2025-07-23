#include <WiFi.h> // Padrão do Esp32
#include <PubSubClient.h> // Necessário para fazer comunicação broker
#include <ArduinoJson.h> // Necessário para manipular JSON
#include <DHT.h> // Necessário para manipular o sensor DHT

// Configurar a conexão com Wifi
const char *SSID = "AAPM-510";
const char *PASSWORD = "aapmSenai#";

// Configurações do Broker MQTT
const char *MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;

// Criando as instâncias (objetos)
WiFiClient espClient;
PubSubClient client(espClient);

//Tópicos que vai escutar
const char *MQTT_TOPICO_TEMPERATURA = "senai928/temperatura";

const int DHT_PIN = 19; // Pino do sensor DHT
const int DHT_TYPE = DHT11; // Tipo do sensor DHT
DHT dht(DHT_PIN, DHT_TYPE); // Instância do sensor DHT

void setup()
{
  Serial.begin(115200);
  conectarWifi();
  conectarMQTT();
  dht.begin(); // Inicializa o sensor DHT

}

void loop()
{
  if (!client.connected())
  {
    Serial.println("MQTT desconectado");
    conectarMQTT();
  }

  client.loop();
  enviarTemperatura();
  delay(5000);
}

void conectarWifi()
{
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Conectando no Wifi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  String ip = " " + WiFi.localIP().toString();
  Serial.println("Conectado WiFi");
  Serial.println(ip.c_str());
}

void conectarMQTT()
{
  // Define o servidor do MQTT que vai receber as mensagens
  client.setServer(MQTT_HOST, MQTT_PORT);


  while (!client.connected())
  {
    // Cria um identificador único para a conexão
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str()))
    {
      Serial.println("Conectado ao Broker :)");
      Serial.println(clientId);
    }
    else
    {
      Serial.println("Erro ao conectar no Broker :(");
      String mqttError = "Codigo do erro: " + String(client.state());
      Serial.println(mqttError.c_str());
    }
  }
}


void enviarTemperatura(){
    float temperatura = dht.readTemperature(); // Lê a temperatura do sensor DHT
    float umidade = dht.readHumidity(); // Lê a umidade do sensor DHT

    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha ao ler o sensor DHT");
        return;
    }
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    Serial.print("Umidade: ");
    Serial.println(umidade);
    
    // Cria um objeto JSON
    JsonDocument doc;
    doc["temperatura"] = temperatura;
    doc["umidade"] = umidade;
    
    // Serializa o objeto JSON para uma string
    char buffer[200];
    size_t tamanho = serializeJson(doc, buffer);

    // Publica a string JSON no tópico MQTT
    client.publish(MQTT_TOPICO_TEMPERATURA, buffer, tamanho);

    Serial.print("Temperatura enviada: ");
    Serial.println(buffer);
}