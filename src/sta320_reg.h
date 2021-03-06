#ifndef __STA230_REG_H
#define __STA320_REG_H

#define REG_CONFA		        0x00    // FDRB, TWAB, TWRB, IR, MCS
    #define CONFA_MCS_MASK      0x07    // Master Clock Select
    #define CONFA_IR_MASK       0x18    // Interpolate Ratio
        #define IR_2OVER        0x00    // 00
        #define IR_PASS         0x08    // 01
        #define IR_2DOWN        0x10    // 10
    #define CONFA_TWRB          0x20    // Thermal Warning Recovery Bypass
    #define CONFA_TWAB          0x40    // Thermal Warning Adjust Bypass
    #define CONFA_FDRB          0x80    // Fault-Detector Recovery Bypass
#define REG_CONFA_CONFIG        0x00
#define REG_CONFA_INIT	        REG_CONFA_CONFIG

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
#define REG_CONFD_CONFIG	    (CONFD_BQL | CONFD_PSL | CONFD_ZDE)
#define REG_CONFD_INIT          (REG_CONFD_CONFIG)

#define REG_CONFE		        0x04
    #define CONFE_MPCV          0x01    // Max Power Correction Variable
    #define CONFE_MPC           0x02    // Max Power Correction enabled
    #define CONFE_NSBW          0x04    // Noise Shaper Bandwidth Selection (DEPRECATED)
    #define CONFE_AME           0x08    // AM Mode enable
    #define CONFE_PWMS          0x10    // PWM Speed 1 = ODD
    #define CONFE_DCCV          0x20    // Distortion Compensation Variable Enable (DEPRECATED)
    #define CONFE_ZCE           0x40    // Zero-Crossing Volume Enable
    #define CONFE_SVE           0x80    // Soft Volume Enable
#define REG_CONFE_INIT	    (CONFE_MPC | CONFE_ZCE | CONFE_SVE)

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
#define REG_MUTE_INIT	        (REG_MUTE_CONFIG | MUTE_MASTER)

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
    #define AUTO1_AMPS          0x80    // AutoMode auto prescale
        #define AMPS_18DB       0x00
        #define AMPS_USER       0x80   
#define REG_AUTO1_INIT      	(AMEQ_PRESET | AMV_STD | AMGC_NOCLIP | AMPS_18DB)

#define REG_AUTO2		        0x0C
    #define AUTO2_AMAME         0x01    // AutoMode AM switching enable
    #define AUTO2_AMAM_MASK     0x0E    // AutoMode AM switching Frequency Selection
    #define AUTO2_XO_MASK       0xF0    // AutoMode Crossover Frequency selection
        #define XO_USER         0x00
        #define XO_80HZ         0x10    
        #define XO_100HZ        0x20
        #define XO_120HZ        0x30
        #define XO_140HZ        0x40
		// ... add 1 for every 20 Hz up to 360 Hz
		#define XO_360HZ		0xF0
#define REG_AUTO2_INIT	        (XO_USER)

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

#define REG_LIM1_RATE			0x12
#define REG_LIM1_ATTREL			0x13
#define REG_LIM2_RATE			0x14
#define REG_LIM2_ATTREL			0x15
	#define LIMx_ATTACK_MASK	0xF0
	#define LIMx_RELEASE_MASK	0x0F

#define REG_CFADDR				0x16
// Registers B1, B2, A1, A2 B0, each 24 bits = 15 registers
#define REG_CFUD				0x26
	#define CFUD_W1				0x01	// Write 1
	#define CFUD_WA				0x02	// Write all
	#define CFUD_R1				0x04	// Read 1
	#define CFUD_RA				0x08	// Read all

#define REG_STATUS              0x2D
    #define STATUS_TWARN        0x01
    #define STATUS_FAULT        0x02
    #define STATUS_TFAULT       0x04
    #define STATUS_OCWARN       0x08
    #define STATUS_PLLUL        0x80


#endif // __STA320_REG_H