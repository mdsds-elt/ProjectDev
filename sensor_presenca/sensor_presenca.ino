//#include <WiFiClientSecure.h>
#include "CTBot.h"


#define PIR   27

CTBot myBot;

bool pir_presence_flag = false;

void msgTelegramBot(void);

String ssid  = "ICOMP_1A";
String pass  = "*******";


String token = "5774484856:AAEwZxVarSmfIEqjUlYHd3GdOBbJ2xAVqns";

void IRAM_ATTR ISR_0() {
  pir_presence_flag = true;
}
//SetUp
void setup() {
  pinMode(PIR, INPUT_PULLDOWN);
  attachInterrupt(PIR, ISR_0, RISING);
  Serial.begin(115200);
  Serial.println("Iniciando TelegramBot...");
  
  //Inicia conexão com WiFi
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  //Testa conexão com o Bot
  if (myBot.testConnection())
    Serial.println("\nTeste de Conexão OK");
  else
    Serial.println("\nTeste de Conexão NOK");
  delay(500);
  //Inicia
  msgTelegramBot();
}

void loop() {
  //Nada a fazer
}

bool reconnectTelgramBot() {
  while (!myBot.testConnection()) {
    Serial.println("");
    //Tenta conexão com WiFi
    if (myBot.wifiConnect(ssid, pass)) {
      Serial.println("Conexão estabelecida");
    }
    else {
      Serial.println("");
    }
  }
  return true;
}

void msgTelegramBot() {
  TBMessage msg;
  while (1) {

    if (CTBotMessageText == myBot.getNewMessage(msg)) {

      myBot.sendMessage(msg.sender.id, msg.text);
    
      while (1) {
        //Movimento detectado
        if (pir_presence_flag) {
          Serial.println("Interrupcao");//Sinaliza que ocorreu a interrupção
          //Reseta o valor da Flag
          pir_presence_flag = false;
          if (!myBot.sendMessage(msg.sender.id, "Presença Detectada!")) {
            //Falha no envio da mensagem
            Serial.println("Falha no envio da mensagem.");
            while (!reconnectTelgramBot()) {
              delay(10);
            }
          
          delay(10000);
        }
        delay(10);
      }
    }
    delay(10);
  }
}
