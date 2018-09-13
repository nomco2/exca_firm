#include <SoftwareSerial.h>
SoftwareSerial laser_35_dollars(10, 11);





boolean D = false;
String data_string = "";
int laser_data_int = 0;
int height_calculation = 0;
float cos_amend_value;

boolean continue_or_not = false;

void laser_35_dollars_init(){
  laser_35_dollars.begin(19200);

}


















