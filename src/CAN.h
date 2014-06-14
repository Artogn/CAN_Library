/* Copyright (C) 2014

    Contributor:  Pedro Cevallos

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Acknowledgements:
Fabian Greif for the initial MCP2515 library http://www.kreatives-chaos.com/artikel/universelle-can-bibliothek
David Harding for his version of the MCP2515 library http://forum.arduino.cc/index.php/topic,8730.0.html
Kyle Crockett CANduino library with 16Mhz oscillator. (http://code.google.com/p/canduino/)
Nuno Alves for the help on Extended ID messaging
Stevenh for his work on library and all of the MCP research/work http://modelrail.otenko.com/arduino/arduino-controller-area-network-can

-------------------------------------------------------------------------------------------------------------
Change Log

DATE		VER		WHO			WHAT
07/07/13	0.1		PC		Modified and merge all MCP2515 libraries found. Stripped away most unused functions and corrected MCP2515 defs
09/12/13	0.2		PC		Added selectable CS SPI for CAN controller to use 1 IC to control several mcp2515
02/05/14	0.3		PC		Added filter and mask controls
05/01/14	0.4		PC		Cleaned up functions, variables and added message structures for J1939, CANopen and CAN.
05/07/14	1.0		PC		Released library to public through GitHub
-------------------------------------------------------------------------------------------------------------

*/


#ifndef _CAN_H_
#define _CAN_H_

#include <inttypes.h>
#include <Arduino.h>

// From the spec
#define CAN_DOMINANT                   0
#define CAN_RECESSIVE                  1

// Standard and Extended ID defines
#define CAN_STANDARD_FRAME             0
#define CAN_EXTENDED_FRAME             1

// For an 11 bit standard frame ID within a 16 bit variable, the first 5
// bits [15:11] are ignored
#define CAN_STANDARD_ID_MASK           0x07FF

// For a 29 bit extended frame ID within a 32 bit variable, the first 3
// bits [31:29] are ignored
#define CAN_EXTENDED_ID_MASK           0x1FFFFFFF

//

// Define the typical bitrate for CAN communication in kbps.
#define CAN_BPS_1M                     1000000
#define CAN_BPS_1000K                  1000000
#define CAN_BPS_800K                   800000
#define CAN_BPS_500K                   500000
#define CAN_BPS_250K                   250000
#define CAN_BPS_200K                   200000
#define CAN_BPS_125K                   125000
#define CAN_BPS_100K                   100000
#define CAN_BPS_83K                    83333  // According to ARINC 825, this is a thing
#define CAN_BPS_80K                    80000
#define CAN_BPS_50K                    50000
#define CAN_BPS_40K                    40000
#define CAN_BPS_33333                  33333
#define CAN_BPS_31K25                  31250
#define CAN_BPS_25K                    25000
#define CAN_BPS_20K                    20000
#define CAN_BPS_10K                    10000
#define CAN_BPS_5K                     5000

// ISO 11783-3:2014
// Section 5.13.3
// Controller response time and timeout defaults
#define CAN_TIMEOUT_TR 200
#define CAN_TIMEOUT_TH 500
#define CAN_TIMEOUT_T1 750
#define CAN_TIMEOUT_T2 1250
#define CAN_TIMEOUT_T3 1250
#define CAN_TIMEOUT_T4 1050


typedef struct __attribute__((__packed__))
{
  uint32_t id : 29;       // if (ide == CAN_RECESSIVE) { extended ID }
                          //   else { standard ID }
  uint8_t valid : 1;      // To avoid passing garbage frames around
  uint8_t rtr : 1;        // Remote Transmission Request Bit (RTR)
  uint8_t extended : 1;   // Identifier Extension Bit (IDE)
  uint32_t fid;           // family ID
  uint8_t priority : 4;	  // Priority but only important for TX frames and then only for special uses.
  uint8_t length : 4;     // Data Length
  uint16_t timeout;       // milliseconds, zero will disable waiting
  uint8_t data[8]; 			  // Message data
} CAN_Frame; // suffix of '_t' is reserved by POSIX for future use


// From http://www.cse.dmu.ac.uk/~eg/tele/CanbusIDandMask.html
// 
typedef struct __attribute__((__packed__))
{
  uint32_t id : 29;       // if (ide == CAN_RECESSIVE) { extended ID }
                          //   else { standard ID }
  uint8_t rtr : 1;        // Remote Transmission Request Bit (RTR)
  uint8_t extended : 1;   // Identifier Extension Bit (IDE)
  uint8_t data[2];        // Filter / Mask for message data
} CAN_Filter; // suffix of '_t' is reserved by POSIX for future use


class CANClass // Can't inherit from Stream
{
  public:
    virtual void begin(uint32_t bitrate);
    virtual void end();
    virtual uint8_t available();
    virtual CAN_Frame read();
    virtual void flush();
    virtual uint8_t write(const CAN_Frame&);

    //CAN_Frame& operator=(const CAN_Frame&);
};

//extern CANClass CAN;

#endif // _CAN_H_
