# Milestone 2

## Objective
The primary goal of Milestone 2 is to be able to distinguish between three treasure frequencies--7kHz, 12kHz, and 17kHz--using our FFT with the IR phototransistor. Additionally, we implemented a short-range wall sensor in order to autonomously detect walls.

## Procedure

### Treasure Detection
Most of the work for this lab had been completed in [Lab 2](../labs/lab2.md), in which we mapped the Fourier transforms of the IR signal detected. To complete this milestone, we added LEDs to our preexisting IR detection circuit (with amplification) to individually light up upon the correct signal of a treasure. We referenced our amplified treasure detection data from the previous lab:

![IR Data (amplified)](../images/lab2/Treasure(7kHz,12kHz,17kHz).jpg)

This data shows that bins 48, 81, and 114 correspond to the 7kHz, 12kHz, and 17kHz treasures, respectively. On average, the frequency of these bins is between 140-160--to be safe, we set our threshold of detection at 130. We added a simple conditional to check the value of the desired bins as follows:

```

if (fft_log_out[48] > 130){         //7kHz, white
      digitalWrite(9, HIGH);  
    }
    else if (fft_log_out[81] > 130){    //12kHz, green
      digitalWrite(10, HIGH); 
    }
    else if (fft_log_out[114] > 130) {  //17kHz, red
      digitalWrite(11, HIGH);
    }
   else {                               //turn all off
      digitalWrite(9, LOW);   
      digitalWrite(10, LOW);  
      digitalWrite(11, LOW);
   }
```

We set up digital pin 9 to be a white LED, 10 to green, and 11 to red. These correspond to 7, 12, and 17kHz. When the treasures are presented in front of the IR phototransistor, the idea was that the corresponding LED would light up. Our test was successful, and can be seen in the following video:

[![Treasure Detection](http://img.youtube.com/vi/pzk-h_Un0u4/0.jpg)](http://www.youtube.com/watch?v=pzk-h_Un0u4)

For additional testing, we set the treasure to 8kHz, 13khz, and 18kHZ to ensure our bins were small enough to not detect extraneous frequencies. This was also successful--the LEDs did not light up. 

### Autonomous Wall Detection


## Build Changes

### Schmitt Triggers

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
