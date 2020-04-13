/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The cpu_map.h files serve as a central pin mapping selection file for different
   processor types or alternative pin layouts. This version of Grbl officially supports
   only the Arduino Mega328p. */


#ifndef cpu_map_h
#define cpu_map_h

// Helper macros for operations on bits
#define IS_SET(port,bit) (((port) & (bit)) == (bit))
#define SET_ON(port,bit) ((port) |= (bit))
#define SET_OFF(port,bit) ((port) &= ~(bit))

// Helper macros for operations on registers
#define SET_SINGLE_DDR(axis, port, type) { SET_ON(DDR##port, INTERNAL_##axis##_##type); }
#define WRITE_SINGLE(val, axis, port, type) { if (IS_SET(val, (1<<axis##_##type##_BIT))) SET_ON(PORT##port, INTERNAL_##axis##_##type); else SET_OFF(PORT##port, INTERNAL_##axis##_##type); }

#ifdef CPU_MAP_ATMEGA328P // (Arduino Uno) Officially supported by Grbl.

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins.
  #define INTERNAL_X_STEP  (1<<2)
  #define INTERNAL_Y_STEP  (1<<3)
  #define INTERNAL_Z_STEP  (1<<4)
  #define X_STEP_BIT       1  // Dummy unique value
  #define Y_STEP_BIT       2  // Dummy unique value
  #define Z_STEP_BIT       3  // Dummy unique value
  #define STEP_MASK        ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  #define SET_STEP_DDR()   { SET_SINGLE_DDR(X, D, STEP); SET_SINGLE_DDR(Y, D, STEP); SET_SINGLE_DDR(Z, D, STEP); }
  #define WRITE_STEP_PORT(val) { WRITE_SINGLE(val, X, D, STEP); WRITE_SINGLE(val, Y, D, STEP); WRITE_SINGLE(val, Z, D, STEP); }

  // Define step direction output pins.
  #define INTERNAL_X_DIRECTION  (1<<5)
  #define INTERNAL_Y_DIRECTION  (1<<6)
  #define INTERNAL_Z_DIRECTION  (1<<7)
  #define X_DIRECTION_BIT       4  // Dummy unique value
  #define Y_DIRECTION_BIT       5  // Dummy unique value
  #define Z_DIRECTION_BIT       6  // Dummy unique value

  #define SET_DIRECTION_DDR() { SET_SINGLE_DDR(X, D, DIRECTION); SET_SINGLE_DDR(Y, D, DIRECTION); SET_SINGLE_DDR(Z, D, DIRECTION); }
  #define WRITE_DIRECTION_PORT(val) { WRITE_SINGLE(val, X, D, DIRECTION); WRITE_SINGLE(val, Y, D, DIRECTION); WRITE_SINGLE(val, Z, D, DIRECTION); }

  // Define stepper driver enable/disable output pin.
  #define INTERNAL_XYZ_ENABLE  (1<<0)

  #define SET_STEPPERS_DISABLE_DDR() { SET_ON(DDRB, INTERNAL_XYZ_ENABLE); }
  #define WRITE_STEPPERS_DISABLE_PORT_ON() { SET_ON(PORTB, INTERNAL_XYZ_ENABLE); }
  #define WRITE_STEPPERS_DISABLE_PORT_OFF() { SET_OFF(PORTB, INTERNAL_XYZ_ENABLE); }

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR        DDRB
  #define LIMIT_PIN        PINB
  #define LIMIT_PORT       PORTB
  #define X_LIMIT_BIT      1  // Uno Digital Pin 9
  #define Y_LIMIT_BIT      2  // Uno Digital Pin 10
  #ifdef VARIABLE_SPINDLE // Z Limit pin and spindle enabled swapped to access hardware PWM on Pin 11.
    #define Z_LIMIT_BIT	   4 // Uno Digital Pin 12
  #else
    #define Z_LIMIT_BIT    3  // Uno Digital Pin 11
  #endif
  #if !defined(ENABLE_DUAL_AXIS)
    #define LIMIT_MASK     ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
  #endif
  #define LIMIT_INT        PCIE0  // Pin change interrupt enable pin
  #define LIMIT_INT_vect   PCINT0_vect
  #define LIMIT_PCMSK      PCMSK0 // Pin change interrupt register

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       DDRC
  #define CONTROL_PIN       PINC
  #define CONTROL_PORT      PORTC
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     PCMSK1 // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       DDRC
  #define PROBE_PIN       PINC
  #define PROBE_PORT      PORTC
  #define PROBE_BIT       5  // Uno Analog Pin 5
  #define PROBE_MASK      (1<<PROBE_BIT)

  #if !defined(ENABLE_DUAL_AXIS)

    // Define flood and mist coolant enable output pins.
    #define COOLANT_FLOOD_DDR   DDRC
    #define COOLANT_FLOOD_PORT  PORTC
    #define COOLANT_FLOOD_BIT   3  // Uno Analog Pin 3
    #define COOLANT_MIST_DDR   DDRC
    #define COOLANT_MIST_PORT  PORTC
    #define COOLANT_MIST_BIT   4  // Uno Analog Pin 4

    // Define spindle enable and spindle direction output pins.
    #define SPINDLE_ENABLE_DDR    DDRB
    #define SPINDLE_ENABLE_PORT   PORTB
    // Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
    #ifdef VARIABLE_SPINDLE
      #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
        // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
        #define SPINDLE_ENABLE_BIT    5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
      #else
        #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
      #endif
    #else
      #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
    #endif
    #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
      #define SPINDLE_DIRECTION_DDR   DDRB
      #define SPINDLE_DIRECTION_PORT  PORTB
      #define SPINDLE_DIRECTION_BIT   5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
    #endif

    // Variable spindle configuration below. Do not change unless you know what you are doing.
    // NOTE: Only used when variable spindle is enabled.
    #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
    #ifndef SPINDLE_PWM_MIN_VALUE
      #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
    #endif
    #define SPINDLE_PWM_OFF_VALUE     0
    #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
    #define SPINDLE_TCCRA_REGISTER    TCCR2A
    #define SPINDLE_TCCRB_REGISTER    TCCR2B
    #define SPINDLE_OCR_REGISTER      OCR2A
    #define SPINDLE_COMB_BIT          COM2A1

    // Prescaled, 8-bit Fast PWM mode.
    #define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
    // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
    // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
    // #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
    #define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

    // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
    #define SPINDLE_PWM_DDR   DDRB
    #define SPINDLE_PWM_PORT  PORTB
    #define SPINDLE_PWM_BIT   3    // Uno Digital Pin 11
  
  #else

    // Dual axis feature requires an independent step pulse pin to operate. The independent direction pin is not 
    // absolutely necessary but facilitates easy direction inverting with a Grbl $$ setting. These pins replace 
    // the spindle direction and optional coolant mist pins.

    #ifdef DUAL_AXIS_CONFIG_PROTONEER_V3_51
      // NOTE: Step pulse and direction pins may be on any port and output pin.
      #define INTERNAL_DUAL_STEP  (1<<4)
      #define DUAL_STEP_BIT      1  // Dummy unique value
      #define STEP_MASK_DUAL      ((1<<DUAL_STEP_BIT))

      #define SET_STEP_DDR_DUAL() { SET_SINGLE_DDR(DUAL, C, STEP); }
      #define WRITE_STEP_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, C, STEP); }

      #define INTERNAL_DUAL_DIRECTION (1<<3)
      #define DUAL_DIRECTION_BIT   2  // Dummy unique value

      #define SET_DIRECTION_DDR_DUAL() { SET_SINGLE_DDR(DUAL, D, DIRECTION); }
      #define WRITE_DIRECTION_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, D, DIRECTION); }

      // NOTE: Dual axis limit is shared with the z-axis limit pin by default. Pin used must be on the same port
      // as other limit pins.
      #define DUAL_LIMIT_BIT    Z_LIMIT_BIT
      #define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<DUAL_LIMIT_BIT))

      // Define coolant enable output pins.
      // NOTE: Coolant flood moved from A3 to A4. Coolant mist not supported with dual axis feature on Arduino Uno.
      #define COOLANT_FLOOD_DDR   DDRB
      #define COOLANT_FLOOD_PORT  PORTB
      #define COOLANT_FLOOD_BIT   5  // Uno Digital Pin 13

      // Define spindle enable output pin.
      // NOTE: Spindle enable moved from D12 to A3 (old coolant flood enable pin). Spindle direction pin is removed.
      #define SPINDLE_ENABLE_DDR    DDRB
      #define SPINDLE_ENABLE_PORT   PORTB
      #ifdef VARIABLE_SPINDLE
        // NOTE: USE_SPINDLE_DIR_AS_ENABLE_PIN not supported with dual axis feature.
        #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
      #else
        #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
      #endif

      // Variable spindle configuration below. Do not change unless you know what you are doing.
      // NOTE: Only used when variable spindle is enabled.
      #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
      #ifndef SPINDLE_PWM_MIN_VALUE
        #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
      #endif
      #define SPINDLE_PWM_OFF_VALUE     0
      #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
      #define SPINDLE_TCCRA_REGISTER    TCCR2A
      #define SPINDLE_TCCRB_REGISTER    TCCR2B
      #define SPINDLE_OCR_REGISTER      OCR2A
      #define SPINDLE_COMB_BIT          COM2A1

      // Prescaled, 8-bit Fast PWM mode.
      #define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
      // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
      // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
      // #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
      #define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

      // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
      #define SPINDLE_PWM_DDR   DDRB
      #define SPINDLE_PWM_PORT  PORTB
      #define SPINDLE_PWM_BIT   3    // Uno Digital Pin 11
    #endif

    // NOTE: Variable spindle not supported with this shield.
    #ifdef DUAL_AXIS_CONFIG_CNC_SHIELD_CLONE
      // NOTE: Step pulse and direction pins may be on any port and output pin.
      #define INTERNAL_DUAL_STEP  (1<<4)
      #define DUAL_STEP_BIT      1  // Dummy unique value
      #define STEP_MASK_DUAL      ((1<<DUAL_STEP_BIT))

      #define SET_STEP_DDR_DUAL() { SET_SINGLE_DDR(DUAL, B, STEP); }
      #define WRITE_STEP_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, B, STEP); }

      #define INTERNAL_DUAL_DIRECTION (1<<5)
      #define DUAL_DIRECTION_BIT   2  // Dummy unique value

      #define SET_DIRECTION_DDR_DUAL() { SET_SINGLE_DDR(DUAL, B, DIRECTION); }
      #define WRITE_DIRECTION_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, B, DIRECTION); }

      // NOTE: Dual axis limit is shared with the z-axis limit pin by default.
      #define DUAL_LIMIT_BIT    Z_LIMIT_BIT
      #define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<DUAL_LIMIT_BIT))

      // Define coolant enable output pins.
      // NOTE: Coolant flood moved from A3 to A4. Coolant mist not supported with dual axis feature on Arduino Uno.
      #define COOLANT_FLOOD_DDR   DDRC
      #define COOLANT_FLOOD_PORT  PORTC
      #define COOLANT_FLOOD_BIT   4  // Uno Analog Pin 4

      // Define spindle enable output pin.
      // NOTE: Spindle enable moved from D12 to A3 (old coolant flood enable pin). Spindle direction pin is removed.
      #define SPINDLE_ENABLE_DDR    DDRC
      #define SPINDLE_ENABLE_PORT   PORTC
      #define SPINDLE_ENABLE_BIT    3  // Uno Analog Pin 3
    #endif

  #endif

#endif

#ifdef CPU_MAP_ANET_A8

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART0_RX_vect
  #define SERIAL_UDRE   USART0_UDRE_vect

  // Define step pulse output pins.
  #define INTERNAL_X_STEP  (1<<7)
  #define INTERNAL_Y_STEP  (1<<6)
  #define INTERNAL_Z_STEP  (1<<3)
  #define X_STEP_BIT       1  // Dummy unique value
  #define Y_STEP_BIT       2  // Dummy unique value
  #define Z_STEP_BIT       3  // Dummy unique value
  #define STEP_MASK        ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  #define SET_STEP_DDR()   { SET_SINGLE_DDR(X, D, STEP); SET_SINGLE_DDR(Y, C, STEP); SET_SINGLE_DDR(Z, B, STEP); }
  #define WRITE_STEP_PORT(val) { WRITE_SINGLE(val, X, D, STEP); WRITE_SINGLE(val, Y, C, STEP); WRITE_SINGLE(val, Z, B, STEP); }

  // Define step direction output pins.
  #define INTERNAL_X_DIRECTION  (1<<5)
  #define INTERNAL_Y_DIRECTION  (1<<7)
  #define INTERNAL_Z_DIRECTION  (1<<2)
  #define X_DIRECTION_BIT       4  // Dummy unique value
  #define Y_DIRECTION_BIT       5  // Dummy unique value
  #define Z_DIRECTION_BIT       6  // Dummy unique value

  #define SET_DIRECTION_DDR() { SET_SINGLE_DDR(X, C, DIRECTION); SET_SINGLE_DDR(Y, C, DIRECTION); SET_SINGLE_DDR(Z, B, DIRECTION); }
  #define WRITE_DIRECTION_PORT(val) { WRITE_SINGLE(val, X, C, DIRECTION); WRITE_SINGLE(val, Y, C, DIRECTION); WRITE_SINGLE(val, Z, B, DIRECTION); }

  // Define stepper driver enable/disable output pin.
  #define INTERNAL_XY_ENABLE  (1<<6)
  #define INTERNAL_Z_ENABLE   (1<<5)

  #define SET_STEPPERS_DISABLE_DDR() { SET_ON(DDRD, INTERNAL_XY_ENABLE); SET_ON(DDRA, INTERNAL_Z_ENABLE); }
  #define WRITE_STEPPERS_DISABLE_PORT_ON() { SET_ON(PORTD, INTERNAL_XY_ENABLE); SET_ON(PORTA, INTERNAL_Z_ENABLE); }
  #define WRITE_STEPPERS_DISABLE_PORT_OFF() { SET_OFF(PORTD, INTERNAL_XY_ENABLE); SET_OFF(PORTA, INTERNAL_Z_ENABLE); }

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR        DDRB
  #define LIMIT_PIN        PINB
  #define LIMIT_PORT       PORTB
  #define X_LIMIT_BIT      1  // Uno Digital Pin 9
  #define Y_LIMIT_BIT      2  // Uno Digital Pin 10
  #ifdef VARIABLE_SPINDLE // Z Limit pin and spindle enabled swapped to access hardware PWM on Pin 11.
    #define Z_LIMIT_BIT	   4 // Uno Digital Pin 12
  #else
    #define Z_LIMIT_BIT    3  // Uno Digital Pin 11
  #endif
  #if !defined(ENABLE_DUAL_AXIS)
    #define LIMIT_MASK     ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
  #endif
  #define LIMIT_INT        PCIE0  // Pin change interrupt enable pin
  #define LIMIT_INT_vect   PCINT0_vect
  #define LIMIT_PCMSK      PCMSK0 // Pin change interrupt register

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       DDRC
  #define CONTROL_PIN       PINC
  #define CONTROL_PORT      PORTC
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     PCMSK1 // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       DDRC
  #define PROBE_PIN       PINC
  #define PROBE_PORT      PORTC
  #define PROBE_BIT       5  // Uno Analog Pin 5
  #define PROBE_MASK      (1<<PROBE_BIT)

  #if !defined(ENABLE_DUAL_AXIS)

    // Define flood and mist coolant enable output pins.
    #define COOLANT_FLOOD_DDR   DDRC
    #define COOLANT_FLOOD_PORT  PORTC
    #define COOLANT_FLOOD_BIT   3  // Uno Analog Pin 3
    #define COOLANT_MIST_DDR   DDRC
    #define COOLANT_MIST_PORT  PORTC
    #define COOLANT_MIST_BIT   4  // Uno Analog Pin 4

    // Define spindle enable and spindle direction output pins.
    #define SPINDLE_ENABLE_DDR    DDRB
    #define SPINDLE_ENABLE_PORT   PORTB
    // Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
    #ifdef VARIABLE_SPINDLE
      #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
        // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
        #define SPINDLE_ENABLE_BIT    5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
      #else
        #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
      #endif
    #else
      #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
    #endif
    #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
      #define SPINDLE_DIRECTION_DDR   DDRB
      #define SPINDLE_DIRECTION_PORT  PORTB
      #define SPINDLE_DIRECTION_BIT   5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
    #endif

    // Variable spindle configuration below. Do not change unless you know what you are doing.
    // NOTE: Only used when variable spindle is enabled.
    #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
    #ifndef SPINDLE_PWM_MIN_VALUE
      #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
    #endif
    #define SPINDLE_PWM_OFF_VALUE     0
    #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
    #define SPINDLE_TCCRA_REGISTER    TCCR2A
    #define SPINDLE_TCCRB_REGISTER    TCCR2B
    #define SPINDLE_OCR_REGISTER      OCR2A
    #define SPINDLE_COMB_BIT          COM2A1

    // Prescaled, 8-bit Fast PWM mode.
    #define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
    // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
    // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
    // #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
    #define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

    // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
    #define SPINDLE_PWM_DDR   DDRB
    #define SPINDLE_PWM_PORT  PORTB
    #define SPINDLE_PWM_BIT   3    // Uno Digital Pin 11
  
  #else

    // Dual axis feature requires an independent step pulse pin to operate. The independent direction pin is not 
    // absolutely necessary but facilitates easy direction inverting with a Grbl $$ setting. These pins replace 
    // the spindle direction and optional coolant mist pins.

    #ifdef DUAL_AXIS_CONFIG_PROTONEER_V3_51
      // NOTE: Step pulse and direction pins may be on any port and output pin.
      #define INTERNAL_DUAL_STEP  (1<<4)
      #define DUAL_STEP_BIT      1  // Dummy unique value
      #define STEP_MASK_DUAL      ((1<<DUAL_STEP_BIT))

      #define SET_STEP_DDR_DUAL() { SET_SINGLE_DDR(DUAL, C, STEP); }
      #define WRITE_STEP_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, C, STEP); }

      #define INTERNAL_DUAL_DIRECTION (1<<3)
      #define DUAL_DIRECTION_BIT   2  // Dummy unique value

      #define SET_DIRECTION_DDR_DUAL() { SET_SINGLE_DDR(DUAL, D, DIRECTION); }
      #define WRITE_DIRECTION_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, D, DIRECTION); }

      // NOTE: Dual axis limit is shared with the z-axis limit pin by default. Pin used must be on the same port
      // as other limit pins.
      #define DUAL_LIMIT_BIT    Z_LIMIT_BIT
      #define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<DUAL_LIMIT_BIT))

      // Define coolant enable output pins.
      // NOTE: Coolant flood moved from A3 to A4. Coolant mist not supported with dual axis feature on Arduino Uno.
      #define COOLANT_FLOOD_DDR   DDRB
      #define COOLANT_FLOOD_PORT  PORTB
      #define COOLANT_FLOOD_BIT   5  // Uno Digital Pin 13

      // Define spindle enable output pin.
      // NOTE: Spindle enable moved from D12 to A3 (old coolant flood enable pin). Spindle direction pin is removed.
      #define SPINDLE_ENABLE_DDR    DDRB
      #define SPINDLE_ENABLE_PORT   PORTB
      #ifdef VARIABLE_SPINDLE
        // NOTE: USE_SPINDLE_DIR_AS_ENABLE_PIN not supported with dual axis feature.
        #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
      #else
        #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
      #endif

      // Variable spindle configuration below. Do not change unless you know what you are doing.
      // NOTE: Only used when variable spindle is enabled.
      #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
      #ifndef SPINDLE_PWM_MIN_VALUE
        #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
      #endif
      #define SPINDLE_PWM_OFF_VALUE     0
      #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
      #define SPINDLE_TCCRA_REGISTER    TCCR2A
      #define SPINDLE_TCCRB_REGISTER    TCCR2B
      #define SPINDLE_OCR_REGISTER      OCR2A
      #define SPINDLE_COMB_BIT          COM2A1

      // Prescaled, 8-bit Fast PWM mode.
      #define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
      // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
      // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
      // #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
      #define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

      // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
      #define SPINDLE_PWM_DDR   DDRB
      #define SPINDLE_PWM_PORT  PORTB
      #define SPINDLE_PWM_BIT   3    // Uno Digital Pin 11
    #endif

    // NOTE: Variable spindle not supported with this shield.
    #ifdef DUAL_AXIS_CONFIG_CNC_SHIELD_CLONE
      // NOTE: Step pulse and direction pins may be on any port and output pin.
      #define INTERNAL_DUAL_STEP  (1<<4)
      #define DUAL_STEP_BIT      1  // Dummy unique value
      #define STEP_MASK_DUAL      ((1<<DUAL_STEP_BIT))

      #define SET_STEP_DDR_DUAL() { SET_SINGLE_DDR(DUAL, B, STEP); }
      #define WRITE_STEP_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, B, STEP); }

      #define INTERNAL_DUAL_DIRECTION (1<<5)
      #define DUAL_DIRECTION_BIT   2  // Dummy unique value

      #define SET_DIRECTION_DDR_DUAL() { SET_SINGLE_DDR(DUAL, B, DIRECTION); }
      #define WRITE_DIRECTION_PORT_DUAL(val) { WRITE_SINGLE(val, DUAL, B, DIRECTION); }

      // NOTE: Dual axis limit is shared with the z-axis limit pin by default.
      #define DUAL_LIMIT_BIT    Z_LIMIT_BIT
      #define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<DUAL_LIMIT_BIT))

      // Define coolant enable output pins.
      // NOTE: Coolant flood moved from A3 to A4. Coolant mist not supported with dual axis feature on Arduino Uno.
      #define COOLANT_FLOOD_DDR   DDRC
      #define COOLANT_FLOOD_PORT  PORTC
      #define COOLANT_FLOOD_BIT   4  // Uno Analog Pin 4

      // Define spindle enable output pin.
      // NOTE: Spindle enable moved from D12 to A3 (old coolant flood enable pin). Spindle direction pin is removed.
      #define SPINDLE_ENABLE_DDR    DDRC
      #define SPINDLE_ENABLE_PORT   PORTC
      #define SPINDLE_ENABLE_BIT    3  // Uno Analog Pin 3
    #endif

  #endif

#endif


#endif
