// NES Controller Arduino code - By Evin Jaff
// Written from scratch from 
// https://tresi.github.io/nes/
// https://www.nesdev.org/wiki/Standard_controller

//Preprocessed Pins

#define CLCK_PIN 2
#define LATCH_PIN 3
#define DAT
#define STATUS_LED 13

#define check_latch digitalRead(LATCH_PIN)
#define check_pulse digitalRead(CLCK_PIN)

//global vars
// in_latch: True if waiting on a Latch Symbol
// in_latch
bool pulse_fall;
bool latch_fall;
unsigned int previous_pulse_state = 0;
unsigned int previous_latch_state = 0;

enum states {
  IDLE,
  A,
  B,
  SELECT,
  START,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum states curr_state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  curr_state=IDLE;
  pulse_fall = false;
  latch_fall = false;
  pinMode(CLCK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT);
  pinMode(LATCH_PIN, INPUT);
  pinMode(STATUS_LED, OUTPUT);

  digitalWrite(STATUS_LED, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int pulse = check_pulse;
  unsigned int latch = check_latch;
  // Serial.print("L: ");
  // Serial.print(latch);
  // Serial.print(" CLCK: ");
  // Serial.print(pulse);
  // Serial.print(" pulse_fall: ");
  // Serial.print(pulse_fall);
  // Serial.print(" latch fall: ");
  // Serial.print(latch_fall);
  // Serial.print(" transition: ");
  // Serial.print(pulse_fall & pulse);
  // Serial.print(" State: ");
  // print_state(curr_state);
  

  //Main loop pseudocode - Start in Latch

  //Wait for a HIGH on the latch, and then poll the controller for inputs
  switch (curr_state){

    case IDLE:
    //check transition
    if (latch){
      curr_state = A;
    }
    break;
    case A:
    if(!latch_fall){
      check_latch_fall(latch);
    }   

    //check latch fall instead
    if(pulse == 1){
      pulse_fall = false;
      latch_fall = false;
      curr_state = B;
    }
    break;
    case B:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    }    
    //signal states
    digitalWrite(DATA_PIN, 1);
    digitalWrite(STATUS_LED, LOW);

    //check latch fall instead
    if(pulse == 1 && pulse_fall){
      pulse_fall = false;
      latch_fall = false;
      curr_state = SELECT;
    }
    break;
    case SELECT:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    } 
    //signal states
    digitalWrite(DATA_PIN, 1);
    digitalWrite(STATUS_LED, LOW);

    //check transition
    if(pulse && pulse_fall){
      pulse_fall = false;
      curr_state = START;
    }
    break;
    case START:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    } 
    //signal states
    digitalWrite(DATA_PIN, 0);
    digitalWrite(STATUS_LED, HIGH);

    //check transition
    if(pulse && pulse_fall){
      pulse_fall = false;
      curr_state = UP;
    }
    break;
    case UP:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    } 
    //signal states
    digitalWrite(DATA_PIN, 1);
    digitalWrite(STATUS_LED, LOW);


    //check transition
    if(pulse && pulse_fall){
      pulse_fall = false;
      curr_state = DOWN;
    }
    break;
    case DOWN:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    } 
    //signal states
    digitalWrite(DATA_PIN, 1);
    digitalWrite(STATUS_LED, LOW);


    //check transition
    if(pulse && pulse_fall){
      pulse_fall = false;
      curr_state = LEFT;
    }
    break;
    case LEFT:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    } 
    //signal states
    digitalWrite(DATA_PIN, 1);
    digitalWrite(STATUS_LED, LOW);

    //check transition
    if(pulse && pulse_fall){
      pulse_fall = false;
      curr_state = RIGHT;
    }
    break;
    case RIGHT:
    if(!pulse_fall){
      check_pulse_fall(pulse);
    } 
    //signal states
    digitalWrite(DATA_PIN, 1);
    digitalWrite(STATUS_LED, LOW);


    //check transition
    if(pulse && pulse_fall){
      pulse_fall = false;
      curr_state = IDLE;
    }
    break;

  }

  previous_pulse_state = pulse;
  previous_latch_state = latch;

  //Then, upon a clock signal, shift off the new value and send that value
  // delay(200);
}

//Code to make sure that a lingering HIGH on the latch doesn't skip the FSM state. Does this by checking every cycle if the latch has fallen to 0
void check_pulse_fall(unsigned int pulse){
  if (previous_pulse_state == 1 & pulse == 0){
    pulse_fall = true;
  }
  else if(pulse == 0){
    pulse_fall = true;
  }
  else{
  pulse_fall = false;
  }
}

//Code to make sure that a lingering HIGH on the latch doesn't skip the FSM state. Does this by checking every cycle if the latch has fallen to 0
void check_latch_fall(unsigned int latch){
  if (latch == 0){
    latch_fall = true;
  }
}

void print_state(enum states state){
  switch (curr_state){
    case IDLE:
    Serial.println("IDLE");
    break;
    case A:
    Serial.println("A");
    break;
    case B:
    Serial.println("B");
    break;
    case SELECT:
    Serial.println("SELECT");
    break;
    case START:
    Serial.println("START");
    break;
    case UP:
    Serial.println("UP");
    break;
    case DOWN:
    Serial.println("DOWN");
    break;
    case LEFT:
    Serial.println("LEFT");
    break;
    case RIGHT:
    Serial.println("RIGHT");
    break;
  }
}
