# fineoffset


problems...

problem 1) code is shit, it's OK

problem 2)?? temperature and deviceid is actually only 12 bits of data, storing it in uint16_t - problem?

problem 3) have to calculate crc8 of payload (which is deviceid, temperature, humidity) (32 bits/4 bytes) - now stored in uint32_t payload.. wise?

(The CRC-8 polynomial used is x8 + x5 + x4 + 1.)
