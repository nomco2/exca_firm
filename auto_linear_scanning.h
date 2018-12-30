boolean is_auto_scanning_start = false;
int laser_measuring_times = -1;
int laser_error_check_times;
char angle_term;


void auto_scan_motor_controler() {
  int motor_speed = 200;

  switch (angle_term) {
    case '1':
      Motor_operation(motor_speed, false, 0, false); //각도가 높으면 아래로
      delay(500);
      break;
    case '2':
      Motor_operation(motor_speed, false, 0, false); //각도가 높으면 아래로
      delay(300);
      break;
    case '3':
      Motor_operation(motor_speed, false, 0, false); //각도가 높으면 아래로
      delay(100);
      break;
    case '4':
      Motor_operation(motor_speed, true, 0, false); //각도가 높으면 아래로
      delay(100);
      break;
    case '5':
      Motor_operation(motor_speed, true, 0, false); //각도가 높으면 아래로
      delay(300);
      break;
    case '6':
      Motor_operation(motor_speed, true, 0, false); //각도가 높으면 아래로
      delay(500);
      break;
  }

  Motor_operation(0, true, 0, false); //motor 멈춤

}


//소프트웨어 시리얼이 available이 될때 동작하도록
void auto_scanning() {

  if (is_auto_scanning_start) {

    laser_measuring_times++;//1회를 늘림
    
    if (laser_measuring_times > 2) { //3회 측정 후
      laser_measuring_times = 0;
      laser_error_check_times = 0;
      auto_scan_motor_controler();//모터 움직여서 각도 이동

    }
    if (laser_error_check_times > 9) { //오류가 9번 이상 나면 더 이상 진행하지 않음
      is_auto_scanning_start = false;
      continue_or_not = false;
    }
  }

}
