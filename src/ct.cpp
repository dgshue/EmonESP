/*
 * -------------------------------------------------------------------
 * Custom code for ADS CT
 * -------------------------------------------------------------------
 */


#include "emonesp.h"
#include "ct.h"
#include "config.h"


#include <Wire.h>
#include <Adafruit_ADS1015.h>

double offsetI;
double filteredI;
double sqI,sumI;
int16_t sampleI;
double Irms;

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */


void
ads_setup() {
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.begin();
}

double squareRoot(double fg)
{
  double n = fg / 2.0;
  double lstX = 0.0;
  while (n != lstX)
  {
    lstX = n;
    n = (n + fg / n) / 2.0;
  }
  return n;
}

boolean calcIrms(String& data)
{

  int Number_of_Samples = 2048;
  /* Be sure to update this value based on the IC and the gain settings! */
  float multiplier = 0.250F;    /* ADS1115 @ +/- 4.096V gain (16-bit results) */
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleI = ads.readADC_Differential_0_1();

  // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
  //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI-offsetI)/1024);
    filteredI = sampleI - offsetI;
    //filteredI = sampleI * multiplier;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum
    sumI += sqI;
  }

  Irms = squareRoot(sumI / Number_of_Samples)*multiplier;

  data = "CT1:";
  data += Irms;

  //Reset accumulators
  sumI = 0;
//--------------------------------------------------------------------------------------

  return true;
}
