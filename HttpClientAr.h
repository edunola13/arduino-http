#ifndef HttpClientAr_h
#define HttpClientAr_h
//#include "WConstants.h"
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"

class HttpResponse{
  protected:
    String code;
    String body;

  public: 
    HttpResponse();
  
	void setVarsHeader(String &header);
    String getCode();
    String getBody();
	void setBody(String body);
};

class HttpClientAr: public EthernetClient{  
  public:
    HttpClientAr();
    HttpResponse httpResponse();

    String readHeader();    
    String readBody();

    void sendRequest(String method, String uri);
    void sendRequest(String method, String uri, String contentType);
    void sendRequest(String method, String uri, String contentType, String &body);
    void sendBody(String &body);
};

#endif

