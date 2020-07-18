#include <ESP8266WiFi.h>

#define LED_1 3                     // Light 1 connected to D3
#define LED_2 4                     // Light 2 connected to D4
#define FAN_1 5                     // Fan 1 connected to D5
#define TEMP A0                     // Temperature sensor connected to A0
 
const char* ssid = "Honor 8x";      // Network name
const char* password = "2a2e952c";  // Network password
int fan=0;                          // retains last state of the fan 
float temp;                         // temperature stored. Data read from 20 to 358


WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200);
  pinMode(LED_1 , OUTPUT);
  pinMode(LED_2 , OUTPUT);
  pinMode(FAN_1 , OUTPUT);
  pinMode(TEMP  , INPUT);

  delay(750);

/*=============WiFi setup=================*/
 
  Serial.print("Connecting to Internet ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
 
 /*-------- server started---------*/ 
  server.begin();
  Serial.println("Server started");
 
  /*------printing ip address--------*/
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the above");
}
 
void loop() 
  {
    WiFiClient client = server.available();    
    if (!client) 
    {
      return;
    }
  Serial.println("Waiting for new client");   
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();                                        // waits for the transmission to complete
    
    temp =analogRead(TEMP);                             // Temperature conversion into degree celcius.
    temp = map(temp,20,358,-40,125);                    // Adjust values to get accurate results according to sensor data sheet

    
  if (request.indexOf("OFF1") != -1)                    // indexOf() checks for the presence of string passed in as a parameter with request
    digitalWrite(LED_1, LOW);
    
  if (request.indexOf("ON1") != -1)  
    digitalWrite(LED_1, HIGH);
  
  if (request.indexOf("ON2") != -1)  
     digitalWrite(LED_2, HIGH);
 
  if (request.indexOf("OFF2") != -1)  
     digitalWrite(LED_2, LOW);

  if (request.indexOf("ONF") != -1)
     fan=1;
      
  if (request.indexOf("OFFF") != -1)
   {
     fan=0;
     digitalWrite(FAN_1,LOW);
   }

  if (temp > 27.00 && fan==1)
    digitalWrite(FAN_1,HIGH);
  else 
    digitalWrite(FAN_1,LOW);
 
 
/*===============Creating html page=====================*/

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1> ROOM CONTROLL </h1><BR>");

/*------------------LIGHT1-------------------------------*/
  
  client.println("<p2> LIGHT 1 </p2><br>");
  client.print("LED is: ");
 
  if(digitalRead(LED_1) == HIGH) 
    client.print("ON");
  else
    client.print("OFF");
  
  client.println("<br><p1> SWITCH : </p1>");
  client.println("<a href=\"/=ON1\"\"><button>ON</button></a>");
  client.println("<a href=\"/=OFF1\"\"><button>OFF</button></a><br><BR><BR>");  


/*---------------------LIGTH 2 ------------------------------------*/
  client.println("<p2> LIGHT 2 </p2><br>");
  client.print("LED is: ");
 
  if(digitalRead(LED_2) == HIGH) 
    client.print("ON");
  else
    client.print("OFF");

  client.println("<br><p1> SWITCH : </p1>");
  client.println("<a href=\"/=ON2\"\"><button>ON</button></a>");
  client.println("<a href=\"/=OFF2\"\"><button>OFF</button></a><br><br><br>"); 


/*----------------------Fan--------------------------------------*/

  client.println("<p2> FAN </p2><br>");
  client.print("FAN is: ");
 
  if(digitalRead(FAN_1) == HIGH) 
    client.print("ON");
  else
    client.print("OFF");
 
  client.print("<br><p1> temperature is :  </p1>");
  client.print(temp);
  client.print("<p1> °C </p1>");
  client.println("<br><p1> SWITCH : </p1>");
  client.println("<a href=\"/=ONF\"\"><button>ON</button></a>");
  client.println("<a href=\"/=OFFF\"\"><button>OFF</button></a><br />");

  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println(" ");

  }
