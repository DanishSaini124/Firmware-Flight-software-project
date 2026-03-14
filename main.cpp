hw_timer_t *timer = NULL;

volatile uint32_t tick_counter = 0;
volatile bool timer_triggered = false; 

void IRAM_ATTR onTimer(){
  tick_counter++;
  timer_triggered = true; 
}

void setup(){
  Serial.begin(115200);
  timer = timerBegin(1000000);

  timerAttachInterrupt(timer, &onTimer);

  timerAlarm(timer, 2500, true, 0);

  Serial.println("Flight Executive Heartbeat Started...");
}

void loop(){
  if(timer_triggered){
    timer_triggered = false; 
    if(tick_counter % 400 == 0){
      Serial.print("One second has passed. Total Ticks: ");
      Serial.println(tick_counter);
    }
  }
}
