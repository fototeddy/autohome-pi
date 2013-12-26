//
//  dht.cpp
//  
//
//  Created by Christopher Täufert on 26.12.13.
//
//

//#include "dht.h"

//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//


// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <iostream>

int MAXTIMINGS 100;

//#define DEBUG


int bits[250], data[100];
int bitidx = 0;


using namespace std;



void readDHT(int type, int pin){
        int counter = 0;
        int laststate = HIGH;
        int j=0;
        
        // Set GPIO pin to output
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
        
        bcm2835_gpio_write(pin, HIGH);
        usleep(500000);  // 500 ms
        bcm2835_gpio_write(pin, LOW);
        usleep(20000);
        
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
        
        data[0] = data[1] = data[2] = data[3] = data[4] = 0;
        
        // wait for pin to drop?
        while (bcm2835_gpio_lev(pin) == 1) {
            usleep(1);
        }
        
        // read data!
        for (int i=0; i< MAXTIMINGS; i++) {
            counter = 0;
            while ( bcm2835_gpio_lev(pin) == laststate) {
                counter++;
                //nanosleep(1);         // overclocking might change this?
                if (counter == 1000)
                    break;
            }
            laststate = bcm2835_gpio_lev(pin);
            if (counter == 1000) break;
            bits[bitidx++] = counter;
            
            if ((i>3) && (i%2 == 0)) {
                // shove each bit into the storage bytes
                data[j/8] <<= 1;
                if (counter > 200)
                    data[j/8] |= 1;
                j++;
            }
        }
    
        if ((j >= 39) &&
            (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
            // yay!
            /*if (type == DHT11)
                
            if (type == DHT22) {
                float f, h;
                h = data[0] * 256 + data[1];
                h /= 10;
                
                f = (data[2] & 0x7F)* 256 + data[3];
                f /= 10.0;
                if (data[2] & 0x80)  f *= -1;
                printf("Temp =  %.1f *C, Hum = %.1f \%\n", f, h);
            }*/
        }
    
    }

int main(void){
    readDHT(11, 17);
    cout << "temp: " << data[2] << " humi: " << data[0] << endl;
}