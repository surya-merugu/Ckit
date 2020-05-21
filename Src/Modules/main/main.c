/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_main.c
  * Origin Date           :   08/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :
  * Target                :   STM32F407-DISCOVERY
  * Notes                 :   None
  *****************************************************************************
  * @attention
  *
  * <h2><center>&copy;Copyright (C) RACEnergy, Inc - All Rights Reserved.
  * </center></h2>
  *
  * Unauthorized copying of this file, via any medium is strictly prohibited
  * Proprietary and confidential
  *
  * Written by Team RACEnergy, FEB 2020
  *****************************************************************************
  */

/* Includes */
#include "main.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"

LogData_t LogData;
static const LogData_t nodata_t;
extern TIM_HandleTypeDef htim3_t;
extern FSTNLCD_data_t LcdDisplay;
extern uint8_t key_status;
extern uint8_t animate;
extern uint8_t save_odo;
extern uint8_t SPI_Tx_Flag;
extern uint8_t range;

uint32_t pclk1, pclk2;
uint8_t uart_rcvd_byte;
//uint8_t rcvd_data_buffer;
uint8_t count = 0;
uint8_t t_10s;

extern UART_HandleTypeDef huart3_t;
extern SPI_HandleTypeDef hspi1_t;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;
gps_t hgps_t;


static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDIO_SD_Init(void);

volatile uint32_t distance;
/* USER CODE BEGIN PV */
/* SD Card Variables FATFS ---------------------------------------------------*/
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS;  /* File system object for SD logical drive */
extern FIL SDFile;     /* File object for SD */

//FILE I/O Variables

/* Capacity related variables */
FATFS *pfs;
FRESULT fres;
FILINFO fno;

DWORD fre_clust;
uint32_t totalSpace, freeSpace;
uint8_t buff_len = 150;

uint16_t err_val;
char buffer[150];
char sd_buf[50];
static char batData_buffer[100];
uint8_t battery_info = 0;
uint32_t SPI_Kit_Msg_ID;
uint16_t SPI_Bat_Msg_ID;
uint8_t KitStatus;
uint8_t SPI_Tx_PhyID_Flag;
uint32_t SPI_KIT_dataBuffer[4];


// FRESULT res;                                               /* FatFs function common result code */
uint32_t byteswritten, bytesread;                          /* File write/read counts */
uint8_t wtext[] = "Hello from Abhilash :), SDIO DMA RTOS"; /* File write buffer */
uint8_t odo_text[];
uint8_t rtext[100];

/* To find out the size of the data in the buffer */
int bufsize(char *pbuf)
{
  int i = 0;
  while (*pbuf++ != '\0')
  {
    i++;
  }
  return i;
}

void clearbuf(void)
{
  for (int i = 0; i < buff_len; i++)
  {
    buffer[i] = '\0';
  }
}



/* Mount SD Card */
void sd_mount(void)
{
  fres = f_mount(&SDFatFS, (TCHAR const *)SDPath, 0);
  /************ Card capacity details ************/
  /* Check free space */
  if (f_getfree("", &fre_clust, &pfs) != FR_OK)
     RE_Error_Handler(__FILE__, __LINE__);

  totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  sprintf(buffer, "SD CARD Total Size: \t%lu\n", totalSpace);
//  printf(buffer);
  clearbuf();
  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
  sprintf(buffer, "SD Card Free Space: \t%1u\n", freeSpace);
//  printf(buffer);
  
  /*
  * Check if directory is existing
  * If not existing create one
  */
 // f_mkdir("battery_info");
#if 0
  fres = f_stat("bat1", NULL);
  if (fres != FR_OK)
  {
    f_mkdir("bat1");
  }
  fres = f_stat("bat2", NULL);
  if (fres != FR_OK)
  {
    f_mkdir("bat2");
  }
  fres = f_stat("bat3", NULL);
  if (fres != FR_OK)
  {
    f_mkdir("bat3");
  }
  fres = f_stat("bat4", NULL);
  if (fres != FR_OK)
  {
    f_mkdir("bat4");
  }

#endif  
//  fres = f_stat("batt_info", NULL);
//  if (fres != FR_OK)
//  {
//    f_mkdir("batt_info");
//  }
//
//  fres = f_stat("controller1", NULL);
//  if (fres != FR_OK)
//  {
//    f_mkdir("controller1");
//  }
}

/** Log @pData into @pFile_name */
void AppendLog(char *pFileName, char *pData)
{
  fres = f_open(&SDFile, pFileName, FA_OPEN_APPEND | FA_READ | FA_WRITE);
  fres = f_puts(pData, &SDFile);
  sprintf(buffer, "%s is created and data is written \n", pFileName);
//  printf(buffer);

  /* Close file */
  fres = f_close(&SDFile);

  /* Open file to read */
  fres = f_open(&SDFile, pFileName, FA_READ);

  /* Read String from the file */
  f_gets(buffer, sizeof(buffer), &SDFile);
//  printf(buffer);

  /* Display free space */
  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
  sprintf(buffer, "SD Card Free Space: \t%1u\n\n", freeSpace);
//  printf(buffer);
  clearbuf();
  /* Close file */
  f_close(&SDFile);
}

void OverWriteLog(char *pFileName, char *pData)
{
  if (f_open(&SDFile, pFileName, FA_OPEN_ALWAYS | FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
//    printf("Failed to open Write file\r\n");
  }
  else
  {
//    printf("Opened Write file successfully\r\n");
    //Write data to text file
    fres = f_write(&SDFile, pData, strlen((char *)pData), (void *)&byteswritten);
    if ((byteswritten == 0) || (fres != FR_OK))
    {
//      printf("Failed to write file!\r\n");
    }
    else
    {
//      printf("File written successfully\r\n");
//      printf("Write Content: %s\r\n", pData);
    }
    f_close(&SDFile);
  }
}

uint32_t ReadLogFile(char *pFileName, uint8_t len)
{
  int Data = 0;
  /* Open file to read */
  fres = f_open(&SDFile, pFileName, FA_READ);
  memset(rtext, 0, sizeof(rtext));
  fres = f_read(&SDFile, rtext, sizeof(rtext), (UINT *)&bytesread);
  if ((bytesread == 0) || (fres != FR_OK))
  {
//    printf("Failed to read file!\r\n");
  }
  else
  {
//    printf("File read successfully\r\n");
//    printf("File content: %s\r\n", (char *)rtext);
    Data = atoi(rtext);
  }
  f_close(&SDFile);
  return Data;
}


/**SPI Transmit function**/

RE_StatusTypeDef RE_SPI_Trasmit_Msg(void)
{
    SPI_Tx_PhyID_Flag++;
    SPI_Kit_Msg_ID = 101;
    SPI_KIT_dataBuffer[0] = SPI_Kit_Msg_ID;
    SPI_KIT_dataBuffer[1] = LcdDisplay.odo;
    SPI_KIT_dataBuffer[2] = range;
    SPI_KIT_dataBuffer[3] = KitStatus;
    HAL_SPI_Transmit(&hspi1_t, (uint8_t *)SPI_KIT_dataBuffer, sizeof(SPI_KIT_dataBuffer), 100);

    SPI_Bat_Msg_ID = 201;
    uint16_t SPI_BAT0_dataBuffer[] = {SPI_Bat_Msg_ID, LogData.bat[0].soc, LogData.bat[0].volt, LogData.bat[0].temp,\
                                       LogData.bat[0].current, LogData.bat[0].remain_cap, LogData.bat[0].avail_energy,\
                                       LogData.bat[0].cycle_count, LogData.bat[0].soh};
    HAL_SPI_Transmit(&hspi1_t, (uint8_t *)SPI_BAT0_dataBuffer, sizeof(SPI_BAT0_dataBuffer), 100);
    
    SPI_Bat_Msg_ID = 202;
    uint16_t SPI_BAT1_dataBuffer[] = {SPI_Bat_Msg_ID, LogData.bat[1].soc, LogData.bat[1].volt, LogData.bat[1].temp,\
                                       LogData.bat[1].current, LogData.bat[1].remain_cap, LogData.bat[1].avail_energy,\
                                       LogData.bat[1].cycle_count, LogData.bat[1].soh};
    HAL_SPI_Transmit(&hspi1_t, (uint8_t *)SPI_BAT1_dataBuffer, sizeof(SPI_BAT1_dataBuffer), 100);

    SPI_Bat_Msg_ID = 203;
    uint16_t SPI_BAT2_dataBuffer[] = {SPI_Bat_Msg_ID, LogData.bat[2].soc, LogData.bat[2].volt, LogData.bat[2].temp,\
                                       LogData.bat[2].current, LogData.bat[2].remain_cap, LogData.bat[2].avail_energy,\
                                       LogData.bat[2].cycle_count, LogData.bat[2].soh};
    HAL_SPI_Transmit(&hspi1_t, (uint8_t *)SPI_BAT2_dataBuffer, sizeof(SPI_BAT2_dataBuffer), 100);  
    
    SPI_Bat_Msg_ID = 204;
    uint16_t SPI_BAT3_dataBuffer[] = {SPI_Bat_Msg_ID, LogData.bat[3].soc, LogData.bat[3].volt, LogData.bat[3].temp,\
                                       LogData.bat[3].current, LogData.bat[3].remain_cap, LogData.bat[3].avail_energy,\
                                       LogData.bat[3].cycle_count, LogData.bat[3].soh};
    HAL_SPI_Transmit(&hspi1_t, (uint8_t *)SPI_BAT3_dataBuffer, sizeof(SPI_BAT3_dataBuffer), 100);
    
    if(SPI_Tx_PhyID_Flag == 5)
    {
        char SPI_PhyID_dataBuffer[5][12];
        strcpy(SPI_PhyID_dataBuffer[0], "301");
        strcpy(SPI_PhyID_dataBuffer[0], "RACEB02A001");
        strcpy(SPI_PhyID_dataBuffer[0], "RACEB02A002");
        strcpy(SPI_PhyID_dataBuffer[0], "RACEB02A003");
        strcpy(SPI_PhyID_dataBuffer[0], "RACEB02A004");
        HAL_SPI_Transmit(&hspi1_t, (uint8_t *)SPI_PhyID_dataBuffer, sizeof(SPI_PhyID_dataBuffer), 100);
        SPI_Tx_PhyID_Flag = 0;
    } 
    return RE_OK;
}

 int main(void)
{
 // static uint32_t success_counter = 0U;
  
   /*start test sd card*/
#if 0
    //time stamp
    LogData.time_stamp.dd = 27;
    LogData.time_stamp.mm = 2;
    LogData.time_stamp.yy = 2020;
    LogData.time_stamp.hrs = 1;
    LogData.time_stamp.min = 23;
    LogData.time_stamp.sec = 20;
    
    
    
    //battery parameters
    for(uint8_t i=0; i<4; i++)
    {
        LogData.bat[i].ID = 1000;
        LogData.bat[i].soc = 87;
        LogData.bat[i].volt = 54.6;
        LogData.bat[i].temp = 30;
        LogData.bat[i].current = 30;
        LogData.bat[i].remain_cap = 50;
        LogData.bat[i].avail_energy = 60;
        LogData.bat[i].cycle_count = 1;
        LogData.bat[i].soh = 90;
        LogData.bat[i].full_cap = 90;
    }
    
    //controller parameters
    LogData.ctrl.rpm = 2800;
    LogData.ctrl.err_code = 2;
    

    
    
#endif
    
    
  /*end test sd card*/
  HAL_Init();
  RE_SystemClock_Config();
  RE_LCD_GpioInit();
  RE_key_GpioInit();
  RE_DockLatch_GpioInit();
  RE_Gear_GpioInit();
 // RE_RevLight_GpioInit();
 +
   RE_Speed_GpioInit();
   RE_TIMER3_Init();
   HAL_TIM_Base_Start_IT(&htim3_t);
   MX_USART3_UART_Init();
//   RE_RTC_Init();
//   RE_SetTimeStamp();
  RE_CAN1_Init();
  RE_CAN1_Filter_Config();
  RE_CAN1_Start_Interrupt();

  /* Initialize all configured peripherals */

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  sd_mount();

#if 0
  /*start testing sd card*/

       strcpy(LogData.Dir,"controller/");
       sprintf(LogData.file_name, "%s%d-%d-%d.txt", LogData.Dir, LogData.time_stamp.dd, LogData.time_stamp.mm,\
                LogData.time_stamp.yy);
       LogData.pfile_name = LogData.file_name;
       sprintf(sd_buf, "%d/%d/%d,%d:%d:%d,%d,%d\n",LogData.time_stamp.dd, LogData.time_stamp.mm,LogData.time_stamp.yy,\
                LogData.time_stamp.hrs,LogData.time_stamp.min, LogData.time_stamp.sec,LogData.ctrl.rpm,\
                LogData.ctrl.err_code);
       AppendLog(LogData.pfile_name, sd_buf);
       
      
      memset(sd_buf, 0, sizeof(sd_buf));

       /*battery data*/
       
       
       for(uint8_t i = 0; i<4; i++)
       {
            sprintf(sd_buf, "%d/%d/%d,%d:%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", LogData.time_stamp.dd,
                     LogData.time_stamp.mm,  LogData.time_stamp.yy,LogData.time_stamp.hrs, LogData.time_stamp.min,\
                     LogData.time_stamp.sec,LogData.bat[i].soc, LogData.bat[i].volt, LogData.bat[i].temp,\
                     LogData.bat[i].current, LogData.bat[i].remain_cap,LogData.bat[i].avail_energy,\
                     LogData.bat[i].cycle_count,LogData.bat[i].soh, LogData.bat[i].full_cap);
            strcat(batData_buffer, sd_buf);
           
            memset(sd_buf, 0, sizeof(sd_buf));
       }
       
       strcpy(LogData.Dir,"batt_info/");
       sprintf(LogData.file_name, "%s%d.txt", LogData.Dir,2000);
       LogData.pfile_name = LogData.file_name;
       

       AppendLog(LogData.pfile_name, batData_buffer);
       
       memset(batData_buffer, 0, sizeof(batData_buffer));
#endif            
    /*end testing sd card*/ 
  
  pclk1 = HAL_RCC_GetPCLK1Freq();
  pclk2 = HAL_RCC_GetPCLK2Freq();
  
  //Ringbuf_Init();
#if 0
  /* Try till MC60 module is initialized sucessfully*/
  while ((MC60_Init() != MC60_OK))
  {
    HAL_Delay(1000);
  }
  Ringbuf_Free();
  (void)MC60_GNSS_Read_Nav_Info_to_GPS_DATA();
//  RE_SetTimeStamp(&hgps_t);
//   while (MC60_TCPIP_Init() != MC60_TCPIP_OK) //try till TCPIP is initialized successfully.
//   {
//   }
   Ringbuf_Free();

  /* Infinite loop */
  for (;;)
  {
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);

//     (void)MC60_Read_Send_GPS_Data_on_TCPIP();
    (void)MC60_GNSS_Read_Nav_Info_to_GPS_DATA();
    HAL_Delay(5000);
    Ringbuf_Free();
    HAL_Delay(500);
    ++success_counter;
  }
#endif

  while(1)
  {
    //RE_GetTimeStamp();
     RE_Req_BatInfo();
     HAL_Delay(500);
     t_10s++;
    /* CPU in idle mode for 1 sec */

    HAL_UART_Receive_IT(&huart3_t,&uart_rcvd_byte,1);
    uint8_t lcd_on = 0;    
/*
// Used wihtout key status 
    RE_RefreshScreen();
    HAL_Delay(500);
*/
    
    if(key_status == 1)
    {
      
      if( animate == 1)
      {
        RE_lcd_start_animation();
        animate = 0;
      }
      RE_RefreshScreen();
    }
    
       /*odo*/
    if (save_odo == 1)
    {
      LcdDisplay.odo = ReadLogFile("ODO.TXT", 10);
      LcdDisplay.odo++;
      LogData.pfile_name = "ODO.TXT";
      sprintf(sd_buf,"%d",LcdDisplay.odo);
      OverWriteLog(LogData.pfile_name, sd_buf); 
      save_odo = 0;
    }
   if ( t_10s >= 20)
   { 
    /************************************battery1 log data*********************************/
        sprintf(LogData.file_name, "bat1.txt");
        LogData.pfile_name = LogData.file_name;
        sprintf(sd_buf, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", LogData.bat[0].soc, LogData.bat[0].volt,\
        LogData.bat[0].temp,LogData.bat[0].current, LogData.bat[0].remain_cap,LogData.bat[0].avail_energy,\
        LogData.bat[0].cycle_count,LogData.bat[0].soh, LogData.bat[0].full_cap);
        AppendLog(LogData.pfile_name, sd_buf);
        memset(sd_buf, 0, sizeof(sd_buf));
          
    /************************************battery2 log data*********************************/   
        sprintf(LogData.file_name, "bat2.txt");
        LogData.pfile_name = LogData.file_name;
        sprintf(sd_buf, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", LogData.bat[1].soc, LogData.bat[1].volt,\
        LogData.bat[1].temp,LogData.bat[1].current, LogData.bat[1].remain_cap,LogData.bat[1].avail_energy,\
        LogData.bat[1].cycle_count,LogData.bat[1].soh, LogData.bat[1].full_cap);
        AppendLog(LogData.pfile_name, sd_buf);        
        memset(sd_buf, 0, sizeof(sd_buf));
     
    /************************************battery3 log data*********************************/        
        sprintf(LogData.file_name, "bat3.txt");
        LogData.pfile_name = LogData.file_name;
        sprintf(sd_buf, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", LogData.bat[2].soc, LogData.bat[2].volt,\
        LogData.bat[2].temp,LogData.bat[2].current, LogData.bat[2].remain_cap,LogData.bat[2].avail_energy,\
        LogData.bat[2].cycle_count,LogData.bat[2].soh, LogData.bat[2].full_cap);
        AppendLog(LogData.pfile_name, sd_buf);
        memset(sd_buf, 0, sizeof(sd_buf));
 
    /************************************battery4 log data*********************************/        
        sprintf(LogData.file_name, "bat4.txt");
        LogData.pfile_name = LogData.file_name;        
        sprintf(sd_buf, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", LogData.bat[3].soc, LogData.bat[3].volt,\
        LogData.bat[3].temp,LogData.bat[3].current, LogData.bat[3].remain_cap,LogData.bat[3].avail_energy,\
        LogData.bat[3].cycle_count,LogData.bat[3].soh, LogData.bat[3].full_cap);
        AppendLog(LogData.pfile_name, sd_buf);          
        memset(sd_buf, 0, sizeof(sd_buf));
   
        t_10s = 0;
   }
   if(SPI_Tx_Flag == 1)
   {
      RE_SPI_Trasmit_Msg();
   }
  }
 return 0;
}
/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/