#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>

const int pinLed = 8;
// Atualizar ultimo valor para ID do seu Kit para evitar duplicatas
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x14 };

// Endereço do Cloud MQTT
char* server = "m12.cloudmqtt.com";

// Valor da porta do servidor MQTT
int port = 11528;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(pinLed, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(pinLed, LOW);  // Turn the LED off by making the voltage HIGH
  }

}
EthernetClient ethClient;
//PubSubClient client(server, port, whenMessageReceived, ethClient);
PubSubClient client(server, port, callback, ethClient);

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}
  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
  } else {
    Serial.println(Ethernet.localIP());
  }

  Serial.println("Connecting...");
  // Conectando com informações de cliente e senha criados através da interface web do serviço
  if (client.connect("arduinoClient", "teste", "123")) {
    Serial.println("Connected");
    client.publish("outTopic ", "até que fim");
    Serial.println("outTopic sent");
    client.subscribe("teste");
  } else {
    Serial.println("Failed to connect to MQTT server");
  }
}

void loop()
{
  // A biblioteca PubSubClient precisa que este método seja chamado em cada iteração de `loop()`
  // para manter a conexão MQTT e processar mensagens recebidas (via a função callback)
  client.loop();
}
