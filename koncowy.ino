#include<LiquidCrystal.h> //odczyt_temp.ino \/
LiquidCrystal Lcd(16,17,23,25,27,29);

#define TEMP     13 
#define Heater  10
#define Fan  44
#define a  -5.075572847372951e-07
#define b  8.878345580026639e-04
#define c  -0.619917675348230
#define d  2.546265516659611e+02

int x1;

double temperatura;
double uchyb;
double grzanie;
double calka;
double pochodna;
double pop_uchyb;
double temp_zadana;
int temp_nowa;
int licznik_lcd;

                    //odczyt_temp.ino /\

#define h HIGH
#define l LOW

// Limit Switches
//
#define X_MIN_PIN           3
#define X_MAX_PIN           2
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40


//Zmienne globalne
int x, y, z, off;
int KrokiX = 0, KrokiY = 0, KrokiZ = 0;
int Kroki;
int temp;
String s;
bool flaga = true;
void setup() {
  // Krancowki
  Serial.begin(57600);
  pinMode(X_MAX_PIN, INPUT);
  digitalWrite(X_MAX_PIN, HIGH);

  pinMode(Y_MAX_PIN, INPUT);
  digitalWrite(Y_MAX_PIN, HIGH);

  pinMode(Z_MAX_PIN, INPUT);
  digitalWrite(Z_MAX_PIN, HIGH);

  pinMode(X_MIN_PIN, INPUT);

  //stepper

  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  pinMode(X_DIR_PIN, l);
  pinMode(X_STEP_PIN, OUTPUT);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, LOW);
  pinMode(Y_DIR_PIN, l);
  pinMode(Y_STEP_PIN, OUTPUT);

  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);
  pinMode(Z_DIR_PIN, l);
  pinMode(Z_STEP_PIN, OUTPUT);

  //grzalka
  pinMode(Heater,OUTPUT);
  digitalWrite(Heater,LOW);
  pinMode(Fan,OUTPUT);
  digitalWrite(Fan,HIGH);
  Lcd.begin(20,4);
  Lcd.print("Hello");
  Lcd.setCursor(0,0);
  calka = 0;
  temp_zadana = 50;

}

void loop() {

 
  //Czytanie Krancowek
  x = digitalRead(X_MAX_PIN);
  y = digitalRead(Y_MAX_PIN);
  z = digitalRead(Z_MAX_PIN);
  off = digitalRead(X_MIN_PIN);

  while (Serial.available() > 0)
  {
  

    s = Serial.readStringUntil('\n');
    sscanf(s.c_str(), "x %d y %d z %d temp %d\n", &KrokiX, &KrokiY, &KrokiZ, &temp_nowa);
    temp_zadana = (double) temp_nowa;
    flaga = false;
    if (KrokiX < 0)
    {
      KrokiX = -KrokiX;
      digitalWrite(X_DIR_PIN, HIGH);
    }
    else
    {
      digitalWrite(X_DIR_PIN, LOW);

    }
    if (KrokiY < 0)
    {
      KrokiY = -KrokiY;
      digitalWrite(Y_DIR_PIN, HIGH);
    }
    else
    {
      digitalWrite(Y_DIR_PIN, LOW);

    }
    if (KrokiZ < 0)
    {
      KrokiZ = -KrokiZ;
      digitalWrite(Z_DIR_PIN, HIGH);
    }
    else
    {
      digitalWrite(Z_DIR_PIN, LOW);

    }

    //    temp = Serial.parseInt();
    //    if(temp>0){
    //      Kroki = temp;
    //    KrokiX = Kroki;
    //    KrokiY = Kroki;
    //    KrokiZ = Kroki;
    //    }
    //    Serial.println(Kroki);

  }


  //  Serial.print("X:");
  //  Serial.print(x);
  //  Serial.print("; Y:");
  //  Serial.print(y);
  //  Serial.print("; Z:");
  //  Serial.print(z);
  //  Serial.println();
  if (!x && KrokiX > 0 && !off)
  {
    digitalWrite(X_STEP_PIN, HIGH);
    KrokiX--;
  }
  if (!y && KrokiY > 0 && !off)
  {
    digitalWrite(Y_STEP_PIN, HIGH);
    KrokiY--;
  }
  if (!z && KrokiZ > 0 && !off)
  {
    digitalWrite(Z_STEP_PIN, HIGH);
    KrokiZ--;
  }

  if(KrokiX == 0 && KrokiY == 0 && KrokiZ == 0 && flaga == false)
  {
  Serial.println("OK");
  flaga = true;
  }
  
  delayMicroseconds(10);
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);
  delayMicroseconds(10);
  regulacja_temp(temp_zadana);

}

void regulacja_temp(double temp_zadana){
     
     
   //kontrola temp
  x1 = analogRead(TEMP);
  temperatura = a*(double)pow(x1,3) + b*(double)pow(x1,2)+c*double(x1)+d;
  
  //Serial.println(temperatura);
  pop_uchyb = uchyb;
  uchyb = temp_zadana - temperatura;
  calka = calka + uchyb;
  pochodna = (uchyb - pop_uchyb)/0.01;
  if( grzanie <255){
    grzanie = 255 * (uchyb/(temp_zadana - 30)) + 5*calka/1000 + 0.9*pochodna; 
  }
  else if( grzanie < 0 ){
    grzanie = 1;
  }
  else{
    grzanie = 255;
  }
  if (temperatura < temp_zadana)
  {
  
   //digitalWrite(Heater,LOW);
   analogWrite(Heater,floor(grzanie));
  
   }
  else
  {
   digitalWrite(Heater,LOW);
  
   }

  if(licznik_lcd > 1000){ 
  licznik_lcd = 0;
  Lcd.setCursor(0,0);
  Lcd.print("Temp:");
  Lcd.print(temperatura);
  Lcd.print(" C  ");
  Lcd.setCursor(0,1);
  Lcd.print("temp_zad: ");
  Lcd.print(temp_zadana);
  }
  licznik_lcd++;
  // kontrola temp 
}


