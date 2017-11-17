/*
fft_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 64 // set to 64 point fft

#include <FFT.h> // include the library
int treasures = B000;

void setup() {
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc3
  DIDR0 = 0x01; // turn off the digital input for adc3

  pinMode(7, OUTPUT);
}

void loop() {
  find_treasures();
}

void find_treasures() {
  //while(1) { // reduces jitter
    if (ADMUX == 0x43){
      ADMUX = 0x44;       //a4
      DIDR0 = 0x05;
    }
    else if (ADMUX == 0x44){
      ADMUX = 0x43;       //a3
      DIDR0 = 0x04;
    }
  
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 128 ; i += 2) { // save 64 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    Serial.println("start");
    //Serial.println(ADMUX);
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.println(fft_log_out[i]); // send out the data      
    }
    
    if (fft_log_out[12] > 60) {         //7kHz
      treasures |= 1;
      digitalWrite(7, HIGH);
    }
    else if (fft_log_out[21] > 50) {    //12kHz      
      treasures |= 1 << 1;
      digitalWrite(7, HIGH);
    }
    else if (fft_log_out[28] > 53) {    //17kHz
      treasures |= 1 << 2;
      digitalWrite(7, HIGH);
    }        
    else {
      digitalWrite(7, LOW);
    }

    Serial.println(int(treasures)); // send out the data
  }
