#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#define NUM_LEDS 8


char * ssid = "motoj6";
char * password = "123456789";

// Controlling the WS2812B

CRGB led[NUM_LEDS];
// led[] = CRGB::Black;


// Setting up the websocket client
WebSocketsClient webSocket;

void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

   // Initializing the WS2812B communication
  FastLED.addLeds<WS2812B, D3,GRB>(&led[0],NUM_LEDS);
  
  setRgb(0,0,0);

  
// Initializing the websocket communication
  webSocket.begin("192.168.43.32", 8000, "/subscribe");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  
}

void loop() {
  webSocket.loop();

}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] Received text: %s\n", payload);
      DynamicJsonBuffer jBuffer;
      JsonObject &root = jBuffer.parseObject(payload);
      setRgb(root["r"],root["g"],root["b"]);
      break;
  }
}


void setRgb(uint8_t r, uint8_t g, uint8_t b) {

  for(int i = 0;i < NUM_LEDS;i++){
  led[i].r = r;
  led[i].g = g;
  led[i].b = b;
  FastLED.show(); 
  }
  
  
}

//void ciclo(){
//  for(int i = 0;i < NUM_LEDS;i++){
//   led[0] 
//  }
//}


