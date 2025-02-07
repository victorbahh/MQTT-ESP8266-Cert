# ESP8266 MQTT with Mutual Certificate Authentication

Este repositório contém a implementação de um cliente MQTT seguro utilizando a plataforma ESP8266, com autenticação mútua baseada em certificados X.509. O cliente se conecta a um broker MQTT sobre TLS/SSL, publica dados de umidade e temperatura aleatórios e subscreve tópicos para receber mensagens.

## Funcionalidades

- **Conexão segura via TLS/SSL**: Utiliza certificados X.509 para autenticação do cliente e do servidor, garantindo a segurança da comunicação.
- **Publicação de dados em JSON**: O cliente publica dados de umidade e temperatura simulados em formato JSON a cada 5 segundos no tópico `esp8266/pub`.
- **Subscrição de tópicos**: O cliente subscreve o tópico `esp8266/sub` para receber mensagens do broker.
- **Configuração automática de horário**: Utiliza SNTP para configurar a hora do dispositivo a partir de servidores de tempo públicos.
  
## Requisitos

- **Placa ESP8266**: Compatível com a biblioteca `ESP8266WiFi`.
- **Bibliotecas necessárias**:
  - `ESP8266WiFi.h`: Para conexão Wi-Fi com o ESP8266.
  - `WiFiClientSecure.h`: Para comunicação segura via TLS/SSL.
  - `PubSubClient.h`: Para comunicação MQTT.
  - `ArduinoJson.h`: Para manipulação de dados JSON.

## Como Usar

1. **Configuração do Wi-Fi**:
   - Substitua as credenciais da sua rede Wi-Fi nas variáveis `WIFI_SSID` e `WIFI_PASSWORD`.

2. **Configuração dos Certificados**:
   - Substitua os valores das variáveis `cacert`, `client_cert` e `privkey` pelos seus próprios certificados e chave privada, conforme explicado no [tutorial](https://medium.com/@kajsuaning/mqtt-mutual-certificate-authentication-f51bc6e1a457).
   - **Certificados autoassinados**: O código funciona com certificados autoassinados. <br>
**Nota:**
Caso não queira verificar o certificado do servidor (não recomendado para produção), você pode descomentar [essa](https://github.com/victorbahh/MQTT-ESP8266-Cert/blob/0668f8396be06bf726741f27396df915c9da8859/main.ino#L105) linha.


3. **Configuração do Broker MQTT**:
   - Substitua o valor de `MQTT_HOST` pelo endereço do seu broker MQTT.
   - Certifique-se de que o broker está configurado para usar a autenticação mútua com certificados. Para mais detalhes sobre a configuração do broker, consulte o [tutorial completo](https://medium.com/@kajsuaning/mqtt-mutual-certificate-authentication-f51bc6e1a457).

4. **Testando com MQTT Explorer**:
   - O código pode ser testado utilizando ferramentas como o **[MQTT Explorer](https://mqtt-explorer.com/)**. Configure o MQTT Explorer para se conectar ao seu broker e inscreva-se nos tópicos `esp8266/pub` e `esp8266/sub` para ver a comunicação em tempo real.

5. **Compilação e Upload**:
   - Carregue o código na sua placa ESP8266 usando o Arduino IDE ou PlatformIO.
   - Conecte o dispositivo à sua rede Wi-Fi e aguarde até que ele se conecte ao broker MQTT.

6. **Publicação de Dados**:
   - A cada 5 segundos, o dispositivo publica dados de umidade e temperatura aleatórios no tópico `esp8266/pub`. Esses dados são formatados como um objeto JSON.

## Licença

Este projeto está licenciado sob a [Licença Apache 2.0](LICENSE).

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues ou pull requests. Se você encontrar problemas ou tiver sugestões, fique à vontade para nos avisar.

## Referências

- [Tutorial para configuração do broker MQTT com autenticação mútua](https://medium.com/@kajsuaning/mqtt-mutual-certificate-authentication-f51bc6e1a457)
- [Tutorial para conectar a esp8266 com a AWS core](https://github.com/rch-goldsnaker/aws-esp8266/blob/main/main.ino)

