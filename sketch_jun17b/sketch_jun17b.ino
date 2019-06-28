#include <LiquidCrystal.h>
#include <swRTC.h>
#include <Servo.h>

#define PIEZOPIN 6
#define LCD_BUTTONPIN 9
#define CDSPIN A0 //조도센서
#define CDS_LEDPIN 22
#define SERVOPIN 26
#define SERVO_BUTTONPIN 28
#define RGB_BUTTONPIN1 23
#define RPIN1 25
#define GPIN1 27
#define BPIN1 29
#define RGB_BUTTONPIN2 45
#define RPIN2 33
#define GPIN2 37
#define BPIN2 41
#define RGB_BUTTONPIN3 46
#define RPIN3 49
#define GPIN3 53
#define BPIN3 50
#define RGB_BUTTONPIN4 30
#define RPIN4 42
#define GPIN4 38
#define BPIN4 34

// D7(2), D6(3), D5(4), D4(5), E(11), RS(12)
LiquidCrystal lcd(12,11,8,4,3,2);        //RS 핀, E핀, 데이터 핀 4개
String lcdString = "";                   //객체 선언 : 출력 할 글자 저장
swRTC rtc; 

int temp = 1405;
Servo servo;
int servoAngle = 0;
int servoLimit = 180;

int rgbControl1 = 0;
int rgbControl2 = 0;
int rgbControl3 = 0;
int rgbControl4 = 0;

//AM PM을 구분해 주는 함수
void Set_AMPM(int hour) {
  if(hour >=12) 
    lcd.print("PM");
  else 
    lcd.print("AM");

  lcd.print(hour%12, DEC);     //시간 출력
}

//10보다 작은수를 출력할때 앞에 0을 출력하게 하는 함수
void Set_lowThanTen(int time) {
  if(time < 10) {
    lcd.print("0");
    lcd.print(time%10);
  }
  else
    lcd.print(time);
}

//유효한 알람시간인지 체크하는 함수
int checkTheAlarmClock(int time) {
  if(time/100 < 24 && time %100 < 60) {
    Serial.println("Success");
    return time;
  }
  else {
    Serial.println("Failed");
    return 0;
  }  
}

//알람이 울릴시간인지 체크하는 함수
void checkTheAlarmTime(int alarmHour, int alarmMinute) {
  if(alarmHour == rtc.getHours() && alarmMinute == rtc.getMinutes()) {
        analogWrite(PIEZOPIN, 128);
        Serial.println("arrrrrrrr");
    }
}

void setup() {                   
  lcd.begin(16,2);         //LCD 크기 지정, 2줄 16칸
  lcd.clear();             //화면 초기화
  
  rtc.stopRTC();           //정지
  rtc.setTime(14,04,58);    //시간, 분, 초 초기화
  rtc.setDate(24, 8, 2014);  //일, 월, 년 초기화 
  rtc.startRTC();          //시작
  
  pinMode(PIEZOPIN, OUTPUT);
  pinMode(LCD_BUTTONPIN, INPUT_PULLUP); 
  pinMode(CDS_LEDPIN, OUTPUT);
  pinMode(SERVO_BUTTONPIN, INPUT);
  servo.attach(SERVOPIN);
  pinMode(RPIN1, OUTPUT);
  pinMode(GPIN1, OUTPUT);
  pinMode(BPIN1, OUTPUT);
  pinMode(RGB_BUTTONPIN1, INPUT);
  pinMode(RPIN2, OUTPUT);
  pinMode(GPIN2, OUTPUT);
  pinMode(BPIN2, OUTPUT);
  pinMode(RGB_BUTTONPIN2, INPUT);
  pinMode(RPIN3, OUTPUT);
  pinMode(GPIN3, OUTPUT);
  pinMode(BPIN3, OUTPUT);
  pinMode(RGB_BUTTONPIN3, INPUT);
  pinMode(RPIN4, OUTPUT);
  pinMode(GPIN4, OUTPUT);
  pinMode(BPIN4, OUTPUT);
  pinMode(RGB_BUTTONPIN4, INPUT);
  Serial.begin(9600);      //시리얼 포트 초기화 
  Serial.begin(9600);                    //시리얼 통신 초기화
}

void loop() {
  int day;
  lcd.setCursor(0,0);                    //커서를 0,0에 지정 
  
  //1초 단위로 갱신하며 현재시간을 LCD에 출력
  Set_AMPM(rtc.getHours()); 
  lcd.print(":");
  Set_lowThanTen(rtc.getMinutes());
  lcd.print(":");
  Set_lowThanTen(rtc.getSeconds());
  //날짜를 LCD에 출력
  lcd.print("[");
  Set_lowThanTen(rtc.getMonth());
  lcd.print("/");
  Set_lowThanTen(rtc.getDay());
  lcd.print("]");
  //세팅된 알람시간을 LCD에 출력
  lcd.setCursor(0,1);
  lcd.print("Alarm ");
  Set_AMPM(temp/100);
  lcd.print(":");
  Set_lowThanTen(temp%100); 
  
  //1초마다 LCD갱신
  lcdString = "";                      //문자열 초기화
  lcd.print("               ");        //전 글씨 삭제
  delay(1000);
  
  //알람이 울릴 시간인지 체크
  checkTheAlarmTime(temp/100, temp%100);
   
  //스위치버튼이 눌렸을 경우 피에조센서의 소리를 0으로 하고 알람시간을 초기화 한다 
  if(!digitalRead(LCD_BUTTONPIN)) {
    temp = 0;
    day = 0;
    analogWrite(PIEZOPIN, 0);
    Serial.println("Alarm clock initialize");
    Serial.println("AM0:00");
  }
  //시리얼 통신을 통해 알람시간을 입력받고 시리얼 모니터에 출력 
  char theDay[4];
  int i = 0;
  if(Serial.available()) {
    while(Serial.available()) {
      theDay[i] = Serial.read();
      i++;
  }
    day = atoi(theDay);
    if(day/100 >= 12) {
      Serial.print("PM");
      Serial.print((day/100)-12);
    }
    else {
      Serial.print("AM");
      Serial.print(day/100);
    }
    Serial.print(":");
    if(day%100 < 10)
      Serial.print("0");
    Serial.println(day%100);
    temp = checkTheAlarmClock(day);
  }

  int cdsValue = analogRead(CDSPIN);

   // 측정된 밝기 값를 시리얼 모니터에 출력합니다.
  Serial.print("cds =  ");
  Serial.println(cdsValue);
 
  // 조도센서로 부터 측정된 밝기 값이 500보다 크다면, 아래의 블록을 실행합니다.
  // 기준 밝기값은 주변 환경에 따라 바꿔주세요.
  if (cdsValue > 500) {
    digitalWrite(CDS_LEDPIN, HIGH);
    Serial.println("LED ON (cds > 500)");
  }
 
  // 조도센서로 부터 측정된 밝기 값이 500보다 작다면, 아래의 블록을 실행합니다.
  else {
    digitalWrite(CDS_LEDPIN, LOW);
    Serial.println("LED OFF (cds < 500)");
  }
  delay(200);

  if(digitalRead(SERVO_BUTTONPIN) == LOW){
    servoAngle = (servoAngle + 20) % servoLimit;
    servo.write(servoAngle);
    delay(100);
  }
  else{
  }
  
  //RGB
  if(digitalRead(RGB_BUTTONPIN1) == LOW){
    if(rgbControl1 % 2 == 0){
      digitalWrite(RPIN1, LOW);
      digitalWrite(GPIN1, LOW);
      digitalWrite(BPIN1, LOW);
      Serial.println("off");
      rgbControl1++;
    }
    else if(rgbControl1 % 2 == 1){
      digitalWrite(RPIN1, 0);
      digitalWrite(GPIN1, 0);
      digitalWrite(BPIN1, 255);
      Serial.println("on");
      rgbControl1++;
    }
  }
    if(digitalRead(RGB_BUTTONPIN2) == LOW){
    if(rgbControl2 % 2 == 0){
      digitalWrite(RPIN2, LOW);
      digitalWrite(GPIN2, LOW);
      digitalWrite(BPIN2, LOW);
      rgbControl2++;
    }
    else if(rgbControl2 % 2 == 1){
      digitalWrite(RPIN2, 0);
      digitalWrite(GPIN2, 255);
      digitalWrite(BPIN2, 0);
      rgbControl2++;
    }
    }
    if(digitalRead(RGB_BUTTONPIN3) == LOW){
    if(rgbControl3 % 2 == 0){
      digitalWrite(RPIN3, LOW);
      digitalWrite(GPIN3, LOW);
      digitalWrite(BPIN3, LOW);
      rgbControl3++;
    }
    else if(rgbControl3 % 2 == 1){
      digitalWrite(RPIN3, 255);
      digitalWrite(GPIN3, 0);
      digitalWrite(BPIN3, 0);
      rgbControl3++;
    }
    }
    if(digitalRead(RGB_BUTTONPIN4) == LOW){
    if(rgbControl4 % 2 == 0){
      digitalWrite(RPIN4, LOW);
      digitalWrite(GPIN4, LOW);
      digitalWrite(BPIN4, LOW);
      Serial.println("ffffffffffffffff");
      rgbControl4++;
    }
    else if(rgbControl4 % 2 == 1){
      digitalWrite(RPIN4, 100);
      digitalWrite(GPIN4, 150);
      digitalWrite(BPIN4, 255);
      Serial.println("dddddddddddd");
      rgbControl4++;
    }
    }
  }
