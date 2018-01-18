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

  double offsetCT1,offsetCT2;
  double filteredCT1,filteredCT2;
  double sqCT1,sumCT1;
  double sqCT2,sumCT2;
  int16_t sampleCT1,sampleCT2;
  double IrmsCT1,IrmsCT2;

  int Number_of_Samples = 2048;
  /* Be sure to update this value based on the IC and the gain settings! */
  float multiplier = 0.250F;    /* ADS1115 @ +/- 4.096V gain (16-bit results) */
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleCT1 = ads.readADC_Differential_0_1();
    sampleCT2 = ads.readADC_Differential_2_3();

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
    // then subtract this - signal is now centered on 0 counts.
    offsetCT1 = (offsetCT1 + (sampleCT1-offsetCT1)/1024);
    offsetCT2 = (offsetCT2 + (sampleCT2-offsetCT2)/1024);

    filteredCT1 = sampleCT1 - offsetCT1;
    filteredCT2 = sampleCT2 - offsetCT2;

    // Root-mean-square method current
    // 1) square current values
    sqCT1 = filteredCT1 * filteredCT1;
    sqCT2 = filteredCT2 * filteredCT2;
    // 2) sum
    sumCT1 += sqCT1;
    sumCT2 += sqCT2;
  }

  IrmsCT1 = squareRoot(sumCT1 / Number_of_Samples)*multiplier;
  IrmsCT2 = squareRoot(sumCT2 / Number_of_Samples)*multiplier;

  data = "CT1:";
  data += IrmsCT1;
  data += ",CT2:";
  data += IrmsCT2;

  //Reset accumulators
  sumCT1 = 0;
  sumCT2 = 0;
//--------------------------------------------------------------------------------------

  return true;
}
