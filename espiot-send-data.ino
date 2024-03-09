//Include libraries
#include <HTTPClient.h>               
#include <WiFi.h>                     
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BH1750.h>
#include <Wire.h>


//Add WIFI data
const char* ssid = "SSID";              //Add your WIFI network name 
const char* password =  "WiFiPW";           //Add WIFI password

String apiKeyValue = "bCkEI52Jk8j5F9"; //API Key

unsigned int Actual_Millis, Previous_Millis;
int refresh_time = 200;

//DS18B20 - D27 in ESP32
#define ONE_WIRE_BUS 27

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Create new instance for BH1750
BH1750 lightMeter; 

void setup() {
  delay(10);
  Serial.begin(115200);                   //Start monitor
  
  sensors.begin(); //DS18B20 - start

  Wire.begin(21,22); //BH1750 -- D21(SDA ESP32) - DAT (sensor) --- D22(SCL ESP32) - SCL(sensor)
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE); 
  
  WiFi.begin(ssid, password);             //Start wifi connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Actual_Millis = millis();               //Save time for refresh loop
  Previous_Millis = Actual_Millis; 
}


void loop() {  
  //We make the refresh loop using millis() so we don't have to sue delay();
  Actual_Millis = millis();
  if(Actual_Millis - Previous_Millis > refresh_time){
    Previous_Millis = Actual_Millis;  
    if(WiFi.status()== WL_CONNECTED){                   //Check WiFi connection status    
            perweb();                           //Sending data to Web
    }//END of WIFI connected
    else{
      Serial.println("WIFI connection error");
    }
  }
}

void perweb(){
   sensors.requestTemperatures(); //DS18B20 - Request sensors data
   float tempC = sensors.getTempCByIndex(0);
   float lux = 0;
   if (lightMeter.measurementReady()) {
   lux = lightMeter.readLightLevel();
} else{
   lux = 1.1111; //Error number
}
   
   HTTPClient https;
   
    // Your Domain name with URL path or IP address with path
    https.begin("https://........./post-data.php");
    
    // Specify content-type header
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(tempC)
                           + "&value2=" + String(lux) + "";
   
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    

    // Send HTTP POST request
    int httpResponseCode = https.POST(httpRequestData);
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();
}
