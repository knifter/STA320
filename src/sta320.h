#ifndef STA320_H
#define STA320_H

#include <Arduino.h>
#include <TwoWireDevice.h>

#define STA320_ADDRESS_DEFAULT              (0x1A)

class STA320: public TwoWireDevice
{
public:
	typedef enum {
		RATE_48K256 =  /* IR= */ 0x00 | /* MCS = */ 0x03,
		RATE_96K256 =  /* IR= */ 0x08 | /* MCS = */ 0x01,
		RATE_192K256 = /* IR= */ 0x10 | /* MCS = */ 0x01
	} samplerate_t;
    void setSampleRate(samplerate_t);

    typedef enum {
        PRESET_FLAT = 0,
        PRESET_ROCK,    PRESET_SOFTROCK,
        PRESET_JAZZ,    PRESET_CLASSICAL,
        PRESET_DANCE,   PRESET_POP,
        PRESET_SOFT,    PRESET_HARD,
        PRESET_PARTY,   PRESET_VOCAL,
        PRESET_HIPHOP,  PRESET_DIALOG,
        PRESET_BASSBOOST1, PRESET_BASSBOOST2, PRESET_BASSBOOST3,
        PRESET_LOUDNESS1 = 0x10,
        PRESET_LOUDNESS2
        // etc... until LOUDNESS16 = 11111
    } eqpreset_t;
    void setPEQ(eqpreset_t);

    STA320(TwoWire& wire, const uint8_t addr = STA320_ADDRESS_DEFAULT) : TwoWireDevice(wire, addr) {};
    STA320(const uint8_t addr = STA320_ADDRESS_DEFAULT) : TwoWireDevice(addr) {};

    bool begin();
    void printStatus();

    //actions
    void powerUp();
    void powerDown();
    void mute(bool mute = true);
    void unmute() { mute(false); };
    bool isMuted() { return _muted; };
    void setVolume(uint8_t vol);
    int8_t setTreble(int8_t db);
    int8_t setBass(int8_t db);

	uint8_t volume;
	uint8_t _reg_tone;

private:
    STA320(const STA320&) = delete;
    STA320& operator=(STA320 const&) = delete;
    
	// uint8_t _tone = 0x77;
    bool _muted = true;
};

#endif // STA320_H
