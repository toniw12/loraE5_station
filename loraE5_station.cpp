// Do not remove the include below

#include "loraE5_station.h"

#include <Wire.h>
#include "lora/lora.h"
#include "lora/logUtils.h"
#include "cinterUtils.h"
#include <CircularBuffer.h>

#include "malloc.h"

#include "LinkedList.h"
#include <bmx280Reader.h>
#include <ADCfilteredReader.h>
#include <ArduinoModbus.h>
#include <DS3232RTC.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "tasksketuler.h"
#include "base64.hpp"
#include "FileLogger.h"


#define SD_EN PA9
#define SD_CONFIG SdSpiConfig(PB9, DEDICATED_SPI, SD_SCK_MHZ(50))
SdFat sd;

HardwareSerial rs485_uart(PA3, PA2);
RS485Class rs485(rs485_uart, PA2, PB10, -1);

DS3232RTC i2cRTC;
ADCfilteredReader vsense(PB3,0.0552329);
BMx280Reader bmx280(&Wire);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
String displayText="";

FileLogger dataLogger("dataLog");

#define internalRegisters_LEN 7
unsigned short internalRegisters[internalRegisters_LEN]={0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF};

//StaticJsonDocument<2048> outputData;

//#define LED PB0

typedef  struct
{
    union __attribute__ ((__packed__)) {
        uint8_t data[51];
         struct __attribute__ ((__packed__)){
            uint8_t stationID[2];
            uint8_t cmdNum;
            uint32_t time;
            union __attribute__ ((__packed__)) {
                uint8_t payload[44];
                int16_t payload_int16_t[22];
                int32_t payload_int32_t[11];
            };
        };
    };
    int len;
}dataPack;

//CircularBuffer<dataPack,40> sendData;
LinkedList<dataPack> sendData = LinkedList<dataPack>();

char stationID[]="S1";

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK
            | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}


typedef union
{
    struct {
        uint8_t id;
        uint8_t type;
        uint8_t address;
        uint8_t nb;
    };
    struct{
        uint8_t data[4];
    };
}ModbusCommand;
#define readoutCommands_LEN 16
ModbusCommand readoutCommands[readoutCommands_LEN]={};
#define commandList_LEN (int) (sizeof(commandList) / sizeof(commandFuction))


bool loraSending=false;

bool sdMounted=false;
bool mountSD(){
    pinMode(SD_EN, OUTPUT);
    digitalWrite(SD_EN, HIGH);
    if(!sdMounted){
        if (!sd.begin(SD_CONFIG)) {
            Serial.println("Card failed, or not present");
            sdMounted = false;
            return false;
        } else {
            sdMounted = true;
            return true;
        }
    }
    else{
        return true;
    }
}

void unmountSD(){
    while (sd.card()->isBusy()) {
        volatile int i=0;
    }
    sd.end();
    sdMounted = false;
    digitalWrite(SD_EN, LOW);
}


uint32_t getutctimestamp(){
    tmElements_t now;
    i2cRTC.read(now);
    return makeTime(now);
}

int printText(uint8_t * args,int argc){
    Serial.print("printText(");
    for(int i =0;i< argc;i++){
        Serial.print((char)args[i]);
    }
    args[argc]=0;
    displayText=(char *)args;

    Serial.println(")");
    return 0;
}

int setReadoutCMD(uint8_t * args,int argc){
    Serial.println("setReadoutCMD");
    for(int i =0;i< argc;i++){
        readoutCommands[i/4].data[i%4]=args[i];
    }
    for(int i =argc;i< (int)sizeof(readoutCommands);i++){
        readoutCommands[i/4].data[i%4]=0;
    }
    return 0;
}

int sensorReadout(ModbusCommand * commands,int numCommands){
    mountSD();

    unsigned short mbdata[16];
    int mbdataIndex=0;
    dataPack outD={.len=0};
    outD.stationID[0]=stationID[0];
    outD.stationID[1]=stationID[1];
    int outDoffset=0;
    int outDLen=0;
    outD.time=getutctimestamp();
    for (int i=0;i<numCommands;i++){
        ModbusCommand cmd=commands[i];
        if (cmd.nb>0){
            if(cmd.id>0){
                if (!ModbusRTUClient.requestFrom(cmd.id, cmd.type, cmd.address, cmd.nb)) {
                    Serial.print("failed to read registers! ");
                    Serial.println(ModbusRTUClient.lastError());
                    for (int i = 0; i < cmd.nb; i++) {
                        mbdata[mbdataIndex] = 0x7FFF;
                        mbdataIndex++;
                    }
                } else {
                    for (int i = 0; i < cmd.nb; i++) {
                        mbdata[mbdataIndex] = (unsigned short) ModbusRTUClient.read();
                        mbdataIndex++;
                    }
                }
            }
            else if(cmd.id==0){
                for (int i = 0; i < cmd.nb; i++) {
                    int internalRegisterAddress=i+cmd.address;
                    if(internalRegisterAddress<internalRegisters_LEN){
                        mbdata[mbdataIndex] = internalRegisters[internalRegisterAddress];
                    }
                    else{
                        mbdata[mbdataIndex] = 0x7FFF;
                    }
                    mbdataIndex++;
                }
            }

            if(outDLen+mbdataIndex>22){
                outD.cmdNum=0x80|outDoffset;
                outD.len=outDLen*2+7;
                sendData.add(outD);
                outDoffset+=outDLen;
                outDLen=0;
            }
            //Data
            for (int i = 0; i < mbdataIndex; i++) {
                dataLogger.print(mbdata[i]);
                dataLogger.print(",");
                outD.payload_int16_t[outDLen+i]=mbdata[i];
            }
            outDLen+=mbdataIndex;
            mbdataIndex=0;
        }
    }
    if(outDLen>0){
        outD.cmdNum=0x80|outDoffset;
        outD.len=outDLen*2+7;
        sendData.add(outD);
    }
    dataLogger.println();
    return 0;
}

typedef struct{
    uint8_t cmd;
    int (*function) (uint8_t * args,int argc);
} commandFuction;

void runCommand(uint8_t * args,int argc,bool saveCMDinFile=false);

void updatePortPower();
void sensorReadTask();
void loraSendDataBuffer();
void actualizeDisplay();

enum tasksEnum{
    TASK_PORT_A,
    TASK_PORT_B,
    TASK_PORT_C,
    TASK_SENSOR_READ,
    TASK_TRANSMIT,
};

task_t tasks[]={
    {20,10,-10,0,updatePortPower,NULL},
    {20,5,-5,0,updatePortPower,NULL},
    {20,2,-2,0,updatePortPower,NULL},
    {20,1,0,0,sensorReadTask,NULL},
    {20,5,-10,0,loraSendDataBuffer,NULL},
    //{5,0,0,0,loraSend,NULL},
};
int taskList_LEN = (sizeof(tasks) / sizeof(task_t));

typedef struct{
    int16_t transmit_period;
    int16_t transmit_moment;
    int16_t measurement_period;
    int16_t port_A_onTime;
    int16_t port_B_onTime;
    int16_t port_C_onTime;
} timings_t;


int setTimings(uint8_t * args,int argc){
    if(argc==sizeof(timings_t)){
        timings_t * t= (timings_t *) args;

        Serial.print("measurement_period: ");
        Serial.println(t->measurement_period);
        Serial.print("port_A_onTime: ");
        Serial.println(t->port_A_onTime);
        Serial.print("port_B_onTime: ");
        Serial.println(t->port_B_onTime);
        Serial.print("port_C_onTime: ");
        Serial.println(t->port_C_onTime);
        Serial.print("transmit_period: ");
        Serial.println(t->transmit_period);
        Serial.print("transmit_moment: ");
        Serial.println(t->transmit_moment);

        tasks[TASK_PORT_A].period=t->measurement_period;
        tasks[TASK_PORT_B].period=t->measurement_period;
        tasks[TASK_PORT_C].period=t->measurement_period;
        tasks[TASK_SENSOR_READ].period=t->measurement_period;
        Serial.print("measurement_period: ");
        Serial.println(t->measurement_period);


        tasks[TASK_PORT_A].onTime=t->port_A_onTime;
        tasks[TASK_PORT_B].onTime=t->port_B_onTime;
        tasks[TASK_PORT_C].onTime=t->port_C_onTime;

        tasks[TASK_PORT_A].offset=-t->port_A_onTime;
        tasks[TASK_PORT_B].offset=-t->port_B_onTime;
        tasks[TASK_PORT_C].offset=-t->port_C_onTime;

        tasks[TASK_TRANSMIT].period=t->transmit_period;
        tasks[TASK_TRANSMIT].offset=t->transmit_moment;

        return 0;
    }
    else{
        return -1;
    }
}

commandFuction commandList[]={
    {4,&setReadoutCMD},
    {5,&printText},
    //{6,&commandACK},
    {7,&setTimings},
};

enum cmdNumbers{CMD_testCMD=3,
   CMD_setReadoutCMD=4,
   CMD_printText=5,
   CMD_commandACK=6,
   CMD_setTimings=7,
};


StaticJsonDocument<1024> config;
void loadCMDfromConfigFile(){
    if(!mountSD()){
        Serial.println("loadCMDfromConfigFile: error mounting");
        return;
    }
    Serial.println("loadCMDfromConfigFile");
    File file = sd.open("config.json");
    if(file<=0){
        Serial.println( "loadCMDfromConfigFile: open config.json failed ");
        return;
    }
    DeserializationError error = deserializeJson(config, file);
    if (error) {
      Serial.print( "deserializeJson() failed: ");
      Serial.println( error.c_str());
      return;
    }
    for( const auto& kv : config["startupCMDs"].as<JsonObject>() ) {

        const char * cmd=kv.value().as<const char*>();
        uint8_t args[52];
        int argc;
        argc=decode_base64((unsigned char*)cmd+2,strlen(cmd)-2,(unsigned char*)args+2)+2;//do not decode stationID
        args[0]=cmd[0];
        args[1]=cmd[1];
        runCommand(args,argc);
    }
}

void saveCMDinConfigfile(uint8_t * args,int argc){
    if(!mountSD()){
        Serial.println("saveCMDinConfigfile: error mounting");
        return;
    }
    Serial.println("saveCMDinConfigfile");
    File file = sd.open("config.json", O_WRITE | O_CREAT);
    if(file<=0){
        Serial.println( "saveCMDinConfigfile: open config.json failed ");
        return;
    }


    JsonVariant variant=config["startupCMDs"];
    JsonObject startupCMDs;
    if(variant.is<JsonObject>()){
        startupCMDs=config["startupCMDs"].as<JsonObject>();
    }
    else{
        startupCMDs=config["startupCMDs"].to<JsonObject>();
    }

    unsigned char encodedCommand[100];
    encodedCommand[0]=args[0];
    encodedCommand[1]=args[1];
    int encodedCommandLen=encode_base64((unsigned char*)args+2,argc-2,encodedCommand+2);
    encodedCommand[encodedCommandLen+2]=0;
    char cmdNumStr[5];
    itoa((int)args[2],cmdNumStr,10);
    startupCMDs[cmdNumStr] = encodedCommand;
    serializeJson(config, file);
    file.close();

}



void runCommand(uint8_t * args,int argc,bool saveCMDinFile){
    //cout << args[0]<< args[1] << " cmd:" << (int)args[2] << " " << argc << endl;
    dataPack * cmd=(dataPack*) args;
    if(cmd->stationID[0]==stationID[0] && cmd->stationID[1]==stationID[1]){
        for(int j=0;j<commandList_LEN;j++){
            if(commandList[j].cmd==cmd->cmdNum){
                int32_t retVal=commandList[j].function(args+7,argc-7);

                if(saveCMDinFile){
                    saveCMDinConfigfile(args,argc);
                }
                uint32_t time=getutctimestamp();
                dataPack outD;

                //Station ID
                outD.stationID[0]=stationID[0];
                outD.stationID[1]=stationID[1];
                outD.cmdNum=CMD_commandACK;
                outD.time=time;
                outD.payload_int32_t[0]=cmd->time; // commandID
                Serial.print("commandID: ");
                Serial.println(cmd->time);
                outD.payload_int32_t[1]=retVal;
                outD.len=15;
                if(args[2]!=CMD_commandACK){
                    Serial.println("Push ACK");
                    sendData.add(outD);
                }
            }
        }
    }
    else{
        //loraSend(args,argc);
    }
}


void setup()
{
    //pinMode(LED, OUTPUT);
    //digitalWrite(LED, HIGH);
    Serial.begin(115200);

    Serial.println("Startup\n");

    Wire.begin();
    vsense.begin();
    bmx280.begin();
    i2cRTC.begin();
    tmElements_t now;
    i2cRTC.read(now);

    dataLogger.begin(i2cRTC.read);


    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("No OLED display found");
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 32);
    display.println("¡¡Hola mundo!!");
    display.display();

    loraInit();
    loraSetRxMode(-1);
    if (!ModbusRTUClient.begin(rs485, 9600)) {
        Serial.println("Failed to start Modbus RTU Client!");
    }
    loadCMDfromConfigFile();

    //setSensorPower(1);
    //digitalWrite(LED, LOW);
    //memset(&commands, 0, sizeof commands);
}



void loraSendDataBuffer(){
    while(sendData.size()>0){
        loraSending=true;
        dataPack inD=sendData.shift();
        loraSend(inD.data,inD.len,1);
        actualizeDisplay();
    }
    loraSending=false;
}

extern int loraRxCount;

void updatePortPower();

void sensorReadTask(){

    sensorReadout(readoutCommands,readoutCommands_LEN);
    updatePortPower();
    unmountSD();
}
uint8_t loraInRX=1;
void loraSetRecive(){
    //loraInit();
    loraSetRxMode(-1);
    loraInRX=1;
}

void loraSetSleep(){
    loraSetSleepMode();
    loraInRX=0;
}


uint8_t powerState=0;
void updatePortPower(){
    powerState=0;
    if(tasks[0].active){
        powerState|=1<<3;
    }
    else{
        volatile int k=0;
    }
    if(tasks[1].active){
        powerState|=1<<7;
    }
    else{
        volatile int k=0;
    }
    if(tasks[2].active){
        powerState|=1<<5;
    }
    else{
        volatile int k=0;
    }
    setSensorPower(powerState);
}

void actualizeDisplay(){
    char timeStr[32];
    tmElements_t now;
    i2cRTC.read(now);
    int nowint=makeTime(now);
    formatDateTime(timeStr, 32, now);

    display.fillRect(0, 0, 128, 64, 0);
    display.setCursor(0, 0);
    display.println(timeStr);
    display.print("rxCnt: ");
    display.println(loraRxCount);
    display.println(displayText);

    struct mallinfo mi = mallinfo();
    display.print("Mem ");
    display.println(mi.uordblks);


    for(int i=0;i<8;i++){
        char pwrText[]="   ";
        if(powerState&1<<3){
            pwrText[0]='A';
        }
        if(powerState&1<<7){
            pwrText[1]='B';
        }
        if(powerState&1<<5){
            pwrText[2]='C';
        }
        display.setCursor(10, 52);
        display.print(pwrText);

    }
    if (loraSending){
        display.setCursor(120, 52);
        display.print("T");
    }
    else if (loraInRX){
        display.setCursor(120, 52);
        display.print("R");
    }
    display.display();
}

void loop(){
    int t0=getCurrentMillis();


    actualizeDisplay();
    tmElements_t now;
    i2cRTC.read(now);
    int nowint=makeTime(now);
    runTasks(nowint);

    LoRaRxPkt_t pkt;
    while(loraGetPacket(&pkt)==0){
        runCommand(pkt.RadioBuffer,pkt.size,true);
        //pkt.RadioBuffer[ pkt.size]=0;
        //Serial.println((char *)pkt.RadioBuffer);
    }

    internalRegisters[0]=vsense.measure();
    bmx280.measure();
    for(int i=0;i<4;i++){
        internalRegisters[i+1]=bmx280.registers[i];
    }

    int tsleep=200-(getCurrentMillis()-t0);
    if(tsleep>200){
        tsleep=200;
    }
    if(tsleep>0){
        delay(tsleep);
    }
}
