int enable1 = 3;
int enable2 = 6;
int motor_1_in1 = 2;
int motor_1_in2 = 4;
int motor_2_in1 = 5;
int motor_2_in2 = 7;


void motor_init(){
  pinMode(motor_1_in1, OUTPUT);      
  pinMode(motor_1_in2, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(motor_2_in1, OUTPUT);
  pinMode(motor_2_in2, OUTPUT);
  pinMode(enable2, OUTPUT);
}

void Motor_operation(int speed1, boolean reverse1, int speed2, boolean reverse2)
{
  analogWrite(enable1, speed1);
  digitalWrite(motor_1_in1, reverse1);
  digitalWrite(motor_1_in2, ! reverse1);
  analogWrite(enable2, speed2);
  digitalWrite(motor_2_in1, reverse2);
  digitalWrite(motor_2_in2, ! reverse2);
//  Serial.println(speed1 & ':' & speed2);

}
