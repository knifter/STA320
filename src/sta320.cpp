#include "sta320.h"

#include <Arduino.h>
#include <TwoWireDevice.h>

#include "sta320_reg.h"

// #ifndef DBG
// 	#define DBG(x)          Serial.print("STA: "); Serial.print(x);
// 	#define DBGLN(x)        Serial.print("STA: "); Serial.println(x);
#define PRINT(x)        Serial.print(x);
// #endif

bool STA320::begin()
{
    TwoWireDevice::begin();

	_reg_tone = REG_TONE_INIT;
    _reg_confd = REG_CONFD_INIT;

	writereg8(REG_CONFA, REG_CONFA_INIT);
	writereg8(REG_CONFB, REG_CONFB_INIT);
	writereg8(REG_CONFC, REG_CONFC_INIT);
	writereg8(REG_CONFD, REG_CONFD_INIT);
	writereg8(REG_CONFE, REG_CONFE_INIT);
	writereg8(REG_CONFF, REG_CONFF_INIT);
    writereg8(REG_MUTE,  REG_MUTE_INIT); // start muted

    writereg8(REG_MVOL, REG_MVOL_INIT);
    writereg8(REG_C1VOL, REG_C1VOL_INIT);
    writereg8(REG_C2VOL, REG_C2VOL_INIT);
    writereg8(REG_C3VOL, REG_C3VOL_INIT);
    writereg8(REG_AUTO1, REG_AUTO1_INIT);
    writereg8(REG_AUTO2, REG_AUTO2_INIT);
    writereg8(REG_PRESET, REG_PRESET_INIT);
    writereg8(REG_C1CFG, REG_C1CFG_INIT);
    writereg8(REG_C2CFG, REG_C2CFG_INIT);
    writereg8(REG_C3CFG, REG_C3CFG_INIT);
	writereg8(REG_TONE, REG_TONE_INIT);

    return true;
}

void STA320::setMiamiMode(bool on)
{
    if(on)
        _reg_confd |= CONFD_MME;
    else
        _reg_confd &= ~CONFD_MME;
     
    writereg8(REG_CONFD, _reg_confd);
}

void STA320::setDSPByPass(bool on)
{
    if(on)
        _reg_confd |= CONFD_DSPB;
    else
        _reg_confd &= ~CONFD_DSPB;
     
    writereg8(REG_CONFD, _reg_confd);
}

void STA320::setDeEmph(bool on)
{
    if(on)
        _reg_confd |= CONFD_DEMP;
    else
        _reg_confd &= ~CONFD_DEMP;
     
    writereg8(REG_CONFD, _reg_confd);
}

void STA320::powerDown()
{
    writereg8(REG_CONFF, REG_CONFF_INIT);
}

void STA320::powerUp()
{
    writereg8(REG_CONFF, REG_CONFF_INIT | CONFF_EAPD | CONFF_PWDN);
}

void STA320::setPEQ(eqpreset_t eq)
{
	writereg8(REG_PRESET, eq & REG_PRESET_MASK);
}

int8_t STA320::setTreble(int8_t db)
{
	// limit to -12..12 db
	if(db < -12)	db = -12;
	if(db > +12) 	db = +12;
	_reg_tone &= 0x0F; // keep bass
	_reg_tone |= ((0x07 + (db >> 1)) << 4);
	// DBG("tone = %x", _reg_tone);
	writereg8(REG_TONE, _reg_tone);
	return db;// & 0xFE; // drop the last bit
}

int8_t STA320::setBass(int8_t db)
{
	// limit to -12..12 db
	if(db < -12)	db = -12;
	if(db > +12) 	db = +12;
	_reg_tone &= 0xF0; // keep treble
	_reg_tone |= (0x07 + (db >> 1));
	// DBG("tone = %x", _reg_tone);
	writereg8(REG_TONE, _reg_tone);
	return db; // drop the last bit
}

void STA320::setSampleRate(samplerate_t rate)
{
	// DBG("setSampleRate: 0x");
	// DBG(rate, HEX);
    
	writereg8(REG_CONFA, REG_CONFA_CONFIG | rate);
}

void STA320::mute(bool mute)
{
#ifdef STA320_DEBUG
	if(mute)
    {
		DBGLN("MUTE");
    }else{
	    DBGLN("UNMUTE");
    };
#endif
    writereg8(REG_MUTE, REG_MUTE_CONFIG | (mute ? MUTE_MASTER : 0x00));
    _muted = mute;
}

int8_t STA320::setVolume(int8_t attn)
{
	if(attn > 127)	attn = 127;
	if(attn < 0)	attn = 0;
	writereg8(REG_MVOL, attn << 1); // 0.5 dB/bit
	return attn;
}

void STA320::printStatus()
{
    uint8_t st = readreg8(REG_STATUS);
    PRINT("\nREG_STATUS: ");
    if(st == 0x0F)
        PRINT("OKAY ");
    if((st & STATUS_PLLUL))
        PRINT("PLLUL ");
    if(!(st & STATUS_OCWARN))
        PRINT("OCWARN ");
    if(!(st & STATUS_TFAULT))
        PRINT("TFAULT ");
    if(!(st & STATUS_FAULT))
        PRINT("FAULT ");
    if(!(st & STATUS_TWARN))
        PRINT("TWARN ");
    PRINT("\n");
}
