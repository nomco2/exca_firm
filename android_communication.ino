#include "eeprom.h"


void cos_amend_value_init(){
    cos_amend_value = (float)EEPROMRead_int(1)/(float)100;
}


void receive_laser_data(){
  laser_35_dollars_loop();
  height_calculation = int(cos(abs(kalAngleY)*cos_amend_value) * laser_data_int);
}



void serial_read_processing_to_divided_mode(char read_data){
  
  switch(read_data){
    case 'd':
      continue_or_not = true;
      laser_35_dollars.write('D');
    break;
    case 'x':
      continue_or_not = false;
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
        EEPROMWrite_int(1,atoi(data));
        cos_amend_value = (float)EEPROMRead_int(1)/(float)100;
        Serial.println(cos_amend_value);

//        cos_amend_value = (float)EEPROMRead_int(1)/(float)100;
//        Serial.println(cos_amend_value);

    break;

    /*motor control */
    case 'm':
      char motor_direction = Serial.read();
      switch(motor_direction){
        case '0':
          digitalWrite(2, true);
          digitalWrite(4, true);
          digitalWrite(5, true);
          digitalWrite(7, true);
          Serial.print("0");
          break;
        case '1':
          Motor_operation(255,true,255,true);
          Serial.print("1");

          break;
        case '2':
          Motor_operation(255,true,255,true);
          break;
        case '3':
          Motor_operation(255,true,255,true);
          break;
        case '4':
          Motor_operation(255,true,255,true);
          break;
        case '5':
          Motor_operation(255,true,255,true);
          break;
        case '6':
          Motor_operation(255,true,255,true);
          break;
        case '7':
          Motor_operation(255,true,255,true);
          break;
        case '8':
          Motor_operation(255,true,255,true);
          break;
      }
      break;


      
    
    
  }
}




void D_processing(){
    char data = laser_35_dollars.read();
    boolean D_send_once = true;
//    Serial.print(data);
    if(data == ' '){
      D = true;
    }else if(data == 'm'){
      D = false;
      laser_data_int = data_string.toInt();
      data_string = "";
      String Quotes = "'";
      Serial.print("[{");
      Serial.print(Quotes);
      Serial.print("distance");
      Serial.print(Quotes);
      Serial.print(":");
      Serial.print(laser_data_int);

      Serial.print(",");
      Serial.print(Quotes);
      Serial.print("angle");
      Serial.print(Quotes);
      Serial.print(":");
      Serial.print(kalAngleY);

      Serial.print(",");
      Serial.print(Quotes);
      Serial.print("height");
      Serial.print(Quotes);
      Serial.print(":");
      Serial.println(height_calculation);
      Serial.print("}]");
    }    
    if(D && data != '.'){
      data_string += data;
      D_send_once = true;
    }else{
      if(D_send_once){
        D_send_once = false;
        
      }
    }
    

}



void laser_35_dollars_loop(){
  if (laser_35_dollars.available()) {
   
    D_processing();  //result is saved the data_int
    
  }else{
        if(continue_or_not){
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


}
