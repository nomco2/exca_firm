#include "eeprom.h"
#include "auto_linear_scanning.h"


void cos_amend_value_init() {
  cos_amend_value = (float)EEPROMRead_int(1) / (float)100;
}


void receive_laser_data() {
  laser_35_dollars_loop();
  height_calculation = int(cos(abs(kalAngleY) * cos_amend_value) * laser_data_int);
}


unsigned long Motor_control_Millis = 0; //시간이 1분 지나면 자동으로 레이저 종료
void serial_read_processing_to_divided_mode(char read_data) {
  String json_name = "amend_value";
  String json_int;
  char motor_direction;
  int ms = 180;
  switch (read_data) {
    case 'd':
      continue_or_not = true;
      is_auto_scanning_start = false;
      laser_35_dollars.write('D');
      Motor_control_Millis = millis();
      laser_measuring_times = -1;
      break;
    case 'x':
      continue_or_not = false;
      is_auto_scanning_start = false;

      break;

    case 'o':
      laser_35_dollars.write('O');
      break;
    case 'c':
      laser_35_dollars.write('C');
      break;
    case 's':

      break;

    /* change amend value */
    case 'E':

      char data[3];
      Serial.readBytes(data, sizeof(data));
      EEPROMWrite_int(1, atoi(data));
      delay(100);
      cos_amend_value = (float)EEPROMRead_int(1) / (float)100;
      //        Serial.println(cos_amend_value);


      json_int = String((float)atoi(data) / (float)100);
      Serial.println(json_maker(&json_name, &json_int));


      break;

    case 'R':
      cos_amend_value = (float)EEPROMRead_int(1) / (float)100;

      json_int = String(cos_amend_value);
      Serial.println(json_maker(&json_name, &json_int));

      break;


    /*motor control */
    case 'm':
      Motor_control_Millis = millis();
      motor_direction = Serial.read();

      switch (motor_direction) {
        case '0':
          digitalWrite(2, true);
          digitalWrite(4, true);
          digitalWrite(5, true);
          digitalWrite(7, true);
          //          Serial.print("0");
          break;

        case '1': //left_up
          Motor_operation(ms, false, ms, false);
          break;
        case '2': //middle_up
          Motor_operation(ms, false, 0, true);
          break;
        case '3'://right_up
          Motor_operation(ms, false, ms, true);
          break;
        case '4'://left
          Motor_operation(0, true, ms, false);
          break;

        case '6'://right
          Motor_operation(0, true, ms, true);
          break;
        case '7'://left_down
          Motor_operation(ms, true, ms, false);
          break;
        case '8'://middle_down
          Motor_operation(ms, true, 0, true);
          break;
        case '9'://right_down
          Motor_operation(ms, true, ms, true);
          break;
      }
      break;

    //자동 스캔 명령
    case 'a':
      angle_term = Serial.read();
      laser_measuring_times = 0;
      laser_error_check_times = 0;
      continue_or_not = true;
      is_auto_scanning_start = true;
      //          auto_scan_motor_controler(angle_term);//모터 움직여서 각도 맞추기 -> while로 맞출때까지 무한 반복


      break;




  }
}




void D_processing() {
  char data = laser_35_dollars.read();
  boolean D_send_once = true;
  //    Serial.print(data);
  if (data == ' ') {
    D = true;
  } else if (data == 'm') {
    D = false;
    laser_data_int = data_string.toInt();
    data_string = "";

    String json_name[5] = {"distance", "angle1", "angle2", "height", "measuring_time"};
    String json_int[5];
    json_int[0] = String(laser_data_int);
    json_int[1] = String(kalAngleX);
    json_int[2] = String(kalAngleY);
    json_int[3] = String(height_calculation);
    json_int[4] = String(laser_measuring_times);

    Serial.println(json_maker(json_name, json_int));
    auto_scanning();
  }
  if (D && data != '.') {
    data_string += data;
    D_send_once = true;
  } else {
    if (D_send_once) {
      D_send_once = false;

    }
  }
  if (data == 'E') {
    laser_error_check_times++; //오류 횟수 저장
  }


}



void laser_35_dollars_loop() {
  if (laser_35_dollars.available()) {
    //    if (is_auto_scanning_start) {
    //      auto_scanning(); //오토 스캔 기능 부여 받으면
    //    } else {
    D_processing();  //result is saved the data_int
    //    }
  } else {
    if (continue_or_not) {
      delay(100);
      laser_35_dollars.write('D');
      //          Serial.println("d send");
    }
  }

  if (Serial.available()) {
    //    laser_35_dollars.write(Serial.read());
    char a = Serial.read();
    //      Serial.print(a);
    serial_read_processing_to_divided_mode(a);
  }

  if ((millis() - Motor_control_Millis) > 300000 && continue_or_not && !is_auto_scanning_start ) { // 5분 이상 컨트롤 없으면 레이저 종료
    continue_or_not = false;

    String json_name[4] = {"distance", "angle1", "angle2", "height"};
    String json_int[4];
    json_int[0] = "-1";
    json_int[1] = "0";
    json_int[2] = "0";
    json_int[3] = "0";
    Serial.println(json_maker(json_name, json_int));
  }



}
