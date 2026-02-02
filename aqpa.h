#ifndef _SQPA_H_
#define _SQPA_H_




#define LOGAMP1_SLOPE          23.5 // mV/dB.  Used with Single Level Calibration
#define LOGAMP2_SLOPE          23.5 // mV/dB.  Used with Single Level Calibration
                                    // Typical value for AD8307 is 25mV/dB if AD input is ~1Mohm or higher
                                    // Typical value for ADL5519 is 22mV/dB
//-----------------------------------------------------------------------------
#define CAL_INP_QUALITY        12.0 // (dB) Minimum difference between the Fwd and Rev
                                    // signals to be allowed to calibrate
//-----------------------------------------------------------------------------
int var1;
#endif