
#ifndef HttpServer_h
#define HttpServer_h
//#include "WConstants.h"
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
//#include <WiFiEspUdp.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"

class HttpRequest{
  protected:
    char method[7];
    char url[15];
    char params[20];
    String body;

  public: 
	HttpRequest();
  
	void setVarsHeader(String &header);
    String getMethod();
    String getUrl();
    char* getParams();
    String getParam(String name);
    String getBody();
	String* getPunteroBody();
	void setBody(String body);
};

struct Route {
  char url[15];
  void (* functionPointer)(HttpRequest&);
  uint8_t type;
  uint8_t cant;
};

class HttpServer: public EthernetServer{  
  public:
    HttpServer(int port);
    void beginServer(uint8_t *mac);
	void beginServer(uint8_t *mac, IPAddress local_ip);
    void beginServer(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

    /*void setSecurity(bool security, String key){
      this->security= security;
      this->key= key;
    }*/

    HttpRequest httpRequest(EthernetClient &client);

    String readHeader(EthernetClient &client);    
    String readBody(EthernetClient &client);
    
    bool send(EthernetClient &client, int code, String contentType, String &body);

    bool sendApiRest(EthernetClient &client, int code, String body);
    bool partialSendApiRest(EthernetClient &client, String body);
    bool sendApiRestEncode(EthernetClient &client, int code, JsonInterface &element);
};

class HttpWiFiServer: public WiFiEspServer{  
  public:
    HttpWiFiServer(uint16_t port);

    HttpRequest httpRequest(WiFiEspClient &client);

    String readHeader(WiFiEspClient &client);    
    String readBody(WiFiEspClient &client);
    
    bool send(WiFiEspClient &client, int code, String contentType, String &body);

    bool sendApiRest(WiFiEspClient &client, int code, String body);
    bool partialSendApiRest(WiFiEspClient &client, String body);
    bool sendApiRestEncode(WiFiEspClient &client, int code, JsonInterface &element);
};


#endif

