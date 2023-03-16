#ifndef LORA_H_
#define LORA_H_
#ifdef __cplusplus
extern "C" {
#endif

#define LoRa_circ_bbuf_LEN 16
#define LoRa_RadioBuffer_LEN 52
typedef struct
{
    uint8_t  RadioBuffer[LoRa_RadioBuffer_LEN];
    uint8_t  size;
    int8_t  RssiPkt;
    int8_t SnrPkt;
}LoRaRxPkt_t;


 void loraInit();
 void loraSend( uint8_t *buffer, uint8_t size,uint8_t setRxOnDone_l );
 void loraSetRxMode( int32_t timeout );
 uint16_t RadioGetIrqStatus( void );
 int loraGetPacket(LoRaRxPkt_t * dataOut);
 int loraGetEvent();
 void loraSetSleepMode( void );
#ifdef __cplusplus
}
#endif
#endif /* LORA_H_ */
