boolean is_auto_scanning_start = false;
int laser_measuring_times;
int laser_error_check_times;
double current_angle;
int user_select_scanning_angle;

int average_distance;
double average_angle1;
int average_counter;


void variables_reset(){
    average_distance = 0;
    average_angle1 = 0;
    average_counter = 0;
    laser_error_check_times = 0;
}


void auto_scan_motor_controler(double aimming_angle) {
  int motor_speed = 140;

  while (true) {
    delay(10);
    if ((kalAngleX > aimming_angle - 1) && (kalAngleX > aimming_angle + 1)) {
      break;
    }
    if ((kalAngleX > aimming_angle)) {
      Motor_operation(motor_speed, true, 0, false); //각도가 높으면 아래로
    } else {
      Motor_operation(motor_speed, false, 0, false); //각도가 높으면 위로
    }


  }
  Motor_operation(0, true, 0, false); //motor 멈춤

}


//소프트웨어 시리얼이 available이 될때 동작하도록
void auto_scanning() {

  if (!is_auto_scanning_start) { //아닐때는 계속 초기화
    laser_measuring_times = 0;
    current_angle = kalAngleX;
    variables_reset();
  } else {
    auto_scan_motor_controler(current_angle);//모터 움직여서 각도 맞추기 -> while로 맞출때까지 무한 반복
    average_distance += laser_data_int;
    average_angle1 += kalAngleX;
    average_counter++;
    
    if (average_counter > 4) { //5회 측정 후 평균 값이 나오면
      current_angle += user_select_scanning_angle; //다음 옮길 각도를 더하기
      
      average_distance = average_distance/average_counter;
      average_angle1 = average_angle1/average_counter;
      
      
      String json_name[4] = {"distance", "angle1", "angle2", "height"};
      String json_int[4];
      json_int[0] = String(average_distance);
      json_int[1] = String(average_angle1);
      json_int[2] = String(0);
      json_int[3] = String(laser_measuring_times);
      Serial.println(json_maker(json_name, json_int));
      
      laser_measuring_times++;//최종적으로 1회를 늘림
      variables_reset(); // 변수들은 초기화
    }
    if(laser_error_check_times > 9){ //오류가 9번 이상 나면 더 이상 진행하지 않음
      is_auto_scanning_start = false;
      continue_or_not = false;
    }
  }

}
