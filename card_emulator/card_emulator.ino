//Magnetic Card emulator (track 2 only)
//Connect an H-Bridge to the left and right pins defined below
//Connect the H-Bridge outputs to a coil wrapped around a ferrous strip thin enough to insert into a card reader.
int leftPin = 6;
int rightPin = 7;
int clockSpeed = 500;
int clockHalf = 0;
char track2[255];

//Track 2 data
char tmp[40] = ";0000000000000000=00000000000000000000?";

void setup() {
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT); 
  for (int i = 0; i < 40; i++) {
    track2[i] = tmp[i];
  }
}

void writeLow() {
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
}

void writeBit(int lowOrHigh) {
  if(lowOrHigh == 1) {
    digitalWrite(rightPin, ((clockHalf == 0)?LOW:HIGH));
    digitalWrite(leftPin, ((clockHalf == 0)?HIGH:LOW));
    delayMicroseconds(clockSpeed);
    digitalWrite(rightPin, ((clockHalf == 0)?HIGH:LOW));
    digitalWrite(leftPin, ((clockHalf == 0)?LOW:HIGH));
    delayMicroseconds(clockSpeed);
  } else {
    digitalWrite(rightPin, ((clockHalf == 0)?LOW:HIGH));
    digitalWrite(leftPin, ((clockHalf == 0)?HIGH:LOW));
    delayMicroseconds(clockSpeed * 2);
    clockHalf = (clockHalf+1)%2;
  }
}

void writeChar(char c) {
  byte t = 0x00;
  switch (c) {
    case '0': t = 0x01; break;
    case '1': t = 0x10; break;
    case '2': t = 0x08; break;
    case '3': t = 0x19; break;
    case '4': t = 0x04; break;
    case '5': t = 0x15; break;
    case '6': t = 0x0d; break;
    case '7': t = 0x1c; break;
    case '8': t = 0x02; break;
    case '9': t = 0x13; break;
    case ':': t = 0x0b; break;
    case ';': t = 0x1a; break;
    case '<': t = 0x07; break;
    case '=': t = 0x16; break;
    case '>': t = 0x0e; break;
    case '?': t = 0x1f; break;
  }
  for (int i = 4; i >= 0; i--) {
    writeBit((t>>i)&0x01);
  }
}

int LRC(char* data, int length) {
  int lrc = 0;
  for (int i = 0; i < length; i++) {
    lrc ^= (data[i]-0x30);
  }
  return lrc+0x30;
}

void loop(){
  for(int i = 0; i < 20; i ++) {
    writeBit(0);
  }
  for (int i = 0; i < 39; i++) { 
    writeChar(track2[i]);
  }
  writeChar(LRC(track2, strlen(track2)));
  for(int i = 0; i < 20; i ++) {
    writeBit(0);
  }
  writeLow();
  delay(clockSpeed);
}
