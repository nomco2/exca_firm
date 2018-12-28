#include <EEPROM.h> 



//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWrite_int(int address, int value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);


      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
int EEPROMRead_int(int address)
      {
      //Read the 4 bytes from the eeprom memory.
      long int four = EEPROM.read(address);
      long int three = EEPROM.read(address + 1);
      long int two = EEPROM.read(address + 2);
      long int one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }
