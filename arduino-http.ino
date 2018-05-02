#define TEST_SENSORS

//ESTO ES NECESARIO PORQUE CUANDO SE LLAMA LA LIBRERIA DESDE EL VENDOR NO AUTOCARGA EL CPP
#include "vendor/igniteit/arduino-basic-oo/JsonHelper.cpp"
#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.cpp"

#include "HttpServer.h"
#include "HttpClientAr.h"

HttpServer serverEt(80);
EthernetClient clientEt;

HttpWiFiServer serverWi(80);
WiFiEspClient clientWi;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

