// water level monitoring system
// telegram and wifi defination
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>// wifi client library
#include <UniversalTelegramBot.h> // Telegram Bot Library for the bot communication
#include <ArduinoJson.h>

// network credentials
 const char* ssid= "NEVEREST";  //put your wifi name
 const char* password = "nikoworks";//enter your wifi password

//anyone who has this ID can access your bot
// Also note that you need to click "start" on a bot before it can
// message you
 #define CHAT_ID  "1125047785"

// Initialize Telegram BOT
#define BOTtoken "5198068575:AAH4P2i3XqNnay7ho-ldoxwVVrnQ-L3W-aQ" // your Bot Token will be given by the bot father in telegram app

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 2 minutes.
int botRequestDelay = 120000;
unsigned long lastTimeBotRan;

//constant variable
const int watersensor=A0;
int greenled=5;
int redled=16;
int yellowled=4;
int sensorpower;
int level;
int val = 0;

int readSensor() {
  digitalWrite(sensorpower, HIGH);// Turn the sensor ON
  delay(10); // wait 10 milliseconds
  val = analogRead(watersensor);// Read the analog value form sensor
  digitalWrite(sensorpower, LOW);// Turn the sensor OFF
  return val;// send current reading
}

String getReadings(){
  int val;
  val = analogRead(watersensor);// Read the analog value form sensor
  String message = "Water level: " + String(val) + " cm \n";
  return message;
}
//what to do when you receive new messages and if they are from an unauthorised people deny access
void handleNewMessages(int NewMessages) {
  Serial.println("You have new messages");
  Serial.println(String(NewMessages));

//checks for new messages
  for (int i=0; i<NewMessages; i++) {
    // Chat id and identifies who sent the message
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");//if the chat ID entered is not yours report unauthorised user
      continue;
    }
    
 // Print the received messages on your bot
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;//saves the name of the sender
//commands to give the bot
        if (text == "/start")//start the bot 
        {
          String welcome = "Welcome, " + from_name + ".\n";
          welcome += "Type the following command to get current readings.\n\n";
          welcome += "/readings \n";
          bot.sendMessage(chat_id, welcome, "");
        }
    
        if (text == "/readings") //type this to receive current readings from your system on your bot remotely
        {
          String readings = getReadings();
          String message = "Water level: " + String(val) + " cm \n";
          bot.sendMessage(chat_id,readings, "");
        }  
    }
}
   int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

void setup() {
  Serial.begin(115200);//for the mcu
  
   #ifdef ESP8266
   client.setInsecure();
  #endif

// put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(greenled,OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(yellowled,OUTPUT);
  pinMode(watersensor,INPUT);
  pinMode(sensorpower,OUTPUT);
  
  
// Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi..");
 }
}

void loop()  // put your main code here, to run repeatedly:
{
handleNewMessages(numNewMessages);
level=analogRead(watersensor);
Serial.println(level);

if (millis() > lastTimeBotRan + botRequestDelay){
if(level<350)
{ digitalWrite(redled, HIGH);
  digitalWrite(greenled, HIGH);
   digitalWrite(yellowled, HIGH);
     Serial.print("Water level: ");
       String message = "Water level: " + String(val) + " cm \n";
        bot.sendMessage(CHAT_ID ,"The level of water in your tank is very LOW", "");
          }
          
 else if(level>350&&level<400)
{ digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);
   digitalWrite(yellowled, LOW);
    }
    
else if(level>400&&level<450)
 {
  digitalWrite(redled, LOW);
  digitalWrite(greenled, LOW);
   digitalWrite(yellowled, HIGH);
    }

else if(level>450&&level<500)
 {
  digitalWrite(redled, LOW);
  digitalWrite(greenled, LOW);
   digitalWrite(yellowled, HIGH);
   }
   
  else if(level>500)
 {
  digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);
   digitalWrite(yellowled, LOW);
     Serial.print("Water level: ");
      String message = "Water level: " + String(val) + " cm \n";
       bot.sendMessage(CHAT_ID ,"Your tank will overflow!!", "");
   }
   
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages)//calls the handlenewmessage function
    {
      Serial.println("Received the response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
    }
}
//This is a function used to get the reading
