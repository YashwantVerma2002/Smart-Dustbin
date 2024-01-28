#include<Servo.h>
#include<ESP8266_AT.h>
Servo servoMain;
int trigpin = 7;
int echopin = 8;
int distance;
float duration;
float cm;
int led = 4;


/* Select Demo */
//#define RECEIVE_DEMO			/* Define RECEIVE demo */
#define SEND_DEMO				/* Define SEND demo */

/* Define Required fields shown below */
#define DOMAIN          "api.thingspeak.com"
#define PORT            "80"
#define API_WRITE_KEY   "NCG86AKF7CJ4NAJK"
#define CHANNEL_ID      "2239865"

#define SSID            "yashwant verma"
#define PASSWORD        "11223344"

char _buffer[150];
uint8_t Connect_Status;
#ifdef SEND_DEMO
uint8_t Sample = 0;
#endif

void setup()
{
 servoMain.attach(9);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(led,OUTPUT);
  
    Serial.begin(115200);
 
    while(!ESP8266_Begin());
    ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);	/* 3 = Both (AP and STA) */
    ESP8266_ConnectionMode(SINGLE);     			/* 0 = Single; 1 = Multi */
    ESP8266_ApplicationMode(NORMAL);    			/* 0 = Normal Mode; 1 = Transperant Mode */
    if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)/*Check WIFI connection*/
    ESP8266_JoinAccessPoint(SSID, PASSWORD);		/*Connect to WIFI*/
    ESP8266_Start(0, DOMAIN, PORT);
}

void loop()
{
  
    Connect_Status = ESP8266_connected();
    if(Connect_Status == ESP8266_NOT_CONNECTED_TO_AP)	/*Again check connection to WIFI*/
    ESP8266_JoinAccessPoint(SSID, PASSWORD);			/*Connect to WIFI*/
    if(Connect_Status == ESP8266_TRANSMISSION_DISCONNECTED)
    ESP8266_Start(0, DOMAIN, PORT);						/*Connect to TCP port*/

    #ifdef SEND_DEMO
    memset(_buffer, 0, 150);
    sprintf(_buffer, "GET /update?api_key=%s&field1=%d", API_WRITE_KEY, Sample++); 	/*connect to thingspeak server to post data using your API_WRITE_KEY*/
    ESP8266_Send(_buffer);
    delay(15000); 										/* Thingspeak server delay */
    #endif
    
    #ifdef RECEIVE_DEMO
    memset(_buffer, 0, 150);
    sprintf(_buffer, "GET /channels/%s/feeds/last.txt", CHANNEL_ID); /*Connect to thingspeak server to get data using your channel ID*/
    ESP8266_Send(_buffer);
    Read_Data(_buffer);
    delay(600);
    
  digitalWrite(trigpin, LOW);
  delay(2);             // Wait for 2 millisecond(s)
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);         // Wait for 2 millisecond(s)
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  cm = (duration/58.82);
  distance = cm;
  if(distance<30)
  {
    servoMain.write(180);
    digitalWrite(led,HIGH);
    delay(3000);
    Serial.println("the distance" +distance);
  /*  for(int i =0;i<180;i++){
    servoMain.write(i);
    delay(100);
    }*/
  }
  else
  { 
   // for(int i =180;i>=0;i-- )
    
    servoMain.write(0);
    digitalWrite(led,LOW);
    delay(30);
    
  }
  if(distance >60 || distance <=0)
  {
    Serial.println("the distance is more than 60 "+distance);
  }
  else
  {
    Serial.print(distance);
    Serial.println("cm");
  }
  delay(100);
}