/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define LOOP_DELAY  10
#define PIN_MOS     5

const char* host = "esp8266-webupdate";
const char* ssid = "XXXXXXXX";
const char* password = "YYYYYYY";

const String header = "<!DOCTYPE HTML>\r\n<html><head><title>MiRobotShutdown</title>";
const String footer = "</html>";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

bool shutdown_now = false;
int shutdown_counter = -1;

void startpage(){
  httpServer.send(200, "text/html", header + "</head><body>Mi Robot Shutdown v1.0<br><br><a href=\"shutdown\"><button>Shutdown now</button></a><a href=\"update\"><button>Update firmware</button></a></body>" + footer);
}

void setup(void) {

  // MOSFET Trigger
  pinMode(PIN_MOS, OUTPUT);
  digitalWrite(PIN_MOS, LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);

  httpServer.on("/", [](){
    startpage();
  });

  httpServer.on("/shutdown", [](){    
    shutdown_now = true; 
  });
  
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop(void) {
  httpServer.handleClient();
  
  if (shutdown_now)
  {
    // press power button
    digitalWrite(PIN_MOS, HIGH);

    // redirect to main page after 6 seconds
    httpServer.send(200, "text/html", header+"<meta http-equiv=\"refresh\" content=\"6;URL='/'\" /></head><body>Shutdown now!</body>"+footer);

    // init shutdown counter (button press length, 5 seconds)
    shutdown_counter = 5000/LOOP_DELAY;
    shutdown_now = false;    
  }
  else if (shutdown_counter > 0)
  {
    shutdown_counter--;
    if (!shutdown_counter)
    {
      // release power button
      digitalWrite(PIN_MOS, LOW);      
    }
  }

  delay(LOOP_DELAY);
}
