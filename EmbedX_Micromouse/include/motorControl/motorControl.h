#ifndef Motor_control  
#define Motor_control
#include <Arduino.h>

// Khai báo chân encoder
extern const int encoder_1A;
extern const int encoder_1B;
 
extern const int encoder_2A;
extern const int encoder_2B;

// Khai báo biến encoder 

extern volatile int encoder1Value;
extern volatile int encoder2Value;

// Chân động cơ trái
extern const int motorPWM_L;
extern const int motorDir1_L;
extern const int motorDir2_L;

// Chân động cơ phải
extern const int motorPWM_R;
extern const int motorDir1_R;
extern const int motorDir2_R;
// Thông số cơ học
//extern double duong_kinh_banh_xe = 0.065; // mét
//extern int so_xung_encoder = 11;
//extern double ti_le_banh_rang = 1.0;
//extern double khoang_cach_2_banh = 0.10; // mét (chưa có thông số chính xác)
// Các hàm điều khiển động cơ sẽ được gọi ở thuật toán 
void setupMotor_control();
void re_trai();
void re_phai();
void di_thang();
void quay_dau();
void dung_lai();
#endif
