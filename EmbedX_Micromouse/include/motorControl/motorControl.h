#ifndef Motor_control  
#define Motor_control
#include <Arduino.h>
#include <PID_v1.h>
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

// Khai báo thông số động cơ và tính toán xung di chuyển 
extern float duong_kinh_banh_xe;//mét
extern float kc ; // khoảng cách cần di chuyển để đi từ ô này sang ô kia là 0.18 mét
extern float re_trai_phai; // khoảng cách mỗi bánh phải đi để rẽ đủ 90 độ (số liệu đo đạc thực nghiệm)
extern float so_xung_1_vong_banh_xe ; // số xung xuất ra khi bánh quay đủ 1 vòng (số liệu tương đối từ đo đạc thực nghiệm)

extern float so_vong_di_thang; // số vòng bánh xe để xe tiến được 1 ô
extern float so_vong_re_trai_phai; // số vòng bánh xe để xe rẽ được 90 độ

extern float xung_can_di_thang; // số xung cần truyền cho encoder biết thời điểm dừng khi đủ khoảng cách mong muốn
extern float xung_can_re_trai_phai; // xung cần truyền cho encoder biết thời điểm rẽ đủ 90 độ

// Các hàm điều khiển động cơ sẽ được gọi ở thuật toán 
void setupMotor_control();
void re_trai();
void re_phai();
void di_thang();
void quay_dau();
void dung_lai();
#endif
