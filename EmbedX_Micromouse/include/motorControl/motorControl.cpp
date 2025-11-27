#include "Motor_control.h"
#include <PID_v1.h>

// Định nghĩa chân động cơ
// Khai báo chân encoder 
const int encoder1_A = 34;
const int encoder1_B = 35;

const int encoder2_A = 32;
const int encoder2_B = 33;

// Khai báo biến encoder
volatile int encoder1Value = 0;
volatile int encoder2Value = 0;

// Chân động cơ trái
const int motorPWM_L = 18;
const int motorDir1_L = 26;
const int motorDir2_L = 27;  

// Chân động cơ phải
const int motorPWM_R = 19;
const int motorDir1_R = 12;
const int motorDir2_R = 13;

// Khai báo thông số động cơ và tính toán xung di chuyển 
float duong_kinh_banh_xe = 0.043;//mét
float kc = 0.18; // khoảng cách cần di chuyển để đi từ ô này sang ô kia là 0.18 mét
float re_trai_phai = 0.08639379797; // khoảng cách mỗi bánh phải đi để rẽ đủ 90 độ (số liệu đo đạc thực nghiệm)
float so_xung_1_vong_banh_xe = 1060; // số xung xuất ra khi bánh quay đủ 1 vòng (số liệu tương đối từ đo đạc thực nghiệm)

float so_vong_di_thang = kc/(PI*duong_kinh_banh_xe); // số vòng bánh xe để xe tiến được 1 ô
float so_vong_re_trai_phai = re_trai_phai/(PI*duong_kinh_banh_xe); // số vòng bánh xe để xe rẽ được 90 độ

float xung_can_di_thang = (so_vong_di_thang*so_xung_1_vong_banh_xe); // số xung cần truyền cho encoder biết thời điểm dừng khi đủ khoảng cách mong muốn
float xung_can_re_trai_phai = so_vong_re_trai_phai*   so_xung_1_vong_banh_xe; // xung cần truyền cho encoder biết thời điểm rẽ đủ 90 độ

// PID 
float Kp = 0.2;
float Ki = 0.6; //PID()
float Kd = 0.5; 

int last_time = 0;
float last_error = 0;
int encoderValuecr = 0;
float e_tich_luy = 0;

void encoder1_isr(){
      int A = digitalRead(encoder1_A);
      int B = digitalRead(encoder1_B);
      if( (A == HIGH) != (B == HIGH) ){
            encoder1Value++;
      }
      else{
            encoder1Value--;
      }
}

void encoder2_isr(){
      int A = digitalRead(encoder2_A);
      int B = digitalRead(encoder2_B);
      if( (A == HIGH) != (B == HIGH)){
           encoder2Value ++;
      }
      else{
           encoder2Value--;
      }
}

void setupMotor_control() {
  pinMode(encoder1_A, INPUT_PULLUP);
  pinMode(encoder1_B, INPUT_PULLUP);

  pinMode(encoder2_A, INPUT_PULLUP);
  pinMode(encoder2_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoder1_A), encoder1_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2_A), encoder2_isr, CHANGE);


  pinMode(motorPWM_L, OUTPUT);
  pinMode(motorDir1_L, OUTPUT);
  pinMode(motorDir2_L, OUTPUT);

  pinMode(motorPWM_R, OUTPUT);
  pinMode(motorDir1_R, OUTPUT);
  pinMode(motorDir2_R, OUTPUT);

  ledcSetup(0, 5000, 8);
  ledcAttachPin(motorPWM_L, 0);

  ledcSetup(1, 5000, 8);
  ledcAttachPin(motorPWM_R, 1);
}
// cùng chiều kim đồng hồ encoder1 nhận giá trị dương encoder2 nhận giá trị âm
// muốn tiến bánh 1 quay thuận bánh 2 quay ngược
// muốn rẽ trái bánh 1 quay thuận bánh 2 quay thuận
// muốn rẽ phải bánh 1 quay ngược bánh 2 quay ngược

void di_thang () {

      encoder1Value = 0; // reset giá trị encoder 1 sau mỗi lần gọi hàm
      encoder2Value = 0; // reset giá trị encoder 2 sau mỗi lần gọi hàm

      digitalWrite(motorDir1_L, LOW);
      digitalWrite(motorDir2_L, HIGH);
      ledcWrite(0,180);

      digitalWrite(motorDir1_R, HIGH);
      digitalWrite(motorDir2_R, LOW);
      ledcWrite(1,180);

      ledcWrite(1,0);// lệnh dừng 1 sau khi số xung đạt tiêu chuẩn
      ledcWrite(0,0);// lệnh dừng 2 sau khi số xung đạt chuẩn
       while((encoder1Value < (long)xung_can_re_trai_phai)){
            Serial.println(encoder1Value);
            ledcWrite(0,0);
      }
      while((encoder2Value < (long)(0 - xung_can_re_trai_phai))){
            Serial.println(encoder2Value);
            ledcWrite(1,0);
      }
      
}

void re_trai() {
      encoder1Value = 0;
      encoder2Value = 0;

      digitalWrite(motorDir1_L, LOW);
      digitalWrite(motorDir2_L, HIGH);
      ledcWrite(0,180);

      digitalWrite(motorDir1_R, LOW);
      digitalWrite(motorDir2_R, HIGH);
      ledcWrite(1,180);

      while((encoder1Value < (long)xung_can_re_trai_phai)){
            Serial.println(encoder1Value);
            ledcWrite(0,0);
      }
      while((encoder2Value < (long)(0 - xung_can_re_trai_phai))){
            Serial.println(encoder2Value);
            ledcWrite(1,0);
      }
      
}  

void re_phai() {
      encoder1Value = 0;
      encoder2Value = 0;
      digitalWrite(motorDir1_L, HIGH);
      digitalWrite(motorDir2_L, LOW);
      ledcWrite(0,180);

      digitalWrite(motorDir1_R, HIGH);
      digitalWrite(motorDir2_R, LOW);
      ledcWrite(1,180);
      while((encoder1Value > (long)(0-xung_can_re_trai_phai))){
            Serial.println(encoder1Value);
            ledcWrite(0,0);
      }
      while((encoder2Value > (long)(xung_can_re_trai_phai))){
            Serial.println(encoder2Value);
            ledcWrite(1,0);
      }  
      
}
 
void dung_lai() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

void quay_dau(){
      encoder1Value = 0;
      encoder2Value = 0;
  
      digitalWrite(motorDir1_L, HIGH);
      digitalWrite(motorDir2_L, LOW);
      ledcWrite(0,180);

      digitalWrite(motorDir1_R, HIGH);
      digitalWrite(motorDir2_R, LOW);
      ledcWrite(1,180);
      while((encoder1Value == 2*(0-xung_can_re_trai_phai))){
            Serial.println(encoder1Value);
            ledcWrite(0,0);
      }
      while((encoder2Value == 2*xung_can_re_trai_phai)){
            Serial.println(encoder2Value);
            ledcWrite(1,0);
      }
      
}
