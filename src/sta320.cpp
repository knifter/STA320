#include "sta320.h"

#include <Arduino.h>
#include <TwoWireDevice.h>

#define REG_CONFA		        0x00    // FDRB, TWAB, TWRB, IR, MCS
    #define CONFA_MCS_MASK      0x07    // Master Clock Select
    #define CONFA_IR_MASK       0x18    // Interpolate Ratio
        #define IR_2OVER        0x00    // 00
        #define IR_PASS         0x08    // 01
        #define IR_2DOWN        0x10    // 10
    #define CONFA_TWRB          0x20    // Thermal Warning Recovery Bypass
    #define CONFA_TWAB          0x40    // Thermal Warning Adjust Bypass
    #define CONFA_FDRB          0x80    // Fault-Detector Recovery Bypass
#define REG_CONFA_CONFIG    0x00
#define REG_CONFA_INIT	    REG_CONFA_CONFIG

// ???
#define REG_CONFB		        0x01    // C2IM, C1IM, DSCKE, SAIFB, SAI
    #define CONFB_SAI_MASK      0x0F    //  Serial Interface Format = 0000 = 24 bit data ???
        #define   SAI_64I2S24B  0x00
        #define   SAI_64I2S20B  0x04
        #define   SAI_64I2S18B  0x08
    #define CONFB_SAIFB         0x10    // Serial Data First Bit: MSB, LSB = 0
    #define CONFB_DSCKE         0x20    // Delay Serial Clock Enable = 0
    #define CONFB_C1IM          0x40    // Channel 1 Input Mapping = 0 = Left
    #define CONFB_C2IM          0x80    // Channel 2 Input Mapping = 1 = Right
#define REG_CONFB_INIT	    CONFB_C2IM | SAI_64I2S24B

// ???
#define REG_CONFC		        0x02
    #define CONFC_OM_DROPCOMP   0x00    // Output Mode: Drop Compensation
    #define CONFC_OM_DISCRETE   0x01    //              Discrete Output Stage
    #define CONFC_OM_FULLPOWER  0x02    //              Full Power Mode     ???
    #define CONFC_OM_VARIABLE   0x03    //              Variable Drop Compensation (CSZx bits)
    #define CONFC_CSZ_MASK      0x7C    // Compensation Pulse Size
    #define CONFC_OCRB          0x80    // Over-Current Warning Detect Adjustment Bypass
#define REG_CONFC_INIT	    (CONFC_OCRB | CONFC_OM_FULLPOWER)

#define REG_CONFD		        0x03
    #define CONFD_HPB           0x01    // High-Pass Filter Bypass (DC Block!) = 0
    #define CONFD_DEMP          0x02    // De-Emphasis Enabled
    #define CONFD_DSPB          0x04    // DSP Bypass
    #define CONFD_PSL           0x08    // Post-Scale Link (all Channel 1)
    #define CONFD_BQL           0x10    // Biquad Coeffic. Link (all Channel 1)
    #define CONFD_DRC           0x20    // Dynamic Range Compression/Anti-Clippin select (0 = Anti Clip, 1=Compression)
    #define CONFD_ZDE           0x40    // Zero-Detect Mute Enable
    #define CONFD_MME           0x80    // Miami Mode
#define REG_CONFD_INIT	    (CONFD_ZDE | CONFD_BQL | CONFD_PSL | CONFD_ZDE)

#define REG_CONFE		        0x04
    #define CONFE_MPCV          0x01    // Max Power Correction Variable
    #define CONFE_MPC           0x02    // Max Power Correction enabled
    #define CONFE_NSBW          0x04    // Noise Shaper Bandwidth Selection (DEPRECATED)
    #define CONFE_AME           0x08    // AM Mode enable
    #define CONFE_PWMS          0x10    // PWM Speed 1 = ODD
    #define CONFE_DCCV          0x20    // Distortion Compensation Variable Enable (DEPRECATED)
    #define CONFE_ZCE           0x40    // Zero-Crossing Volume Enable
    #define CONFE_SVE           0x80    // Soft Volume Enable
#define REG_CONFE_INIT	    (CONFE_ZCE | CONFE_SVE | CONFE_MPC)

#define REG_CONFF		        0x05
    #define CONFF_OCFG_MASK     0x03    // Output Configuration Engine Selection = 00
    #define CONFF_OCFG          0x00
    #define CONFF_IDE           0x04    // Invalid Input Detect Mute
    #define CONFF_BCLE          0x08    // Binary Output Mode Clock Loss Detection Enable
    #define CONFF_LDTE          0x10    // LRCLK Double Trigger Protection Enable
    #define CONFF_ECLE          0x20    // Auto EAPD on Clock Loss
    #define CONFF_PWDN          0x40    // IC Power Down disable, 0 = Power down, 1 = Normal operation
    #define CONFF_EAPD          0x80    // External Amplifier Power Down disable, 0 = Power down..
#define REG_CONFF_INIT        (CONFF_ECLE | CONFF_IDE | CONFF_OCFG)

#define REG_MUTE 		        0x06
    #define MUTE_MASTER         0x01    // Master Mute
    #define MUTE_C1M            0x02    // Channel 1 Mute
    #define MUTE_C2M            0x04    // Channel 2 Mute
    #define MUTE_C3M            0x08    // Channel 3 Mute
    #define MUTE_TFRB           0x10    // Thermal Fault Mute disable (DEPRECATED)
    #define MUTE_QXEN           0x40    // Qxpander Enable (DEPRECATED)
    #define MUTE_QFILTER        0x80    // Qfilter disable (i.s.o. Simple LPF) (DEPRECATED)
#define REG_MUTE_CONFIG         MUTE_C3M
#define REG_MUTE_INIT	     MUTE_MASTER

#define REG_MVOL 		        0x07
#define REG_C1VOL               0x08
#define REG_C2VOL               0x09
#define REG_C3VOL               0x0A
#define REG_MVOL_INIT           0x7F    // -127 dB
#define REG_C1VOL_INIT          0x14    // 0 = +48 dB, 0.5 dB Steps
#define REG_C2VOL_INIT          0x14
#define REG_C3VOL_INIT          0x14

#define REG_AUTO1		        0x0B
    #define AUTO1_AMEQ_MASK     0x03    // AutoMode EQ
        #define AMEQ_USER       0x00    // User Programmable (Channel pre-scale enabled, disabled otherwise)
        #define AMEQ_PRESET     0x01    // Preset EQ - PEQ bits
        #define AMEQ_AUTO       0x02    // Auto controlled loudness curve
        #define AMEQ_NOTUSED    0x03    // Not Used
    #define AUTO1_AMV_MASK      0x0C    // AutoMode Volume
        #define AMV_STD         (0x00 << 2)
        #define AMV_30          (0x01 << 2)
        #define AMV_40          (0x02 << 2)
        #define AMV_50          (0x03 << 2)
    #define AUTO1_AMGC_MASK     0x30    // AutoMode gain compression/limiters
        #define AMGC_USER       0x00    // User Programmable Curve
        #define AMGC_NOCLIP     0x10    // AC no clipping
        #define AMGC_CLIP       0x20    // AC limited clipping (10%)
        #define AMGC_DRC        0x30    // DRC Nighttime listening mode
    #define AUTO1_AMPS          0x80    // AutoMove auto prescale
        #define AMPS_18DB       0x00
        #define AMPS_USER       0x80   
#define REG_AUTO1_INIT      (AMEQ_PRESET | AMV_STD | AMGC_NOCLIP | AMPS_18DB)

#define REG_AUTO2		        0x0C
    #define AUTO2_AMAME         0x01    // AutoMode AM switching enable
    #define AUTO2_AMAM_MASK     0x0E    // AutoMode AM switching Frequency Selection
    #define AUTO2_XO_MASK       0xF0    // AutoMode Crossover Frequency selection
        #define XO_USER         0x00
        #define XO_80HZ         0x10    // Use this as reference, add 1 for every 20 Hz up to 360 Hz
        #define XO_100HZ        0x20
        #define XO_120HZ        0x30
        #define XO_140HZ        0x40
#define REG_AUTO2_INIT	        0x00

#define REG_PRESET              0x0D
#define REG_PRESET_MASK         0x1F
#define REG_PRESET_INIT         (PRESET_FLAT)

#define REG_C1CFG		        0x0E
#define REG_C2CFG		        0x0F
#define REG_C3CFG		        0x10
    #define CxCFG_TCB           0x01    // Tone Control Bypass (ch 1&2 only)
    #define CxCFG_EQBP          0x02    // EQ Bypass (ch 1&2 only)
    #define CxCFG_VBP           0x04    // Volume Bypass
    #define CxCFG_BO            0x08    // Binary Output Enable (iso DDX) = 0
    #define CxCFG_LS_MASK       0x30    // Channel Limiter mapping
        #define LS_LIMITERDISABLE   0x00    // Limiter disabled
        #define LC_LIMITER1         0x10    // Channel x is mapped to limiter 1
        #define LC_LIMITER2         0x20    // Channel x is mapped to limiter 1
    #define CxCFG_M_MASK        0xC0    // Channel PWM Output mapping
        #define PWMMAP_CH1      0x00
        #define PWMMAP_CH2      0x40
        #define PWMMAP_CH3      0x80
        #define PWMMAP_NONE     0xC0
#define REG_C1CFG_INIT	        (LC_LIMITER1 | PWMMAP_CH2) // 0x50
#define REG_C2CFG_INIT	        (LC_LIMITER1 | PWMMAP_CH1) // 0x10 
#define REG_C3CFG_INIT	        (LC_LIMITER2 | PWMMAP_CH3) // 0xA0

#define REG_TONE		        0x11
    #define TONE_BTC_MASK       0x0F    // Bass
    #define TONE_TTC_MASK       0xF0    // Treble
#define REG_TONE_INIT			0x33	// 0 db, 0 db
#define REG_STATUS              0x2D
    #define STATUS_TWARN        0x01
    #define STATUS_FAULT        0x02
    #define STATUS_TFAULT       0x04
    #define STATUS_OCWARN       0x08
    #define STATUS_PLLUL        0x80

// #ifndef DBG
// 	#define DBG(x)          Serial.print("STA: "); Serial.print(x);
// 	#define DBGLN(x)        Serial.print("STA: "); Serial.println(x);
#define PRINT(x)        Serial.print(x);
// #endif

bool STA320::begin()
{
    TwoWireDevice::begin();

	_reg_tone = REG_TONE_INIT;

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
