/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>
#include <EEPROM.h>

#include "config.h"
#include "website.h"

#define LOOP_DELAY                                10    // milliseconds
#define PWR_BUTTON_SHUTDOWN_TIME   (5000/LOOP_DELAY)
#define PWR_BUTTON_START_TIME       (500/LOOP_DELAY)

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
HTTPClient http;


uint16_t press_pwr_button_time;

uint8_t auto_start_enabled;
bool auto_start_runned = false;
uint16_t auto_start_counter;

uint8_t auto_shutdown_enabled;
bool auto_shutdown_runned = false;
uint16_t auto_shutdown_counter;


int robot_batt_soc;
int robot_state;
String robot_state_str;
String robot_access_error = "<font color=\"red\">NO ROBOT CONNECTION!</font>";

const size_t bufferSize = JSON_OBJECT_SIZE(12) + 200;
StaticJsonDocument<bufferSize> jsonDoc;

void getSavedData(){
  EEPROM.begin(4);
  EEPROM.get(0, auto_start_enabled);
  EEPROM.get(0+sizeof(auto_start_enabled), auto_shutdown_enabled);    
  EEPROM.end();  

  if (auto_start_enabled > 1 || auto_shutdown_enabled > 1){
    auto_start_enabled = 0;
    auto_shutdown_enabled = 0;
    saveData();
  }
}

void saveData(){
  EEPROM.begin(4);
  EEPROM.put(0, auto_start_enabled);
  EEPROM.put(0+sizeof(auto_start_enabled), auto_shutdown_enabled);  
  EEPROM.commit();
  EEPROM.end();
}

void get_robot_state(){
  http.begin(valetudo_url + "/api/current_status/");
  int httpCode = http.GET();

  if(httpCode == HTTP_CODE_OK) {
    String response = http.getString();

    deserializeJson(jsonDoc, response);
    JsonObject root = jsonDoc.as<JsonObject>();

    robot_batt_soc = root["battery"]; // 90
    robot_state = root["state"]; // "8"
    robot_state_str = root["human_state"].as<String>();
    

    robot_access_error="";
  }
  else
    robot_access_error="<font color=\"red\">NO ROBOT CONNECTION!</font>";
}


void setup(void) {

  // MOSFET Trigger
  pinMode(PIN_MOS, OUTPUT);
  digitalWrite(PIN_MOS, LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
    delay(3000);
  }


  MDNS.begin(host);

  httpUpdater.setup(&httpServer);

  httpServer.on("/", [](){
    httpServer.send(200, "text/html", get_startpage());
  });

  httpServer.on("/shutdown", [](){   
    // show shtudown message and redirect to main page after 6 seconds
    httpServer.send(200, "text/html", header+"<meta http-equiv=\"refresh\" content=\"6;URL='/'\" /></head><body>Shutdown now!</body>"+footer); 
    press_pwr_button_time = PWR_BUTTON_SHUTDOWN_TIME;
  });

  httpServer.on("/toggle_auto_shutdown", [](){    
    auto_shutdown_enabled = 1 - auto_shutdown_enabled; 
    saveData();
    httpServer.send(200, "text/html", get_startpage());
  });

  httpServer.on("/toggle_auto_start", [](){    
    auto_start_enabled = 1 - auto_start_enabled; 
    saveData();
    httpServer.send(200, "text/html", get_startpage());
  });

  httpServer.on("/get_data", [](){
   httpServer.send(200, "text/html", (String)robot_state_str + " (" + (String)robot_state + ")" + "\n" + \
                                     (String)robot_access_error + "\n" + \
                                     (String)robot_batt_soc + "\n" + \
                                     (String)auto_start_counter + "\n" + \
                                     (String)auto_shutdown_counter);
  });
  
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.println("Autocontrol ready! My IP is:");
  Serial.println(WiFi.localIP().toString());

  // get saved settings
  getSavedData();

  press_pwr_button_time = 0;

  auto_start_runned = false;
  auto_shutdown_runned = false;

  auto_start_counter = AUTO_START_DELAY;
  auto_shutdown_counter = AUTO_SHUTDOWN_DELAY;
}


void loop(void) {
  httpServer.handleClient();

  static uint16_t second_counter=1000/LOOP_DELAY;
  second_counter--;
  if (!second_counter){
    // run every second
    second_counter=1000/LOOP_DELAY;

    // get current state of robot from valetudo API
    get_robot_state();

    // if we did not shutdown before...
    if (auto_shutdown_runned == false){
      // if auto shutdown is enabled, we do not press button right now and we are in "charger disconnected" state, start timer
      if (auto_shutdown_enabled && press_pwr_button_time == 0 && robot_state == 2 ){    
        auto_shutdown_counter--;
        if (!auto_shutdown_counter){        
          //auto_shutdown_counter = AUTO_SHUTDOWN_DELAY;
          auto_shutdown_runned = true; 
          press_pwr_button_time = PWR_BUTTON_SHUTDOWN_TIME;               
        }
      }
      else{
        auto_shutdown_counter = AUTO_SHUTDOWN_DELAY;
      }
    }
      
    // if we did not start before...
    if (auto_start_runned == false){
      // if auto start is enabled, we do not press button right now and we are in "charging" state, start timer
      if (auto_start_enabled && press_pwr_button_time == 0 && robot_state == 8){    
        auto_start_counter--;
        if (!auto_start_counter){        
          //auto_start_counter = AUTO_START_DELAY;        
          auto_start_runned = true;
          press_pwr_button_time = PWR_BUTTON_START_TIME;               
        }
      }
      else{
        auto_start_counter = AUTO_START_DELAY;
      }
    }
    
  }
  
  
  if (press_pwr_button_time > 0)
  {
    digitalWrite(PIN_MOS, HIGH); 
    press_pwr_button_time--;
    if (!press_pwr_button_time)
    {
      // release power button
      digitalWrite(PIN_MOS, LOW);  
      Serial.println("release");    
    }
  }

  delay(LOOP_DELAY);
}
