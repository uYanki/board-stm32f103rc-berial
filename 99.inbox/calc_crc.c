
uint8_t calc_crc(uint8_t dat[], uint8_t cnt);

uint8_t calc_crc(uint8_t dat[], uint8_t cnt)
{
    uint8_t crc = 0;
    uint8_t idx, bit;

    // p(x)=x^8+x^5+x^4+1 = 0b100110001 = 305 = 0x131

    for (idx = 0; idx < cnt; ++idx) {
        crc ^= dat[idx];
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc <<= 1;
                crc ^= 0x131;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}
