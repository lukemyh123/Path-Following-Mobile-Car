int rightMotor = 3;
int leftMotor = 10;

int redLED = 4;  //red for right turn
int blueLED = 5; //blue for left turn
int greenLED = 7;  //green for straight ahead

int speed_L = 0;
int speed_R = 0;
int pErrorLeft = 0;
int dErrorLeft = 0;
int totalErrorLeft = 0;

int pErrorRight = 0;
int dErrorRight = 0;
int totalErrorRight = 0;

int kp = 5.5;    //need to adjust in practice
int ki = 0;
int kd = 1.5;

int set_leftSensor = 0;
int set_rightSensor = 0;  //when phototransistor is above the white color thing, the max number of analogRead from A1 or A2;

int previous_seneor_wheel = 0;
int dSensorWheel = 0;  //the difference between previous number and next number
int counter = 0;  //using the counter to count the states changing

bool wheel_running = false;

unsigned long timeCounter;

void setup() {
    Serial.begin(9600);
    
    pinMode(rightMotor, OUTPUT);
    pinMode(leftMotor, OUTPUT);
    
    pinMode(redLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    
    digitalWrite(redLED, HIGH);
    delay(100);
    digitalWrite(blueLED, HIGH);
    delay(100);
    digitalWrite(greenLED, HIGH);
    delay(100);
    
}

void loop() {
    
      int sensorLeft = analogRead (A1);
     int sensorRight = analogRead (A3);
     
     if(set_leftSensor > sensorLeft)
     set_leftSensor = set_leftSensor;
     else if(set_leftSensor < sensorLeft)
     set_leftSensor = sensorLeft;
     
     if(set_rightSensor > sensorRight)
     set_rightSensor = set_rightSensor;
     else if(set_rightSensor < sensorRight)
     set_rightSensor = sensorRight;
     
     int initialError = pErrorLeft;
     pErrorLeft = set_leftSensor - sensorLeft;
     dErrorLeft = pErrorLeft - initialError;
     totalErrorLeft = kp*pErrorLeft + kd*dErrorLeft;
     
     int initialError_right = pErrorRight;
     pErrorRight = set_rightSensor - sensorRight;
     dErrorRight = pErrorRight - initialError_right;
     totalErrorRight = kp*pErrorRight + kd*dErrorRight;
     
     /////////////////////////////////////////////////////////////////////
     
     //Left_MOTOR
     speed_L = 255-totalErrorLeft;  //slow down the left motor
     if(totalErrorLeft > 800)
     {
     speed_L = 0;
     speed_R = 170;
     
     }
     
     if(totalErrorLeft < 50)
     speed_L = 230;   //slow speed 170  left motor should go faster because the car always go left
     
     if(totalErrorLeft > 50 && totalErrorLeft < 800)
     speed_L = 150;
     
     //Right_MOTOR
     speed_R = 255-totalErrorRight;  //slow down the right motor
     if(totalErrorRight > 800)
     {
     speed_R = 0;
     speed_L = 170;
     
     }
     if(totalErrorRight < 50)
     speed_R = 200;  //slow speed 180
     
     if(totalErrorRight > 50 && totalErrorRight < 800)
     speed_R = 150;
     
     analogWrite(leftMotor, speed_L);
     analogWrite(rightMotor, speed_R);
     
     /////////////////////////////////////////////////////////////////////
     
     if(speed_L > 160 && speed_R > 160)
     {
     digitalWrite(redLED, LOW);
     digitalWrite(blueLED, LOW);
     digitalWrite(greenLED, HIGH);
     }
     else if(speed_R < 50 && speed_L > 160)
     {
     digitalWrite(redLED, HIGH);
     digitalWrite(blueLED, LOW);
     digitalWrite(greenLED, LOW);
     }
     else if(speed_L < 50 && speed_R > 160)
     {
     digitalWrite(redLED, LOW);
     digitalWrite(blueLED, HIGH);
     digitalWrite(greenLED, LOW);
     }
     
     
     if(totalErrorRight > 800 && totalErrorLeft > 800)
     {
     while(1)
     {
     analogWrite(leftMotor, 0);     //stop the wheel when approaching the end
     analogWrite(rightMotor, 0);
     
     digitalWrite(redLED, HIGH);   //Blue and Red lights flash when aproaching the end
     digitalWrite(blueLED, HIGH);
     digitalWrite(greenLED,LOW);
     
     }
     }
     
     /*
     Serial.print(sensorLeft);   //Test the Left Phototransistor
     Serial.print(" ");
     
     Serial.print(sensorRight);   //Test the Right Phototransistor;
     Serial.print(" ");
     
     Serial.print(totalErrorLeft);   //Test the Right Phototransistor;
     Serial.print(" ");
     
     Serial.print(totalErrorRight);   //Test the Right Phototransistor;
     Serial.print(" ");
     
     
     Serial.print(speed_L);   //Test the Left speed;
     Serial.print(" ");
     
     Serial.print(speed_R);   //Test the Right speed;
     Serial.println(" ");
     
     */
    //Extra credit
    
    analogWrite(leftMotor, 120);
    analogWrite(rightMotor, 120);
    
    int sensorWheel = analogRead (A5);
    
    dSensorWheel = sensorWheel - previous_seneor_wheel;
    previous_seneor_wheel = sensorWheel;
    //Test the Right Phototransistor;
    
    if (abs(dSensorWheel) < 10)  // wheel is stopping
    {
        counter ++;
    }
    else                         //wheel is running
    {
        counter = 0;
        
    }
    
    if (counter > 10)
    {
        analogWrite(leftMotor, 255);
        analogWrite(rightMotor, 255);
        
        delay(1000);
        
        analogWrite(leftMotor, 120);
        analogWrite(rightMotor, 120);
    }
    
    Serial.print(sensorWheel);   //Test the Right Phototransistor;
    Serial.print(" ");
    
    Serial.print(abs(dSensorWheel));
    Serial.print(" ");
    Serial.println(counter);
    
    
    if(counter > 5)
    {
        digitalWrite(redLED, HIGH);    //LED light flash 5/sec when stoping the wheel
        digitalWrite(blueLED, HIGH);
        digitalWrite(greenLED,HIGH);
        delay(200);
        
        digitalWrite(redLED, LOW);
        digitalWrite(blueLED, LOW);
        digitalWrite(greenLED,LOW);
        delay(200);
    }
    else if(counter < 10)
    {
        digitalWrite(redLED, LOW);
        digitalWrite(blueLED, LOW);
        digitalWrite(greenLED,LOW);
    }
}
