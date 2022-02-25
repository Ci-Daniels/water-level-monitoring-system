// water level monitoring system
//arduino test
// Sensor pins
#define sensorPower 7
#define watersensor A0
#define greenled 5
#define redled 6

// Value for storing water level
int val = 0;

void setup() {
  // Set D7 as an OUTPUT
  pinMode(sensorPower, OUTPUT);
  pinMode(greenled,OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(watersensor,INPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
  
  Serial.begin(9600);
}

void loop() {
  //get the reading from the function below and print it
  int level = readSensor();
  
if(level<400)
{ digitalWrite(redled, HIGH);
  digitalWrite(greenled, HIGH);
     Serial.print("REFILL TANK: Water level: ");
      Serial.println(level);      
      }
          
 else if(level>400&&level<450)
{ digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);
    }
    
else if(level>450&&level<500)
 {
  digitalWrite(redled, LOW);
  digitalWrite(greenled, HIGH);
    }
  else if(level>500)
 {
  digitalWrite(redled, LOW);
  digitalWrite(greenled, LOW);
     Serial.print("OVERFLOW;SWITCH OFF PUMP; Water level: ");
      Serial.println(level);    
      
   }
  
  delay(6000);
}

//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(watersensor);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}
