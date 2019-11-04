
uint32_t lastTrg = 0;
bool lastButtonState = 1;
uint32_t lastButtonEvent = 0;

uint32_t SlowMode = 200;
uint32_t FastMode = 80;

uint32_t SpeedMode  = SlowMode;

class LED{
  public:
    bool D0 : 1;
    bool D1 : 1;
    bool D2 : 1;
    bool D3 : 1;
    bool D4 : 1;
    bool D5 : 1;
    bool D6 : 1;
    bool D7 : 1;
};

class Liu{
    LED * states;
    int stateNum;
    int state;
  public:
    Liu(){}
    Liu( LED * s, int num ){
      states = s;
      stateNum = num;
    }

    void LiuInit( LED * s, int num ){
      states = s;
      stateNum = num;
    }

    void NextState() {
      state ++;
      state = state % stateNum;
      
      digitalWrite( 2, states[state].D0 );
      digitalWrite( 3, states[state].D1 );
      digitalWrite( 4, states[state].D2 );
      digitalWrite( 5, states[state].D3 );
      digitalWrite( 6, states[state].D4 );
      digitalWrite( 7, states[state].D5 );
      digitalWrite( 8, states[state].D6 );
      digitalWrite( 9, states[state].D7 );
    }
};

uint8_t LedModeA[] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
};

uint8_t LedModeB[] = {
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
};

uint8_t LedModeC[] = {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00000000,
  B00100100,
  B01000010,
  B10000001,
};

uint8_t LedModeD[] = {
  B10101010,
  B01010101,
};

void LCDDisplay0(){
  digitalWrite( A0, LOW);
  digitalWrite( A1, LOW);
  digitalWrite( A2, LOW);
  digitalWrite( A3, LOW);
  digitalWrite( A4, HIGH);
  digitalWrite( A5, LOW);
  digitalWrite( 11, LOW);
}

void LCDDisplay1(){
  digitalWrite( A0, LOW);
  digitalWrite( A1, LOW);
  digitalWrite( A2, HIGH);
  digitalWrite( A3, HIGH);
  digitalWrite( A4, HIGH);
  digitalWrite( A5, HIGH);
  digitalWrite( 11, HIGH);
}

void LCDDisplay2(){
  digitalWrite( A0, LOW);
  digitalWrite( A1, HIGH);
  digitalWrite( A2, LOW);
  digitalWrite( A3, LOW);
  digitalWrite( A4, LOW);
  digitalWrite( A5, LOW);
  digitalWrite( 11, HIGH);
}

void LCDDisplay3(){
  digitalWrite( A0, LOW);
  digitalWrite( A1, LOW);
  digitalWrite( A2, HIGH);
  digitalWrite( A3, LOW);
  digitalWrite( A4, LOW);
  digitalWrite( A5, LOW);
  digitalWrite( 11, HIGH);
}

void LCDDisplay4(){
  digitalWrite( A0, LOW);
  digitalWrite( A1, LOW);
  digitalWrite( A2, HIGH);
  digitalWrite( A3, HIGH);
  digitalWrite( A4, LOW);
  digitalWrite( A5, HIGH);
  digitalWrite( 11, LOW);
}

Liu LedMode[4];

void setup() {
  for( int i = 2; i < 12; i ++){
    pinMode( i , OUTPUT );
  }

  for( int i = A0; i < A6; i ++){
    pinMode( i , OUTPUT );
  }

  //digitalWrite( A5, HIGH );

  pinMode( 12, INPUT_PULLUP );

  LedMode[0].LiuInit( (LED *)LedModeA, sizeof(LedModeA));
  LedMode[1].LiuInit( (LED *)LedModeB, sizeof(LedModeB));
  LedMode[2].LiuInit( (LED *)LedModeC, sizeof(LedModeC));
  LedMode[3].LiuInit( (LED *)LedModeD, sizeof(LedModeD));
}

uint8_t ModeID = 0;

void loop() {

  switch( ModeID ){
    case 0: LCDDisplay1(); break;
    case 1: LCDDisplay2(); break;
    case 2: LCDDisplay3(); break;
    case 3: LCDDisplay4(); break;
  }
  
  unsigned long now = millis();

  bool buttonState = digitalRead( 12 );
  if( (now - lastTrg) >= SpeedMode ){
    lastTrg = now;

    LedMode[ ModeID ].NextState();
  }

  if( lastButtonState != buttonState ){
    if( lastButtonState == 1 && buttonState == 0 && ( now - lastButtonEvent ) > 50 ){
      ModeID ++;
      ModeID = ModeID % 4;
    }

    if( ( now - lastButtonEvent ) > 800 ){
      SpeedMode = FastMode;
    }
    else{
      SpeedMode = SlowMode;  
    }
    
    lastButtonState = buttonState;
    lastButtonEvent = now;
  }
}
