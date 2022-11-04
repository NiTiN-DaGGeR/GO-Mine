#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESPmDNS.h>
#include <FirebaseESP32.h>
#include  <WiFi.h>
#include "DHT.h"
#include "MAX30105.h"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;
MAX30105 particleSensor;

//#define FIREBASE_HOST "go-mine-85a32-default-rtdb.firebaseio.com"
//#define FIREBASE_Authorization_key "0ZHoDg8x3hLdjG2pyvb3ntaUStuLV1OPA7bWazOA"

#define FIREBASE_Authorization_key "euDB3Coml1tifCgWCMXayU5zt2DhZSIHqMxNoeVI"
#define FIREBASE_HOST "database-ands-default-rtdb.firebaseio.com"

const char* ssid = "DaGGeR";
const char* password = "12345678";
char auth[] = "wdzSxn6RwBg1NoPIlTVtua4nNqJC5_v6";

#define DHTTYPE DHT11
#define DHTPIN 27
#define ONE_WIRE_BUS 4
#define SOUND_SPEED 0.034
#define MQ2 35
#define GREEN 16
#define RED 17
int sensorValue = 0;
int ir=13; 
int led=14; 


FirebaseData firebaseData;
FirebaseJson json;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String temperatureF = "";
String temperatureC = "";
String rtemperature = "";
String rhumidity = "";
String distance = "";
String gas = "";
String bpm = "";
String irs = "";

unsigned long lastTime = 0;  
unsigned long timerDelay = 10;
DHT dht(DHTPIN, DHTTYPE);
const int trigPin = 5;
const int echoPin = 18;
long duration;

AsyncWebServer server(80);

String readDHTTemperature() {

  float t = dht.readTemperature();

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "00";
  }
  else {
    Serial.print("Mine Temperature: ");
    Serial.println(t);
    Firebase.setFloat(firebaseData, "/ESP32_APP/TEMPERATURE", t);
//    delay(200);

     if (t > 27)
  {
    Blynk.notify("Alert!! Temperature Rising!!");
  }
  }
    return String(t);
  
}

String readDHTHumidity() {
  
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "00";
  }
  else {
    Serial.print("Mine Humidity: ");
    Serial.println(h);
    Firebase.setFloat(firebaseData, "/ESP32_APP/HUMIDITY", h);
//    delay(200);

     if (h > 65)
  {
    Blynk.notify("Alert!! Humidity Rising!!");
  }
  }
    return String(h);
  
}

String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);

  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "00";
  } else {
    Serial.print("Body Temperature Celsius: ");
    Serial.println(tempC); 
    Firebase.setFloat(firebaseData, "/ESP32_APP/BODY TEMPERATURE in C", tempC);
//    delay(200);

     if (tempC > 27)
  {
    Blynk.notify("Alert!! Harmful Body Temperature!!");
  }
  }
  return String(tempC);
}

String readDSTemperatureF() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempF = sensors.getTempFByIndex(0);

  if(int(tempF) == -196){
    Serial.println("Failed to read from DS18B20 sensor");
    return "00";
  } else {
    Serial.print("Body Temperature Fahrenheit: ");
    Serial.println(tempF);
    Firebase.setFloat(firebaseData, "/ESP32_APP/BODY TEMPERATURE in F", tempF);
//    delay(200);

    if (tempF > 83)
  {
    Blynk.notify("Alert!! Harmful Body Temperature!!");
  }
  }
  return String(tempF);
}

String readDistance(){

  float distanceCm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  
  distanceCm = duration * SOUND_SPEED/2;

  if (distanceCm < 1)
  {
    Serial.println("Ultrasonic Sensor not detected");
    return "00";
  }

  else
  {
 
  Serial.print("Water Level: ");
  Serial.println(distanceCm);
   Firebase.setFloat(firebaseData, "/ESP32_APP/WATER LEVEL in cm", distanceCm);
//    delay(200);

   if (distanceCm < 5)
  {
    Blynk.notify("Water is Rising Rapidly !!");
  }
  }

   return String(distanceCm);
   
}

String sendUptime(){

  float sensorValue;
  
  sensorValue = analogRead(MQ2);
  Blynk.virtualWrite(V1, sensorValue);
  Serial.print("Gas Level: ");
  Serial.println(sensorValue);
  Firebase.setFloat(firebaseData, "/ESP32_APP/GAS LEVEL", sensorValue);
//  delay(200);

  if (sensorValue > 1000)
  {
    Blynk.notify("Gas Detected!");
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
  }

  else
  {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
  }

  return String(sensorValue);
}

String readBPM() {

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
//    while (1);
    return "00";
  }
  else
{

byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411; //Options: 69, 118, 215, 411
int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
boolean state = false;
  
{
   float hr;
  
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  hr=(particleSensor.getIR()/1000);
  
 Serial.print("Heart Rate: ");
 Serial.println(hr); //Send raw data to plotter
 Firebase.setFloat(firebaseData, "/ESP32_APP/HEART RATE", hr);
//  delay(200);

    if(hr>80)
  {
    Blynk.notify("Alert!! Abnormal Heartbeat!!");
  }
   return String(hr);
 }
}  
}

String readIRS() {
  
 {
      int irvalue = HIGH;
      irvalue = digitalRead(ir);
      //int irvalue = digitalRead(ir);
      
      if(irvalue == LOW)
    {
      const char* i ="WORN";
      Serial.print("Helmet Status: ");  
      Serial.println(i); 
      Firebase.setString(firebaseData, "/ESP32_APP/HELMET STATUS", i);
      digitalWrite(led, LOW);
      
      return String(i);
    }
      else
    {
      const char* i ="NOT WORN";
      Serial.print("Helmet Status: "); 
      Serial.println(i); 
      Firebase.setString(firebaseData, "/ESP32_APP/HELMET STATUS", i); 
      digitalWrite(led, HIGH);
      Blynk.notify("Alert!! Helmet Opened");
          
      return String(i);
    }
  }
  
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    h2 {padding-left: 50px; }
    p {padding-left: 50px; }
    
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>WORKER HEALTH ANALYSIS<HR></h2>
  <p>
    <i class="fas fa-temperature-high" style="color:#059e8a;"></i> 
    <span class="dht-labels">Mine Temperature<br></span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</br></sup>
  </p>
  <HR>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Mine Humidity<br></span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</br></sup>
  </p>
  <HR>
  <p>
    <i class="fas fa-thermometer-half" style="color:#98f5e8;"></i> 
    <span class="dht-labels">Body Temperature<br></span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</br></sup>
  </p>
  <HR>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Body Temperature<br></span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;F</br></sup>
  </p>
  <HR>
  <p>
   <i class="fas fa-road" style="color:#0c42ae;"></i> 
    <span class="dht-labels">Water Level<br></span> 
    <span id="Distance">%DISTANCE%</span>
    <sup class="units">cm</br></sup>
  </p>
  <HR>
  <p>
    <i class="fas fa-gas-pump" style="color: #059e8a;"></i>
    <span class="dht-labels">Gas Detection<br></span> 
    <span id="gas">%GAS%</span>
    <sup class="units">ppm</br></sup>
  </p>
  <HR>
  <p>
    <i class="fas fa-heartbeat" style="color:rgb(245, 16, 16);"></i>
    <span class="dht-labels">Heart Rate<br></span> 
    <span id="bpm">%BPM%</span>
    <sup class="units">bpm</br></sup>
  </p>
  <HR>
   <p>
    <i class="fas fa-heartbeat" style="color:rgb(245, 16, 16);"></i>
    <span class="dht-labels">Helmet Status<br></span> 
    <span id="irs">%IRS%</span>
    <sup class="units">**</br></sup>
  </p>
  <HR>
  <p>
    <i class="fa fa-percent" style="color:rgb(57, 191, 235);"></i>
    <span class="dht-labels">SPO2<br></span> 
    <span id="Percentage">--</span>
    <sup class="units">&percnt;</br></sup>
  </p>
  <hr>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Distance").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/Distance", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("gas").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/gas", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("bpm").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/bpm", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("irs").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/irs", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";

String processor(const String& var){
  Serial.println(var);
  if(var == "TEMPERATURE"){
    return rtemperature;
  }
  else if(var == "HUMIDITY"){
    return rhumidity;
  }
  else if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  else if(var == "TEMPERATUREF"){
    return temperatureF;
  }
  else if(var == "DISTANCE"){
   return distance;
  }
  else if(var == "GAS"){
   return gas;
  } 
  else if(var == "BPM"){
   return bpm;
  }
  else if(var == "IRS"){
   return irs;
  } 
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);


  dht.begin();
  sensors.begin();

   Serial.println();
   Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);

     // Connect to Wi-Fi
  WiFi.begin (ssid, password);
  Blynk.begin(auth, ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  if(!MDNS.begin("esp32")) {
     Serial.println("Error starting mDNS");
     return;
  }

    Serial.println(WiFi.localIP());

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MQ2, INPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(ir, INPUT);
  pinMode(led, OUTPUT);


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", rtemperature.c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", rhumidity.c_str());
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureF.c_str());
  });
  server.on("/distanceCm", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", distance.c_str());
  });
  server.on("/gas", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", gas.c_str());
  });
  server.on("/bpm", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", bpm.c_str());
  });
  server.on("/irs", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", irs.c_str());
  });

  server.begin();
}
 
void loop(){

  Blynk.run();
  timer.run();
  
  if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    temperatureF = readDSTemperatureF();
    distance = readDistance();
    rhumidity = readDHTHumidity();
    rtemperature = readDHTTemperature();
    gas = sendUptime();
    bpm = readBPM();
    irs = readIRS();
    lastTime = millis();
  }  

}
