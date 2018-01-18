/*
 * -------------------------------------------------------------------
 * Custom code for ADS CT
 * -------------------------------------------------------------------
 */
#include <Arduino.h>

extern void ads_setup();


// -------------------------------------------------------------------
// Read input sent via the web_server or serial.
//
// data: if true is returned data will be updated with the new line of
//       input
// -------------------------------------------------------------------
extern boolean calcIrms(String& data);
