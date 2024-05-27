#include <EEPROM.h>

bool print_to_Serial = true;
bool print_to_Serial_advanced = false;

//give a delay were the sensors data will not be read to not read multiple times the same object (values are in miliseconds and add 82 miliseconds corresponding to the average runtime)
int delay_beetween_laps = 1000;

//set trig and echo pin for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

//This is the threshold at witch the sensore will activate
int distance_precision = 200;

//this function is up here becaus it is used to update the last_adress_of_addr and last_adress_of_vals variables
unsigned int read_unsigned_int_from_EEPROM(unsigned int addr){
  unsigned int result = (EEPROM.read(addr)<<8) + EEPROM.read(addr+1);

  if (print_to_Serial_advanced == true){
    Serial.print ("The int at adress ");
    Serial.print (addr);
    Serial.print (" is ");
    Serial.println (result);
  }
  return result;
}

//stores the last adress of addr and of vals
unsigned long last_adress_of_addr = read_unsigned_int_from_EEPROM(3842);
unsigned long last_adress_of_vals = read_unsigned_int_from_EEPROM(3845);
unsigned long before_in_micro, current_micro;

void blink_led_forever(){
  //blinks led forever
  if (print_to_Serial==true){
    Serial.println("Led is blinking forever");
  }
  while (true){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
}

void got_error(byte error_number){

  if (print_to_Serial==true){
    Serial.print("Error : ");
    Serial.println(error_number);
  }
  int addr = 4000;
  while (EEPROM.read(addr) != 255 && addr <= 4092){
    addr += 1;
  }
  if (addr == 4092){
    byte a = EEPROM.read(addr) + 1;
    if (a !=255){
      EEPROM.write(4092, a);
    }
    //to show that there is no more memory for to store errors values
    blink_led_forever();
  }
  else {
    EEPROM.write (addr, error_number);
  }
  if (error_number <= 199){
    blink_led_forever();
  }
}

void write_unsigned_long_to_EEPROM(unsigned long addr, unsigned long val){
  byte byte1 = (val & 0xFF);
  byte byte2 = ((val >> 8) & 0xFF);
  byte byte3 = ((val >> 16) & 0xFF);
  byte byte4 = ((val >> 24) & 0xFF);
  
  EEPROM.update(addr, byte1);
  EEPROM.update(addr + 1, byte2);
  EEPROM.update(addr + 2, byte3);
  EEPROM.update(addr + 3, byte4);

  if (print_to_Serial_advanced == true){
    Serial.print (val);
    Serial.print (" has been printed at adress : ");
    Serial.print (addr);
    Serial.print (" , ");
    Serial.print (addr+1);
    Serial.print (" , ");
    Serial.print (addr+2);
    Serial.print (" and ");
    Serial.println (addr+3);
  }
}

unsigned long read_unsigned_long_from_EEPROM(unsigned int addr){
  unsigned long byte1 = EEPROM.read(addr);
  unsigned long byte2 = EEPROM.read(addr + 1);
  unsigned long byte3 = EEPROM.read(addr + 2);
  unsigned long byte4 = EEPROM.read(addr + 3);
  
  unsigned long result =  ((byte1 << 0) & 0xFF) + ((byte2 << 8) & 0xFFFF) + ((byte3 << 16) & 0xFFFFFF) + ((byte4 << 24) & 0xFFFFFFFF);

  if (print_to_Serial_advanced == true){
    Serial.print ("The unsigned long at adress ");
    Serial.print (addr); 
    Serial.print (" is ");
    Serial.println (result);
  }
  return result;
}

//value has to be from 0 to 255
void write_byte_to_EEPROM (unsigned int addr, unsigned int val){
  EEPROM.update (addr, val);
  if (print_to_Serial_advanced == true){
    Serial.print(val);
    Serial.print(" has been writen to adress ");
    Serial.println (addr);
  }
}

byte read_byte_to_EEPROM (unsigned int addr){
  byte value  = EEPROM.read (addr);
  if (print_to_Serial_advanced == true){
    Serial.print("The value at adress ");
    Serial.print (addr);
    Serial.print(" is ");
    Serial.println(value);
  }
  return value;
}

void erase_byte (unsigned int addr){
  EEPROM.update(addr, 0xFF);
  if (print_to_Serial_advanced == true){
    Serial.print("The byte at adress ");
    Serial.print (addr);
    Serial.print (" has been erased");
  }
}

void write_unsigned_int_to_EEPROM(unsigned int addr, unsigned int val){

  byte byte1 = (val >> 8) & 0xFF;
  byte byte2 = val & 0xFF;

  EEPROM.update(addr, byte1);
  EEPROM.update(addr+1, byte2);
  if (print_to_Serial_advanced == true){
    Serial.println ();
    Serial.print (val);
    Serial.print (" has been printed at adress : ");
    Serial.print (addr);
    Serial.print (" and ");
    Serial.println (addr+1);
  }
}

void erase_all_memory(){
  if (print_to_Serial==true){
    Serial.println("Erasing memory...");
  }
  for (int i=0; i < EEPROM.length(); i++ ){
    if (EEPROM.read(i) != 0xFF){
      EEPROM.update(i, 0xFF);
    }
  }
  if (print_to_Serial==true){
    Serial.println("All 4096 bytes of EEPROM have been erased.");
  }
  last_adress_of_addr = 0;
  last_adress_of_vals = 1281;
  write_unsigned_int_to_EEPROM(3842, 0);
  write_unsigned_int_to_EEPROM(3845, 1281);
}


void print_out_memory(){
  if (print_to_Serial==true){
    for (int i=0; i<EEPROM.length(); i++){
      Serial.print(i);
      Serial.print(" : ");
      Serial.print(EEPROM.read(i));
      Serial.print(" | ");
      if (i % 5 == 0){
        Serial.println();
      } 
    }
  }
}

bool check_check_if_memory_is_none(){
  bool result = true;

  if (print_to_Serial==true){
    Serial.println("Checking memory...");
  }

  for (int i =0; i < EEPROM.length(); i++){
    if (EEPROM.read(i) != 0xFF){
      if (print_to_Serial==true){
        Serial.print("At adress ");
        Serial.print(i);
        Serial.print(", there is the value : ");
        Serial.println(EEPROM.read(i));
      }
      result = false;
    }
  }
  if (result == true && print_to_Serial==true){
    Serial.println("Every byte of EEPROM is set to None");
  }
  return result;
}

//useful for debugging
void print_all_intervals() {
    unsigned int currentAddr = 0;
    unsigned int storedAddr;
    while (currentAddr < last_adress_of_addr) {
      storedAddr = read_unsigned_int_from_EEPROM(currentAddr);
      unsigned long interval = read_unsigned_long_from_EEPROM(storedAddr);
      if (print_to_Serial==true){
        Serial.print("Interval: ");
        Serial.print(interval);
        Serial.print(" at address: ");
        Serial.print(storedAddr);
        Serial.print(" corresponding to : ");
      }
      convert_interval_to_time(interval);
      
      currentAddr += 2;
    }
}

void convert_interval_to_time(unsigned long interval){
  //we substract 82 because this is the average time it takes to asigne micros to a variable
  if (print_to_Serial==true){
    interval -= 82;
    unsigned long us = interval % 1000;
    unsigned long ms = (interval / 1000) % 1000;
    unsigned long s = (interval / 1000000) % 60;
    unsigned long m = (interval / (1000000 * 60)) % 60;
    unsigned long h = (interval / (3600000000)) % 24;

    // Print the time units in a formatted manner
    if (h < 10) Serial.print("0");
    Serial.print(h);
    Serial.print(":");
    if (m < 10) Serial.print("0");
    Serial.print(m);
    Serial.print(":");
    if (s < 10) Serial.print("0");
    Serial.print(s);
    Serial.print(".");
    if (ms < 100) Serial.print("0");
    if (ms < 10) Serial.print("0");
    Serial.print(ms);
    Serial.print(".");
    if (us < 100) Serial.print("0");
    if (us < 10) Serial.print("0");
    Serial.println(us);
  }
}

void check_memory_left(){
  if (print_to_Serial == false){
    return;
  }
  int i = 0;
  //checking if there is still place in memory for adresses
  while (i <= 1280){
    if (read_unsigned_int_from_EEPROM(i) == 65535){
      break;
    }
    i+=2;
  }
  int z = 1281;
  //checking if there is still place in memory for values
  while (z <= 3841){
    if (read_unsigned_long_from_EEPROM(z) == 4294967295){
      break;
    }
    z+=4;
  }
  int s = 3842;
  //checking if there is still place in memory for errors and other values stored in byte 3842 and above 
  while (s <= 4096){
    if (EEPROM.read(s) == 0xFF){
      break;
    }
    s++;
  }

  // printing out the values remaning 
  Serial.print("There are ");
  if (1280 - i <= 0){
    Serial.print("no more (0)");
  } else{
    Serial.print(1280 - i);
  }
  Serial.println(" bytes left to store addresses");

  Serial.print("There are ");
  
  if (3841 - z <= 0){
    Serial.print("no more (0)");
  } else{
    Serial.print(3841 - z);
  }
  Serial.println(" bytes left to store values");

  Serial.print("There are ");
  if (4096 - z <= 0){
    Serial.print("no more (0)");
  } else{
    Serial.print(4096 - z);
  }
  Serial.println(" bytes left to store errors and similars values");
}

void print_leaderboard(){
  if (print_to_Serial==false){
    return;
  }
  Serial.println("Leader Board :");
  unsigned int currentAddr = 1281;
  unsigned long storedAddr = read_unsigned_long_from_EEPROM(currentAddr);
  int i = 0;
  // get the lenght i of the array to store all variables
  while (storedAddr <= 4294967290 && i < 641) {
      storedAddr = read_unsigned_long_from_EEPROM(currentAddr);
      currentAddr += 4;
      i++;
  }
  currentAddr = 1281;
  unsigned long arr [i];

  Serial.println(i);
  
  //appends to the aray every lap inetrval
  for (int x = 0; x<i; x++) {
      arr[x] = read_unsigned_long_from_EEPROM(currentAddr); 
      currentAddr += 4;

  if (i == 0) {
    Serial.println("No data found.");
    return;
  }
  }
  for (int z = 0; z < i; z++) {
    unsigned long smallest_value = 4294967290;
    int smallest_index = -1;
    for (int w = 0; w < i; w++) {
        if (arr[w] < smallest_value) {
            smallest_value = arr[w];
            smallest_index = w;
        }
    }
    if (smallest_index != -1) {
      if (z<10){
        Serial.print(0);
      }
      if (z<100){
        Serial.print(0);
      }
      Serial.print(z);
      Serial.print(" st : ");
      convert_interval_to_time(smallest_value);
      arr[smallest_index] = 4294967295; 
    }
  }
}

//This program is meant to run for laps that are under 71 min
unsigned long calculate_and_save_interval (unsigned long before_in_micro, unsigned long current_micro){
  unsigned long interval_micro = current_micro - before_in_micro;

  if (print_to_Serial_advanced==true){
    Serial.print("interval ");
    Serial.println(interval_micro);
  }

  //checks if micro has overflown 
  if (current_micro > before_in_micro){
    //the micro has overflown so little calculation
    interval_micro = (4294967294 - before_in_micro) + current_micro;
  }

  if (last_adress_of_addr + 2 < 1281){
    if (last_adress_of_vals + 4 < 3842){
      // write to EEPROM
      write_unsigned_int_to_EEPROM(last_adress_of_addr, last_adress_of_vals);
      write_unsigned_long_to_EEPROM(last_adress_of_vals, interval_micro);
      last_adress_of_addr = last_adress_of_addr + 2;
      write_unsigned_int_to_EEPROM(3842, last_adress_of_addr);
      last_adress_of_vals = last_adress_of_vals + 4;
      write_unsigned_int_to_EEPROM(3845, last_adress_of_vals);
    }
    else{
      //error for last_adress_of_vals limite 
      got_error(3);
    }
  }
  else{
    //error for last_adress_of_addr limite
    got_error(2);
  }
  return interval_micro;
}

void check_sensor(){
  float duration, distance;
  for (int i = 0;i < 100; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    if (print_to_Serial==true){
      Serial.print("Distance : ");
      Serial.println(distance);
    }
    if (distance < distance_precision){
      return;
    } 
    delay (500);
  }
  if (print_to_Serial==true){
    Serial.println("Sensor probleme");
  }

  got_error(1);
}

void ultrasonic_sensor_loop(){
  float duration, distance = 12000;

  while (distance > distance_precision){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;

    if (print_to_Serial==true){
      Serial.print("Distance : ");
      Serial.println(distance);
    }
  }
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  if (print_to_Serial==true || print_to_Serial_advanced == true){
    Serial.begin(9600);
  }




  if (print_to_Serial == true){
    print_leaderboard();
  }








  
  if (print_to_Serial_advanced == true){
    check_memory_left();
    //delay to be able to see if there are any values that may overflow
    delay (4000);
  }

  if (print_to_Serial==true){
    Serial.println("Checking sensor...");
  }
  check_sensor();
  if (print_to_Serial==true){
    Serial.println("Sensor is good");
  }

  //erases every single byte writen to memory
  //erase_all_memory()

  //print out the best scores : 
  //print_leaderboard()

  //checks if there are no values into memory and prints out every byte that is not equale to 0xFF
  //check_check_if_memory_is_none

  //prints out every single value from memory in a "nice" way even the bytes = 0xFF
  //print_out_memory()

  //for debugging, this function prints the every interval stored to memory
  //print_all_intervals();


  //The micros can overflow after roughly 71 mins
  before_in_micro = micros();

  //Starts to count after first detection by ultrasonic sensor and not when the arduino is pluged
  ultrasonic_sensor_loop();
  
  if (print_to_Serial==true){
    Serial.println("activate the sensor once to start first lap");
  }
  before_in_micro = micros();
}

void loop() {
  if (delay_beetween_laps >0){
    delay (delay_beetween_laps);
  }

  if (print_to_Serial == true){
    Serial.println();
    Serial.println("New lap");
    Serial.println();
  }
  ultrasonic_sensor_loop();

  //sets current_micro to the current time to not remove the running time of the programme from the lap
  current_micro = micros();

  if (print_to_Serial_advanced == true){
    //printing the values of before and current
    Serial.print("Before :");
    Serial.println(before_in_micro);
    Serial.print("Current :");
    Serial.println(current_micro);
  }

  //calculates interval
  calculate_and_save_interval(before_in_micro, current_micro);

  //before_in_micro = current_micro;
  before_in_micro = micros();
}


//The End
