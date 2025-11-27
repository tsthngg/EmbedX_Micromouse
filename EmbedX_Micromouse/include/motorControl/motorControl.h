#ifndef Motor_control  
#define Motor_control
#include<Adruino.h>
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
extern float diameter;// đường kính bánh xe: mét
extern float distance; // khoảng cách cần di chuyển để đi từ ô này sang ô kia là 0.18 mét
extern float road_turn; // khoảng cách mỗi bánh phải đi để rẽ đủ 90 độ (số liệu đo đạc thực nghiệm)
extern float PWM_1cycle; // số xung xuất ra khi bánh quay đủ 1 vòng (số liệu tương đối từ đo đạc thực nghiệm)

extern float cycles_1square; // số vòng bánh xe để xe tiến được 1 ô
extern float cycles_turn; // số vòng bánh xe để xe rẽ được 90 độ

extern float PWM_straight; // số xung cần truyền cho encoder biết thời điểm dừng khi đủ khoảng cách mong muốn
extern float PWM_turn; // xung cần truyền cho encoder biết thời điểm rẽ đủ 90 độ

// Các hàm điều khiển động cơ sẽ được gọi ở thuật toán 
void setupMotor_control();
void turnLeft();
void turnRight();
void goStraight();
void turnBack();
void stop ();
#endif
