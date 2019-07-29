#include <SoftwareSerial.h>

#define rx 2
#define tx 3
///const String dat PROGMEM ="<h1> RECIEVED </h1><br/><html><body><head><title> REGISTRATION </title></head><form action=\"http://192.168.43.219:80\"  method=POST><input type=\"submit\" name = \"Action\" value=\"START\"><br/><br/><input type=\"submit\" name = \"Action\" value=\"STOP\"></form>";
SoftwareSerial esp(rx, tx);
uint8_t car1 = 8;
uint8_t cargo = 4;
uint8_t carstop = 5;
uint8_t conn_led = 9;

void setup() {
  const char acess[] = "davics";
  const char mod[] = "OK";


  Serial.begin(38400);
  esp.begin(38400);

  milli(2000);

  pinMode(conn_led , OUTPUT);
  digitalWrite(conn_led , LOW);

  pinMode(cargo, OUTPUT);
  digitalWrite(cargo, LOW);

  pinMode(carstop , OUTPUT);
  digitalWrite(carstop, LOW);

  pinMode(car1, OUTPUT);
  digitalWrite(car1, LOW);

  { ///  RST scope......

    char store[100];

    esp.println("AT+RST");
    milli(1000);
    verify(store , mod);

  }               /// end of RST....

  clear_b();



  { /// CWMODE Scope...........

    char store[50];

    esp.println("AT+CWMODE_CUR=1");
    milli(1000);
    verify(store , mod);

  }            /// End of CWMODE

  clear_b();



  { /// CIPMUX SCOPE

    char store[50];

    esp.println("AT+CIPMUX=1");
    milli(1000);
    verify(store , mod);


  }          /// END of CIPMUX

  clear_b();


  { /// CIPSERVER
    char store[50];

    esp.println("AT+CIPSERVER=1,80");
    milli(1000);
    verify(store , mod);

  }          /// END of CIPSERVER
  clear_b();

  esp.print("AT+CWJAP=");
  esp.print("\"");
  esp.print(acess);
  esp.print("\"");
  esp.print(",");
  esp.println("\"""\"");
  milli(2000);

  bool conn = false;
  uint8_t e = 0;
  char store[100];
  unsigned long long tim = 0;

  while (!conn) {
    while (esp.available()) {
      store[e] = esp.read();
      Serial.print(store[e]);  /// test........
      ++e;
    }

    tim = millis();
    while (!esp.available()) {

      store[e] = '\0';
      if (millis() - tim >= 8000) {

        conn = true;
        break;
      }

    }

  } /// end of while !conn

  if (strstr(store, "OK")) {
    digitalWrite(conn_led , HIGH);
  }
  ///........................................

  esp.println("AT+CIFSR");
  milli(1000);
  conn = false;
  store[0] = NULL;
  e = 0;
  while (!conn) {
    while (esp.available()) {
      store[e] = esp.read();
      Serial.print(store[e]);  /// test........
      ++e;
    }

    tim = millis();
    while (!esp.available()) {

      store[e] = '\0';
      if (millis() - tim >= 1000) {

        conn = true;
        break;
      }

    }

  } /// end of while !conn

  digitalWrite(carstop, HIGH);

}  //// end of setup


void loop() {


  const char act1[] = "START";
  const char act2[] =  "STOP";







  char store[300];
  uint16_t a = 0;

  uint8_t connections = 0;
  bool conn = false;
  unsigned long long tim;

  if (esp.find("Action=")) {
    /// over = true;

    digitalWrite(conn_led , LOW);

    while (!conn) {
      while (esp.available()) {
        store[a] = esp.read();
        ++a;
      }

      tim = millis();
      while (!esp.available()) {
        store[a] = '\0';
        if (millis() - tim >= 3000) {
          conn = true;
          break;
        }
      } //
    }  /// !conn


    if (strstr(store , act1)) {
      digitalWrite(car1, HIGH);
      digitalWrite(cargo, HIGH);
      digitalWrite(carstop, LOW);
      digitalWrite(conn_led , HIGH);
    }

    if (strstr(store, act2)) {
      digitalWrite(car1, LOW);
      digitalWrite(cargo, LOW);
      digitalWrite(carstop, HIGH);
      digitalWrite(conn_led , HIGH);
    }

    Serial.println(store);

clear_b();
  }



}
void clear_b() {
  milli(1500);

  while (esp.available() > 0) {         /// function to erase any data left in buffer
    char a = esp.read();
  }

}



void milli(uint16_t time_t) {

  unsigned long long delayy =  millis();
  while (1) {

    if ( (millis() - delayy) >= time_t) {
      break;
    }
  }

}




void verify(char *store , char *check) {

  unsigned short re = 0;
  bool rest = false;




  while (1) {

    if (rest) {

      break;
    }


    if (esp.available() > 0) {
      store[re] = esp.read();
      Serial.print(store[re]);
      ++re;
    }
    short key = 0;
    short count = 0;
    short key2 = 0;
    short count2 = 0;

    for (short i = 0; i < re + 1; ++i) {
      key = 0;
      count = 0;
      key2 = 0;
      count2 = 0;

      if (rest) {
        break;
      }
      if ( store[i] == check[key]) {


        ++key;
        ++count;
        for (short p = i + 1; p < re + 1; ++p) {

          if (store[p] != check[key] ) {

            break;
          }

          if ( store[p] == check[key]) {
            ++count;

            if (count == strlen(check)) {
              rest = true;
              break;

            }
            ++key;
          }
        }

      }

    }


  }

}
