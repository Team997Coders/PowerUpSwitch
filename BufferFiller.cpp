// This code slightly follows the conventions of, but is not derived from:
//      EHTERSHIELD_H library for Arduino etherShield
//      Copyright (c) 2008 Xing Yu.  All right reserved. (this is LGPL v2.1)
// It is however derived from the enc28j60 and ip code (which is GPL v2)
//      Author: Pascal Stang
//      Modified by: Guido Socher
//      DHCP code: Andrew Lindsay
// Hence: GPL V2
//
// 2010-05-19 <jc@wippler.nl>

#include "BufferFiller.h"

static char* wtoa (uint16_t value, char* ptr) {
    if (value > 9)
        ptr = wtoa(value / 10, ptr);
    *ptr = '0' + value % 10;
    *++ptr = 0;
    return ptr;
}

void BufferFiller::emit_p(const char* fmt PROGMEM, ...) {
    va_list ap;
    va_start(ap, fmt);
    for (;;) {
        char c = pgm_read_byte(fmt++);
        if (c == 0)
            break;
        if (c != '$') {
            *ptr++ = c;
            continue;
        }
        c = pgm_read_byte(fmt++);
        switch (c) {
        case 'D':
#ifdef __AVR__
            wtoa(va_arg(ap, uint16_t), (char*) ptr);
#else
            wtoa(va_arg(ap, int), (char*) ptr);
#endif
            break;
#ifdef FLOATEMIT
        case 'T':
            dtostrf    (    va_arg(ap, double), 10, 3, (char*)ptr );
            break;
#endif
        case 'H': {
#ifdef __AVR__
            char p1 =  va_arg(ap, uint16_t);
#else
            char p1 =  va_arg(ap, int);
#endif
            char p2;
            p2 = (p1 >> 4) & 0x0F;
            p1 = p1 & 0x0F;
            if (p1 > 9) p1 += 0x07; // adjust 0x0a-0x0f to come out 'a'-'f'
            p1 += 0x30;             // and complete
            if (p2 > 9) p2 += 0x07; // adjust 0x0a-0x0f to come out 'a'-'f'
            p2 += 0x30;             // and complete
            *ptr++ = p2;
            *ptr++ = p1;
            continue;
        }
        case 'L':
            ltoa(va_arg(ap, long), (char*) ptr, 10);
            break;
        case 'S':
            strcpy((char*) ptr, va_arg(ap, const char*));
            break;
        case 'F': {
            const char* s PROGMEM = va_arg(ap, const char*);
            char d;
            while ((d = pgm_read_byte(s++)) != 0)
                *ptr++ = d;
            continue;
        }
        case 'E': {
            byte* s = va_arg(ap, byte*);
            char d;
            while ((d = eeprom_read_byte(s++)) != 0)
                *ptr++ = d;
            continue;
        }
        default:
            *ptr++ = c;
            continue;
        }
        ptr += strlen((char*) ptr);
    }
    va_end(ap);
}
