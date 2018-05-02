//#include "WConstants.h"
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
//#include "../arduino-basic-oo/ElementAbstract.h"
#include "HttpClientAr.h"

//
//HTTP RESPONSE
HttpResponse::HttpResponse(){ 
}
void HttpResponse::setVarsHeader(String &header){
  //Me quedo con solo la primer linea del header
  int i= header.indexOf("\n");
  header= header.substring(0, i);
  //HTTP CODE
  i= header.indexOf(" ");
  this->code= header.substring(i + 1, i + 4);
}
String HttpResponse::getCode(){
  return code;
}
String HttpResponse::getBody(){
  return body;
}
void HttpResponse::setBody(String body){
  this->body= body;
}

//
//HTTP CLIENT
HttpClientAr::HttpClientAr(): EthernetClient(){}

HttpResponse HttpClientAr::httpResponse(){
  HttpResponse httpResponse;
  String header= this->readHeader();
  httpResponse.setVarsHeader(header);
  httpResponse.setBody(this->readBody());  
  return httpResponse;
}

String HttpClientAr::readHeader(){
  String header;
  //an http request ends with a blank line
  boolean currentLineIsBlank = true;
  while (this->connected()) {
    if (this->available()) {
      char c = this->read();
      header+= c;
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        return header;
      }
      if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
  }
}

String HttpClientAr::readBody(){
  String body;
  boolean currentLineIsBlank = true;
  while (this->connected()) {
    if (this->available()) {
      char c = this->read();
      //Serial.write(c);
      body+= c;
    }else{
      return body;
    }
  }
}

void HttpClientAr::sendRequest(String method, String uri){
  this->println(method + " " + uri + " HTTP/1.1");
  this->println("Host: 10.10.10.100");  
  this->println("User-Agent: arduino-ethernet");  
  this->println("Connection: close");
  this->println();
}
void HttpClientAr::sendRequest(String method, String uri, String contentType){
  this->println(method + " " + uri + " HTTP/1.1");
  this->println("Host: 10.10.10.100");  
  this->println("User-Agent: arduino-ethernet");  
  this->println("Connection: close");
  this->println("Content-Type: " + contentType);
  this->println();
}
void HttpClientAr::sendRequest(String method, String uri, String contentType, String &body){
  this->println(method + " " + uri + " HTTP/1.1");
  this->println("Host: 10.10.10.100");  
  this->println("User-Agent: arduino-ethernet");  
  this->println("Connection: close");
  this->println("Content-Type: " + contentType); 
  this->print("Content-Length: ");
  this->println(body.length());
  this->println();
  this->println(body);
}
void HttpClientAr::sendBody(String &body){
  this->println(body);
}
