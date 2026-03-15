#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define ALARM_LED 2

hw_timer_t *timer = NULL;
Adafruit_MPU6050 mpu;

volatile uint32_t tick_counter = 0;
volatile bool timer_triggered = false; 
volatile uint32_t startTime;

float max_g = 0.0;
float filtered_z = 9.81;
float alpha = 0.15;

enum FlightState {
  ON_PAD, 
  BOOST,
  COAST,
  APOGEE,
  LANDED
};

FlightState currentState = ON_PAD;

void IRAM_ATTR onTimer(){
  tick_counter++;
  timer_triggered = true; 
}

void setup(){
  Serial.begin(115200);
  pinMode(ALARM_LED, OUTPUT);

  if(!mpu.begin()){
    while(1){ delay(10); }
  }
  
  Wire.setClock(400000);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 2500, true, 0);
}

void loop(){
  if(timer_triggered){
    startTime = micros();
    
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    filtered_z = (alpha * a.acceleration.z) + ((1.0 - alpha) * filtered_z);

    switch(currentState){
      
      case ON_PAD:
        if(filtered_z > 15.0){
          currentState = BOOST;
          Serial.println(">>> EVENT: LIFTOFF DETECTED! <<<");
        }
        break;
      
      case BOOST:
        if(filtered_z > max_g){
          max_g = filtered_z;
        }
        if(filtered_z < 11.0){
          currentState = COAST;
          Serial.print(">>> EVENT: BURNOUT. Max Z: ");
          Serial.print(max_g);
          Serial.println(" m/s^2 <<<");
        }
        break;

      case COAST:
        if(filtered_z < 2.0){
          currentState = APOGEE;
          digitalWrite(ALARM_LED, HIGH);
          Serial.println(">>> EVENT: APOGEE / PARACHUTE DEPLOY <<<");
        }
        break;

      case APOGEE:
        if(abs(filtered_z - 9.81) < 0.5){
          currentState = LANDED;
          digitalWrite(ALARM_LED, LOW);
          Serial.println(">>> EVENT: TOUCHDOWN DETECTED <<<");
        }
        break;

      case LANDED:
        break;
    }

    timer_triggered = false; 

    if(tick_counter % 400 == 0){
       uint32_t load = micros() - startTime;
       Serial.print("State: " );
       Serial.print(currentState);
       Serial.print(" | Filt Z: ");
       Serial.print(filtered_z);
       Serial.print(" | Load: ");
       Serial.println(load);
    }
  }
}
