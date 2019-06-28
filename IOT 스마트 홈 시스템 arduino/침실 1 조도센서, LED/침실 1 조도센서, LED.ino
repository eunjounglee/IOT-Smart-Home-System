int cds = A0;
int led = 4;
 
// 실행시 가장 먼저 호출되는 함수이며, 최초 1회만 실행됩니다.
// 변수를 선언하거나 초기화를 위한 코드를 포함합니다.
void setup() {
  Serial.begin(9600);          // 조도센서의 동작 상태를 확인하기 위하여 시리얼 통신을 설정합니다. (전송속도 9600bps)
  pinMode(led, OUTPUT);   // LED 핀을 OUTPUT으로 설정합니다.
}
 
// setup() 함수가 호출된 이후, loop() 함수가 호출되며,
// 블록 안의 코드를 무한히 반복 실행됩니다.
void loop() {
  // 조도센서로 부터 측정된 밝기 값을 읽어 cdsValue라는 변수에 저장합니다.
  int cdsValue = analogRead(cds);
 
  // 측정된 밝기 값를 시리얼 모니터에 출력합니다.
  Serial.print("cds =  ");
  Serial.println(cdsValue);
 
  // 조도센서로 부터 측정된 밝기 값이 500보다 크다면, 아래의 블록을 실행합니다.
  // 기준 밝기값은 주변 환경에 따라 바꿔주세요.
  if (cdsValue > 300) {
    digitalWrite(led, HIGH);
    Serial.println("LED ON (cds > 300)");
  }
 
  // 조도센서로 부터 측정된 밝기 값이 500보다 작다면, 아래의 블록을 실행합니다.
  else {
    digitalWrite(led, LOW);
    Serial.println("LED OFF (cds < 300)");
  }
  delay(200);
}
