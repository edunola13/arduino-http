//#include "WConstants.h"
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"
#include "HttpServer.h"
//#include <MemoryFree.h> Serial.println(freeMemory());

//
//HTTP REQUEST
HttpRequest::HttpRequest(){
}
void HttpRequest::setVarsHeader(String &header){
  //Me quedo con solo la primer linea del header - por el momento el header solo incluye esa primer linea
  //int i= header.indexOf("\n");
  //header= header.substring(0, i);
  //METHOD
  int i= header.indexOf(" ");
  header.substring(0, i).toCharArray(this->method, 7);
  //header.substring(0, i).toCharArray(this->method, 8);
  //Me adelanto hasta la url
  header= header.substring(i + 1);  
  i= header.indexOf("?");
  if(i != -1){
    //Si tiene parametros
    //URL
    i= header.indexOf("?");
    header.substring(0, i).toCharArray(this->url, 15);
    //PARAMS
    header= header.substring(i);
    i= header.indexOf(" ");
    header.substring(1, i).toCharArray(this->params, 20);
  }else{
    //Si no tiene parametros
	String empty;
	empty.substring(1, i).toCharArray(this->params, 20);
    //URL
    i= header.indexOf(" ");
    header.substring(0, i).toCharArray(this->url, 15);
  }
}

String HttpRequest::getMethod(){
  return String(method);
}
String HttpRequest::getUrl(){
  return String(url);
}
char* HttpRequest::getParams(){
  return params;
}
String HttpRequest::getParam(String name){
  name+= "=";
  String val(this->params);
  int i= val.indexOf(name);
  if(i != -1){
    val= val.substring(i + name.length());
    i= val.indexOf("&");
    if(i != -1){
      return val.substring(0, i); 
    }else{
      return val;
    }        
  }else{
    return "-1";
  }
}
String HttpRequest::getBody(){
  return body;
}
String* HttpRequest::getPunteroBody(){
  return &body;
}
void HttpRequest::setBody(String body){
  this->body= body;
}

//
//HTTP SERVER
HttpServer::HttpServer(int port): EthernetServer(port){}

void HttpServer::beginServer(uint8_t *mac){
  Ethernet.begin(mac);
  this->begin();
}
void HttpServer::beginServer(uint8_t *mac, IPAddress local_ip){
  Ethernet.begin(mac, local_ip);
  this->begin();
}
void HttpServer::beginServer(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet){
  Ethernet.begin(mac, local_ip, dns_server, gateway, subnet);
  this->begin();
}

/*void setSecurity(bool security, String key){
  this->security= security;
  this->key= key;
}*/

HttpRequest HttpServer::httpRequest(EthernetClient &client){
  HttpRequest httpRequest;
  String header= this->readHeader(client);
  httpRequest.setVarsHeader(header);
  httpRequest.setBody(this->readBody(client));
  return httpRequest;
}

String HttpServer::readHeader(EthernetClient &client){
  String header;
  //an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean firstLine= true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
	  if(firstLine){
		header+= c;
	  }
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        return header;
      }
      if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
		firstLine= false;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
  }
}

String HttpServer::readBody(EthernetClient &client){
  String body;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      //Serial.write(c);
      body+= c;
    }else{
      return body;
    }
  }
}

bool HttpServer::send(EthernetClient &client, int code, String contentType, String &body){
  client.println("HTTP/1.1 " + String(code) + " OK");  
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: GET, PUT, POST, DELETE, PATCH, OPTIONS");
  client.println("Access-Control-Allow-Headers: Content-Type,Accept");
  client.println("Content-Type: " + contentType);
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
  client.println(body);
}

bool HttpServer::sendApiRest(EthernetClient &client, int code, String body){
  this->send(client, code, "application/json", body);
}
bool HttpServer::partialSendApiRest(EthernetClient &client, String body){
  client.print(body);
}

bool HttpServer::sendApiRestEncode(EthernetClient &client, int code, JsonInterface &element){
  this->sendApiRest(client, code, element.toJson());
}

//
//HTTP WIFI SERVER
HttpWiFiServer::HttpWiFiServer(uint16_t port): WiFiEspServer(port){}

/*void setSecurity(bool security, String key){
  this->security= security;
  this->key= key;
}*/

HttpRequest HttpWiFiServer::httpRequest(WiFiEspClient &client){
  HttpRequest httpRequest;
  String header= this->readHeader(client);
  httpRequest.setVarsHeader(header);
  httpRequest.setBody(this->readBody(client));
  return httpRequest;
}

String HttpWiFiServer::readHeader(WiFiEspClient &client){
  String header;
  //an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean firstLine= true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
	  if(firstLine){
		header+= c;
	  }
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        return header;
      }
      if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
		firstLine= false;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
  }
}

String HttpWiFiServer::readBody(WiFiEspClient &client){
  String body;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      //Serial.write(c);
      body+= c;
    }else{
      return body;
    }
  }
}

bool HttpWiFiServer::send(WiFiEspClient &client, int code, String contentType, String &body){
  client.println("HTTP/1.1 " + String(code) + " OK");  
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: GET, PUT, POST, DELETE, PATCH, OPTIONS");
  client.println("Access-Control-Allow-Headers: Content-Type,Accept");
  client.println("Content-Type: " + contentType);
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
  client.println(body);
}

bool HttpWiFiServer::sendApiRest(WiFiEspClient &client, int code, String body){
  this->send(client, code, "application/json", body);
}
bool HttpWiFiServer::partialSendApiRest(WiFiEspClient &client, String body){
  client.print(body);
}

bool HttpWiFiServer::sendApiRestEncode(WiFiEspClient &client, int code, JsonInterface &element){
  this->sendApiRest(client, code, element.toJson());
}

