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
      Serial.print("distance:");
      Serial.print(laser_data_int);
      Serial.print(" /angle:");
      Serial.print(kalAngleY);
      Serial.print(" /height:");
      Serial.println(height_calculation);
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
          delay(10);
          laser_35_dollars.write('D');
//          Serial.println("d send");
        }
  }
  if (Serial.available()) {
//    laser_35_dollars.write(Serial.read());
      serial_read_processing_to_divided_mode(Serial.read());
  }


}
