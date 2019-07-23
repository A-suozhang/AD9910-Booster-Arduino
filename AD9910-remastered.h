
# define uchar unsigned char
# define CLOCKSPEED 1000000
#include <SPI.h>


class AD9910
{
    public:
    // ----public entitys----;
    int _cs ,_rst ,_update ,_sdio ,_sclk;
    SPIClass * _hspi;
    uint8_t cfr1[4] = {0x00, 0x40, 0x00, 0x02};
    uint8_t cfr2[4] = {0x01, 0x00, 0x08, 0x20};
    uint8_t cfr3[4] = {0x05, 0x08, 0x41, 0x32};
    uint8_t DAC_config[4] = {0x00,0x00,0x00,0xFF};
    uint8_t profile0[8] = {0x3F, 0x3F, 0x00, 0x00, 0x25, 0x09, 0x7b, 0x42};
    // ----Constructor----
    AD9910(int cs , int rst , int update , int sdio , int sclk, SPIClass* hspi)    
    // 如果这里传入SPIClass hspi是SPIClass*
    // 那么就传了一个空指针
    // 传一个指针的指针(指针的地址进去)
    // 然后调用的时候(*hspi)->begin()
    {
        _cs  = cs ;
        _rst  = rst ;
        _update  = update ;
        _sdio  = sdio ;
        _sclk  = sclk ;
        _hspi = hspi;
        
        // _pwr  = pwr ;    LOW
        // _drctl = drctl;  LOW
        // _drhold = drhold;    LOW
        // _drover = _drover;   LOW
        // _ps0 = ps0;  LOW
        // _ps1 = ps1;  LOW
        // _ps2 = ps2;  LOW
        // _osk  = osk ;    LOW

    }

    // -------------Init IO For AD9910 =============
    void begin()
    {
        // Set IO
        pinMode(_cs , OUTPUT);
        pinMode(_rst , OUTPUT);
        pinMode(_update , OUTPUT);
        pinMode(_sdio, OUTPUT);
        pinMode(_sclk, OUTPUT);

        // Set HIGH/LOW
        digitalWrite(_cs , HIGH);
        digitalWrite(_rst , LOW);
        digitalWrite(_update , LOW);

        reset();
        initialize();
    }

    void reset()
    {
        digitalWrite(_rst , HIGH);
        // delay(1);
        digitalWrite(_rst , LOW);
    }

    void update()
    {
        digitalWrite(_update , HIGH);
        // delay(1);  在Loop中会调用,实际测试发现不用delay
        digitalWrite(_update ,LOW);
    }

    void SPI_Write_Reg(uint8_t addr, uint8_t bytes[], uint8_t num_bytes)
    {

        (_hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); // 开始SPI传输,其中CLOCKSPEED是SCLK传输,选用MODE0(经测试除了MODE1之外其他MODE都能操作)
        digitalWrite(_cs , LOW);    // 手动拉低CS
        (_hspi)->transfer(addr);
        // delay(1);
        for (int i = 0; i < num_bytes; i++){
            (_hspi)->transfer(bytes[i]);
            // (_hspi)->transfer(0x0A);         For Test ONLY!
        }
        digitalWrite(_cs , HIGH);
        (_hspi)->endTransaction();
    }
    void initialize()
    {
        // Write Control Word Into Reg
        SPI_Write_Reg(0x00,cfr1,4);
        delay(1);
        SPI_Write_Reg(0x01,cfr2,4);
        delay(1);
        SPI_Write_Reg(0x02,cfr3,4);
        delay(1);
        SPI_Write_Reg(0x03,DAC_config,4);   // 据说需要初始化一下DAC,不然输出信号的幅值到不了完全
        delay(1);
        update();
        delay(1);
    }

    void set_freq(double freq, uint8_t profile = 0)
    {
        if (profile > 7){
            return;
        }
        unsigned long temp;
        if (freq > 400000000){
            freq = 400000000;
        }
        temp = freq*4.294967296;    // 2^32/10^9(SysCLK)  

        profile0[7] = (uchar)temp;
        profile0[6] = (uchar)(temp >> 8);
        profile0[5] = (uchar)(temp >> 16);
        profile0[4] = (uchar)(temp >> 24);
        //
        SPI_Write_Reg(0x0E + profile, profile0, 8);
        update();
    }

    void set_Amp(double amp, uint8_t profile = 0)
    {
        unsigned long temp;
        temp = (unsigned long)amp*25.20615385;  // 25.20615385=(2^14)/650
        if (temp > 0x3FFF){
            temp = 0x3FFF;
        }
        temp &= 0x3FFF;
        profile0[0] = (unsigned char)(temp >> 8);
        profile0[1] = (unsigned char)(temp);

        SPI_Write_Reg(0x0E + profile,profile0,8);
        update();
    }

};

