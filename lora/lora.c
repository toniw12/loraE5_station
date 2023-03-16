#include "stm32wlxx_hal_subghz.h"
#include "stm32wlxx_hal.h"
#include "stm32wlxx_ll_rcc.h"
#include "stm32wlxx.h"
#include "stm32yyxx_ll.h"
#include "subghz.h"
#include "radio_driver.h"
#include "lora.h"

#define RF_FREQUENCY                                868000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             22        // dBm tx output power
#define LORA_BANDWIDTH                              0         // bandwidth=125khz  0:250kHZ,1:125kHZ,2:62kHZ,3:20kHZ.... look for radio line 392
#define LORA_SPREADING_FACTOR                       12         // spreading factor=11 [SF5..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false     // variable data payload
#define LORA_IQ_INVERSION_ON                        false
#define LORA_PAYLOADLENGTH                          0         // 0: variable receive length
// 1..255 payloadlength

//#define LORA_BANDWIDTH                              0         /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
//#define LORA_SPREADING_FACTOR                       8         /* [SF7..SF12] */
//#define LORA_CODINGRATE                             1         /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
//#define LORA_PREAMBLE_LENGTH                        8         /* Same for Tx and Rx */
//#define LORA_SYMBOL_TIMEOUT                         5         /* Symbols */
//#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
//#define LORA_IQ_INVERSION_ON                        false

typedef struct SubgRf_s
{
    bool RxContinuous;
    uint32_t TxTimeout;
    uint32_t RxTimeout;
    struct
    {
        bool Previous;
        bool Current;
    } PublicNetwork;
    PacketParams_t PacketParams;
    PacketStatus_t PacketStatus;
    ModulationParams_t ModulationParams;
    RadioIrqMasks_t RadioIrq;
    uint8_t AntSwitchPaSelect;
    uint32_t RxDcPreambleDetectTimeout; /* 0:RxDutyCycle is off, otherwise on with  2*rxTime + sleepTime (See STM32WL Errata: RadioSetRxDutyCycle)*/
} SubgRf_t;


typedef struct {
    LoRaRxPkt_t data[LoRa_circ_bbuf_LEN];
    int head;
    int tail;
} LoRa_circ_bbuf_t;

LoRa_circ_bbuf_t LoRa_RxBuf;

SubgRf_t SubgRf;
int rxBufferWr=0;
uint8_t setRxOnDone=0;
uint8_t sending=false;

static uint8_t MaxPayloadLength = 255;
const RadioLoRaBandwidths_t Bandwidths[] = { LORA_BW_125, LORA_BW_250, LORA_BW_500 };

int loraGetPacket(LoRaRxPkt_t * dataOut)
{
    int next;
    if (LoRa_RxBuf.head == LoRa_RxBuf.tail)
        return -1;

    next = LoRa_RxBuf.tail + 1;
    if(next >= LoRa_circ_bbuf_LEN)
        next = 0;

    memcpy(dataOut,&LoRa_RxBuf.data[LoRa_RxBuf.tail],sizeof(LoRaRxPkt_t));
    LoRa_RxBuf.tail = next;
    return 0;
}


int loraRxCount=0;
void SUBGHZ_Radio_IRQHandler(void)
{
    uint8_t tmpisr[2] = {0};
    uint16_t itsource;

    /* Retrieve Interrupts from SUBGHZ Irq Register */
    (void)HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_IRQSTATUS, tmpisr, 2);
    itsource = tmpisr[0];
    itsource = (itsource << 8) | tmpisr[1];

    /* Packet transmission completed Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_TX_CPLT) != RESET)
    {
        volatile int i=0;
        if (setRxOnDone){
            loraSetRxMode(-1);
        }
        sending=false;
    }

    /* Packet received Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_RX_CPLT) != RESET)
    {
        loraRxCount++;
        int next;
        next = LoRa_RxBuf.head + 1;  // next is where head will point to after this write.
        if (next >= LoRa_circ_bbuf_LEN)
            next = 0;

        if (next == LoRa_RxBuf.tail)  // if the head + 1 == tail, circular buffer is full
        {
            while(1); // RxBuffer owerflow
        }

        LoRaRxPkt_t * data= &LoRa_RxBuf.data[LoRa_RxBuf.head];  // Load data and then move

        SUBGRF_GetPayload( &data->RadioBuffer, &data->size, LoRa_RadioBuffer_LEN );
        SUBGRF_GetPacketStatus( &( SubgRf.PacketStatus ) );
        data->RssiPkt =SubgRf.PacketStatus.Params.LoRa.RssiPkt;
        data->SnrPkt =SubgRf.PacketStatus.Params.LoRa.SnrPkt;

        LoRa_RxBuf.head = next;             // head to next data offset.
        // return success to indicate successful push.
        loraSetRxMode(-1);
    }

    /* Preamble Detected Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_PREAMBLE_DETECTED) != RESET)
    {

    }

    /*  Valid sync word detected Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_SYNCWORD_VALID) != RESET)
    {

    }

    /* Valid LoRa header received Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_HEADER_VALID) != RESET)
    {

    }

    /* LoRa header CRC error Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_HEADER_ERROR) != RESET)
    {
    }

    /* Wrong CRC received Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_CRC_ERROR) != RESET)
    {
    }

    /* Channel activity detection finished Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_CAD_DONE) != RESET)
    {
      /* Channel activity Detected Interrupt */
      if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_CAD_ACTIVITY_DETECTED) != RESET)
      {

      }
    }

    /* Rx or Tx Timeout Interrupt */
    if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_RX_TX_TIMEOUT) != RESET)
    {
    }

    /* Clear SUBGHZ Irq Register */
    (void)HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_CLR_IRQSTATUS, tmpisr, 2);
}

static void RadioStandby( void )
{
    SUBGRF_SetStandby( STDBY_RC );
}
static void RadioInit(  )
{
	SUBGRF_Init( NULL );
    SUBGRF_SetRegulatorMode( );
    SUBGRF_SetBufferBaseAddress( 0x00, 0x00 );
    SUBGRF_SetTxParams( RFO_LP, 0, RADIO_RAMP_200_US );
    SUBGRF_SetDioIrqParams( IRQ_RADIO_ALL, IRQ_RADIO_ALL, IRQ_RADIO_NONE, IRQ_RADIO_NONE );
    /* ST_WORKAROUND_BEGIN: Sleep radio */
    SleepParams_t params = { 0 };

    params.Fields.WarmStart = 1;
    SUBGRF_SetSleep( params );
    LoRa_RxBuf.head=0;
    LoRa_RxBuf.tail=0;

    HAL_NVIC_EnableIRQ(SUBGHZ_Radio_IRQn);
}

void loraSetSleepMode( void )
{
    SleepParams_t params = { 0 };

    params.Fields.WarmStart = 1;
    SUBGRF_SetSleep( params );

    delay( 2 );
}

static void RadioSetModem(  )
{

        SUBGRF_SetPacketType( PACKET_TYPE_LORA );
}

static void RadioSetTxConfig( int8_t power, uint32_t fdev,
                              uint32_t bandwidth, uint32_t datarate,
                              uint8_t coderate, uint16_t preambleLen,
                              bool fixLen, bool crcOn, bool freqHopOn,
                              uint8_t hopPeriod, bool iqInverted, uint32_t timeout );

static void RadioSetTxConfig( int8_t power, uint32_t fdev,
                              uint32_t bandwidth, uint32_t datarate,
                              uint8_t coderate, uint16_t preambleLen,
                              bool fixLen, bool crcOn, bool freqHopOn,
                              uint8_t hopPeriod, bool iqInverted, uint32_t timeout )
{

   // APP_LOG(TS_OFF, VLEVEL_M, "RadioSetTxConfig(modem=%d)\n\r",(int) modem);

            SubgRf.ModulationParams.PacketType = PACKET_TYPE_LORA;
            SubgRf.ModulationParams.Params.LoRa.SpreadingFactor = ( RadioLoRaSpreadingFactors_t ) datarate;
            SubgRf.ModulationParams.Params.LoRa.Bandwidth =  Bandwidths[bandwidth];
            SubgRf.ModulationParams.Params.LoRa.CodingRate= ( RadioLoRaCodingRates_t )coderate;

            if( ( ( bandwidth == 0 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
                ( ( bandwidth == 1 ) && ( datarate == 12 ) ) )
            {
                SubgRf.ModulationParams.Params.LoRa.LowDatarateOptimize = 0x01;
            }
            else
            {
                SubgRf.ModulationParams.Params.LoRa.LowDatarateOptimize = 0x00;
            }

            SubgRf.PacketParams.PacketType = PACKET_TYPE_LORA;

            if( ( SubgRf.ModulationParams.Params.LoRa.SpreadingFactor == LORA_SF5 ) ||
                ( SubgRf.ModulationParams.Params.LoRa.SpreadingFactor == LORA_SF6 ) )
            {
                if( preambleLen < 12 )
                {
                    SubgRf.PacketParams.Params.LoRa.PreambleLength = 12;
                }
                else
                {
                    SubgRf.PacketParams.Params.LoRa.PreambleLength = preambleLen;
                }
            }
            else
            {
                SubgRf.PacketParams.Params.LoRa.PreambleLength = preambleLen;
            }
            SubgRf.PacketParams.Params.LoRa.HeaderType = ( RadioLoRaPacketLengthsMode_t )fixLen;
            SubgRf.PacketParams.Params.LoRa.PayloadLength = MaxPayloadLength;
            SubgRf.PacketParams.Params.LoRa.CrcMode = ( RadioLoRaCrcModes_t )crcOn;
            SubgRf.PacketParams.Params.LoRa.InvertIQ = ( RadioLoRaIQModes_t )iqInverted;

            RadioStandby( );
            RadioSetModem(  );
            SUBGRF_SetModulationParams( &SubgRf.ModulationParams );
            SUBGRF_SetPacketParams( &SubgRf.PacketParams );


    SubgRf.AntSwitchPaSelect = SUBGRF_SetRfTxPower( power );
    //RFW_SetAntSwitch( SubgRf.AntSwitchPaSelect ); /* ST_WORKAROUND: ?????? */
    SubgRf.TxTimeout = timeout;
}

static void RadioSetRxConfig( uint32_t bandwidth,
                              uint32_t datarate, uint8_t coderate,
                              uint32_t bandwidthAfc, uint16_t preambleLen,
                              uint16_t symbTimeout, bool fixLen,
                              uint8_t payloadLen,
                              bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                              bool iqInverted, bool rxContinuous );


static void RadioSetRxConfig( uint32_t bandwidth,
                              uint32_t datarate, uint8_t coderate,
                              uint32_t bandwidthAfc, uint16_t preambleLen,
                              uint16_t symbTimeout, bool fixLen,
                              uint8_t payloadLen,
                              bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                              bool iqInverted, bool rxContinuous )
{
	//APP_LOG(TS_OFF, VLEVEL_M, "RadioSetRxConfig()\n\r");
    SubgRf.RxContinuous = rxContinuous;
    //RFW_DeInit(); /* ST_WORKAROUND: Switch Off FwPacketDecoding by default */
    if( rxContinuous == true )
    {
        symbTimeout = 0;
    }
    if( fixLen == true )
    {
        MaxPayloadLength = payloadLen;
    }
    else
    {
        MaxPayloadLength = 0xFF;
    }
	SUBGRF_SetStopRxTimerOnPreambleDetect( false );
	SubgRf.ModulationParams.PacketType = PACKET_TYPE_LORA;
	SubgRf.ModulationParams.Params.LoRa.SpreadingFactor = ( RadioLoRaSpreadingFactors_t )datarate;
	SubgRf.ModulationParams.Params.LoRa.Bandwidth = Bandwidths[bandwidth];
	SubgRf.ModulationParams.Params.LoRa.CodingRate = ( RadioLoRaCodingRates_t )coderate;

	if( ( ( bandwidth == 0 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
		( ( bandwidth == 1 ) && ( datarate == 12 ) ) )
	{
		SubgRf.ModulationParams.Params.LoRa.LowDatarateOptimize = 0x01;
	}
	else
	{
		SubgRf.ModulationParams.Params.LoRa.LowDatarateOptimize = 0x00;
	}

	SubgRf.PacketParams.PacketType = PACKET_TYPE_LORA;

	if( ( SubgRf.ModulationParams.Params.LoRa.SpreadingFactor == LORA_SF5 ) ||
		( SubgRf.ModulationParams.Params.LoRa.SpreadingFactor == LORA_SF6 ) )
	{
		if( preambleLen < 12 )
		{
			SubgRf.PacketParams.Params.LoRa.PreambleLength = 12;
		}
		else
		{
			SubgRf.PacketParams.Params.LoRa.PreambleLength = preambleLen;
		}
	}
	else
	{
		SubgRf.PacketParams.Params.LoRa.PreambleLength = preambleLen;
	}

	SubgRf.PacketParams.Params.LoRa.HeaderType = ( RadioLoRaPacketLengthsMode_t )fixLen;

	SubgRf.PacketParams.Params.LoRa.PayloadLength = MaxPayloadLength;
	SubgRf.PacketParams.Params.LoRa.CrcMode = ( RadioLoRaCrcModes_t )crcOn;
	SubgRf.PacketParams.Params.LoRa.InvertIQ = ( RadioLoRaIQModes_t )iqInverted;

	RadioStandby( );
	RadioSetModem( /*MODEM_LORA*/ );
	SUBGRF_SetModulationParams( &SubgRf.ModulationParams );
	SUBGRF_SetPacketParams( &SubgRf.PacketParams );
	SUBGRF_SetLoRaSymbNumTimeout( symbTimeout );

	// WORKAROUND - Optimizing the Inverted IQ Operation, see STM32WL Erratasheet
	if( SubgRf.PacketParams.Params.LoRa.InvertIQ == LORA_IQ_INVERTED )
	{
		// RegIqPolaritySetup = @address 0x0736
		SUBGRF_WriteRegister( SUBGHZ_LIQPOLR, SUBGRF_ReadRegister( SUBGHZ_LIQPOLR ) & ~( 1 << 2 ) );
	}
	else
	{
		// RegIqPolaritySetup @address 0x0736
		SUBGRF_WriteRegister( SUBGHZ_LIQPOLR, SUBGRF_ReadRegister( SUBGHZ_LIQPOLR ) | ( 1 << 2 ) );
	}
	// WORKAROUND END

	// Timeout Max, Timeout handled directly in SetRx function
	SubgRf.RxTimeout = 0xFFFF;

}

uint16_t RadioGetIrqStatus( void ){
	return SUBGRF_GetIrqStatus();
}

void loraSetRxMode( int32_t timeout )
{
    SUBGRF_SetDioIrqParams( IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR | IRQ_HEADER_ERROR ,
                                IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR | IRQ_HEADER_ERROR ,
                                IRQ_RADIO_NONE,
                                IRQ_RADIO_NONE );

    /* ST_WORKAROUND_BEGIN : Set the debug pin and update the radio switch */
    /* switch off RxDcPreambleDetect See STM32WL Errata: RadioSetRxDutyCycle*/
    SubgRf.RxDcPreambleDetectTimeout = 0;
    /* RF switch configuration */
    SUBGRF_SetSwitch( SubgRf.AntSwitchPaSelect, RFSWITCH_RX );
    /* ST_WORKAROUND_END */

    if( timeout == -1 )
    {
        SUBGRF_SetRx( 0xFFFFFF ); // Rx Continuous
    }
    else
    {
        SUBGRF_SetRx( timeout << 6 );
    }
}


 void loraSend( uint8_t *buffer, uint8_t size ,uint8_t setRxOnDone_l)
{
	//APP_LOG(TS_OFF, VLEVEL_M, "loraSend(size=%d)\n\r",(int) size);
    /* ST_WORKAROUND_BEGIN : Set the debug pin and update the radio switch */

     while(sending);

    SUBGRF_SetDioIrqParams( IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT,
                            IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT,
                            IRQ_RADIO_NONE,
                            IRQ_RADIO_NONE );
    /* ST_WORKAROUND_END */

    /* ST_WORKAROUND_BEGIN : Set the debug pin and update the radio switch */
    /* Set DBG pin */
    //DBG_GPIO_RADIO_TX( SET );

    /* Set RF switch */
    SUBGRF_SetSwitch( SubgRf.AntSwitchPaSelect, RFSWITCH_TX );
    /* WORKAROUND - Modulation Quality with 500 kHz LoRaTM Bandwidth*/
    /* RegTxModulation = @address 0x0889 */
    if(  SubgRf.ModulationParams.Params.LoRa.Bandwidth == LORA_BW_500  )
    {
        SUBGRF_WriteRegister( SUBGHZ_SDCFG0R, SUBGRF_ReadRegister( SUBGHZ_SDCFG0R ) & ~( 1 << 2 ) );
    }
    else
    {
        SUBGRF_WriteRegister( SUBGHZ_SDCFG0R, SUBGRF_ReadRegister( SUBGHZ_SDCFG0R ) | ( 1 << 2 ) );
    }
	SubgRf.PacketParams.Params.LoRa.PayloadLength = size;
	SUBGRF_SetPacketParams( &SubgRf.PacketParams );
	SUBGRF_SendPayload( buffer, size, 0 );
	setRxOnDone=setRxOnDone_l;
	sending=true;
}

 void loraInit(){
	MX_SUBGHZ_Init();
	RadioInit(  );

	SUBGRF_SetRfFrequency( RF_FREQUENCY );
	RadioSetTxConfig(TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
	                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
	                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
	                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

	RadioSetRxConfig(LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
	                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
	                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
	                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
	HAL_NVIC_EnableIRQ(SUBGHZ_Radio_IRQn);
}


 int loraGetEvent()
 {
   int retVal= IRQ_RADIO_NONE;
   uint8_t tmpisr[2U] = {0U};
   uint16_t itsource;

   /* Retrieve Interrupts from SUBGHZ Irq Register */
   (void)HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_IRQSTATUS, tmpisr, 2U);
   itsource = tmpisr[0U];
   itsource = (itsource << 8U) | tmpisr[1U];

   /* Packet transmission completed Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_TX_CPLT) != RESET)
   {
     retVal= IRQ_TX_DONE;
   }
   /* Packet received Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_RX_CPLT) != RESET)
   {
	   retVal= IRQ_RX_DONE;
   }
   /* Preamble Detected Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_PREAMBLE_DETECTED) != RESET)
   {
	   retVal= IRQ_PREAMBLE_DETECTED;
   }
   /*  Valid sync word detected Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_SYNCWORD_VALID) != RESET)
   {
	   retVal= IRQ_SYNCWORD_VALID;
   }
   /* Valid LoRa header received Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_HEADER_VALID) != RESET)
   {
	   retVal= IRQ_HEADER_VALID;
   }
   /* LoRa header CRC error Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_HEADER_ERROR) != RESET)
   {
	   retVal= IRQ_HEADER_ERROR;
   }
   /* Wrong CRC received Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_CRC_ERROR) != RESET)
   {
	   retVal= IRQ_CRC_ERROR;
   }
   /* Channel activity detection finished Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_CAD_DONE) != RESET)
   {
     /* Channel activity Detected Interrupt */
     if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_CAD_ACTIVITY_DETECTED) != RESET)
     {
    	 retVal= IRQ_CAD_DETECTED;
     }
     else
     {
    	 retVal= IRQ_CAD_CLEAR;
     }
   }

   /* Rx or Tx Timeout Interrupt */
   if (SUBGHZ_CHECK_IT_SOURCE(itsource, SUBGHZ_IT_RX_TX_TIMEOUT) != RESET)
   {
	   retVal= IRQ_RX_TX_TIMEOUT;
   }

   /* Clear SUBGHZ Irq Register */
   (void)HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_CLR_IRQSTATUS, tmpisr, 2);

   return retVal;
 }



 /*void loraGetPacket(uint8_t * RadioBuffer,uint8_t * size,int8_t * RssiPkt,int8_t *SnrPkt){

	         SUBGRF_GetPayload( RadioBuffer, size, 255 );
	         SUBGRF_GetPacketStatus( &( SubgRf.PacketStatus ) );
	         *RssiPkt=SubgRf.PacketStatus.Params.LoRa.RssiPkt;
	         *SnrPkt=SubgRf.PacketStatus.Params.LoRa.SnrPkt;
	      //RadioEvents->RxDone( RadioBuffer, size, SubgRf.PacketStatus.Params.LoRa.RssiPkt, SubgRf.PacketStatus.Params.LoRa.SnrPkt );
 }
*/
