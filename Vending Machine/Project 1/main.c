#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

enum state {s0, s500, s1000, sKeluar, sKembalian} state_Vending;
int C500, C1000 = 0;
void CekSensor(void);
void permenKeluar(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch (state_Vending)
	  {
	  	 case s0:
	  	 {
	  		 CekSensor();
	  			  if(C500 == 1)
	  				  {
	  					  state_Vending=s500;
	  				  }
	  			  else if(C1000 == 1)
	  				  {
	  					  state_Vending=s1000;
	  				  }
	  			  break;
	  	 }
	  	 case s500:
	  	 {
	  		CekSensor();
		  		 if(C500 == 2)
  			  		 {
  			  			 state_Vending=s1000;
  			  		 }
		  		 else if((C1000 == 1)&&(C500 == 1))
  			  		 {
  			  			 state_Vending=sKeluar;
  			  		 }
  			  	break;
	  	 }
	  	 case s1000:
	  	 {
	  		 CekSensor();
	  		 	 if((C1000 == 1)&&(C500 == 1))
	  		 	 {
	  		 		 state_Vending=sKeluar;
	  		 	 }
	  		 	 else if(C1000 == 2)
	  		 	 {
	  		 		 state_Vending=sKembalian;
	  		 	 }
	  		 	else if((C500 == 2)&&(C1000 == 2))
	  		 	 {
	  		 		 state_Vending=sKembalian;
	  		 	 }
	  		 	 else if(C500 == 3)
	  		 	 {
	  		 		 state_Vending=sKeluar;
	  		 	 }
	  		 	 break;
	  	 }
	  	 case sKeluar:
	  	 {
	  		 permenKeluar();
	  		 break;
	  	 }
	  	 case sKembalian:
	  	 {
	  		HAL_GPIO_WritePin(Kembalian_GPIO_Port, Kembalian_Pin, GPIO_PIN_SET);
	  		HAL_Delay(200);
	  		HAL_GPIO_WritePin(Kembalian_GPIO_Port, Kembalian_Pin, GPIO_PIN_RESET);
	  		state_Vending = sKeluar;
	  	 }

	  }
  }
  /* USER CODE END 3 */
}

void CekSensor(void){
	if (HAL_GPIO_ReadPin(InK500_GPIO_Port, InK500_Pin)==GPIO_PIN_SET){
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(InK500_GPIO_Port, InK500_Pin)==GPIO_PIN_RESET){
			++C500;
			HAL_GPIO_WritePin(K500_GPIO_Port, K500_Pin, GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(K500_GPIO_Port, K500_Pin, GPIO_PIN_RESET);
		}
	}
	if (HAL_GPIO_ReadPin(InK1000_GPIO_Port, InK1000_Pin)==GPIO_PIN_SET){
			HAL_Delay(50);
			if (HAL_GPIO_ReadPin(InK1000_GPIO_Port, InK1000_Pin)==GPIO_PIN_RESET){
			++C1000;
			HAL_GPIO_WritePin(K1000_GPIO_Port, K1000_Pin, GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(K1000_GPIO_Port, K1000_Pin, GPIO_PIN_RESET);
			}
		}
}

void permenKeluar(void){
	HAL_GPIO_WritePin(Keluar_GPIO_Port, Keluar_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(Keluar_GPIO_Port, Keluar_Pin, GPIO_PIN_RESET);
	state_Vending=s0;
	C500 = 0;
	C1000 = 0;
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(K500_GPIO_Port, K500_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, K1000_Pin|Kembalian_Pin|Keluar_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : InK500_Pin InK1000_Pin */
  GPIO_InitStruct.Pin = InK500_Pin|InK1000_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : K500_Pin */
  GPIO_InitStruct.Pin = K500_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K500_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : K1000_Pin Kembalian_Pin Keluar_Pin */
  GPIO_InitStruct.Pin = K1000_Pin|Kembalian_Pin|Keluar_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
