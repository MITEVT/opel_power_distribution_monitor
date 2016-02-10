#include "board.h"


// -------------------------------------------------------------
// Board ISRs

/**
 * SysTick Timer Interrupt Handler. Counts milliseconds since start
 */
void SysTick_Handler(void) {
	msTicks++;
}

/**
 * CCAN Interrupt Handler. Calls the isr() API located in the CCAN ROM
 */
void CAN_IRQHandler(void) {
	LPC_CCAN_API->isr();
}

/**
 * @brief	I2C Interrupt Handler
 * @return	None
 */
void I2C_IRQHandler(void) {
	if (Chip_I2C_IsMasterActive(I2C0)) {
		Chip_I2C_MasterStateHandler(I2C0);
	}
	else {
		Chip_I2C_SlaveStateHandler(I2C0);
	}
}
// -------------------------------------------------------------
// Public Functions and Members

const uint32_t OscRateIn = 0;

int8_t Board_SysTick_Init(void) {
	msTicks = 0;

	// Update the value of SystemCoreClock to the clock speed in hz
	SystemCoreClockUpdate();

	// Initialize SysTick Timer to fire interrupt at 1kHz
	return (SysTick_Config (SystemCoreClock / 1000));
}

void Board_LEDs_Init(void) {
	Chip_GPIO_Init(LPC_GPIO);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED0, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED1, true);
}

void Board_UART_Init(uint32_t baudrate) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, UART_RX_IOCON, (IOCON_FUNC1 | IOCON_MODE_INACT));	// Rx pin
	Chip_IOCON_PinMuxSet(LPC_IOCON, UART_TX_IOCON, (IOCON_FUNC1 | IOCON_MODE_INACT));	// Tx Pin
	
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, baudrate);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);
}

void Board_UART_Print(const char *str) {
	Chip_UART_SendBlocking(LPC_USART, str, strlen(str));
}

void Board_UART_Println(const char *str) {
	Board_UART_Print(str);
	Board_UART_Print("\r\n");
}

void Board_UART_PrintNum(const int num, uint8_t base, bool crlf) {
	static char str[32];
	itoa(num, str, base);
	Board_UART_Print(str);
	if (crlf) Board_UART_Print("\r\n");
}

void Board_UART_SendBlocking(const void *data, uint8_t num_bytes) {
	Chip_UART_SendBlocking(LPC_USART, data, num_bytes);
}

int8_t Board_UART_Read(void *data, uint8_t num_bytes) {
	return Chip_UART_Read(LPC_USART, data, num_bytes);
}

void CAN_baudrate_calculate(uint32_t baud_rate, uint32_t *can_api_timing_cfg)
{
	uint32_t pClk, div, quanta, segs, seg1, seg2, clk_per_bit, can_sjw;
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_CAN);
	pClk = Chip_Clock_GetMainClockRate();

	clk_per_bit = pClk / baud_rate;

	for (div = 0; div <= 15; div++) {
		for (quanta = 1; quanta <= 32; quanta++) {
			for (segs = 3; segs <= 17; segs++) {
				if (clk_per_bit == (segs * quanta * (div + 1))) {
					segs -= 3;
					seg1 = segs / 2;
					seg2 = segs - seg1;
					can_sjw = seg1 > 3 ? 3 : seg1;
					can_api_timing_cfg[0] = div;
					can_api_timing_cfg[1] =
						((quanta - 1) & 0x3F) | (can_sjw & 0x03) << 6 | (seg1 & 0x0F) << 8 | (seg2 & 0x07) << 12;
					return;
				}
			}
		}
	}
}

void Board_CAN_Init(uint32_t baudrate, void (*rx_callback)(uint8_t), void (*tx_callback)(uint8_t), void (*error_callback)(uint32_t)) {

	uint32_t can_api_timing_cfg[2];
	
	CCAN_CALLBACKS_T callbacks = {
		rx_callback,
		tx_callback,
		error_callback,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	};

	CAN_baudrate_calculate(baudrate, can_api_timing_cfg);

	/* Initialize the CAN controller */
	LPC_CCAN_API->init_can(&can_api_timing_cfg[0], TRUE);
	/* Configure the CAN callback functions */
	LPC_CCAN_API->config_calb(&callbacks);

	/* Enable the CAN Interrupt */
	NVIC_EnableIRQ(CAN_IRQn);
}

void Board_I2C_Init(void){
	Chip_SYSCTL_PeriphReset(RESET_I2C0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, (IOCON_FUNC1));
        Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, (IOCON_FUNC1));
	Chip_SYSCTL_DeassertPeriphReset(RESET_I2C0);
	Chip_I2C_Init(DEFAULT_I2C);
	Chip_I2C_SetClockRate(DEFAULT_I2C, SPEED_100KHZ);
	Chip_I2C_SetMasterEventHandler(DEFAULT_I2C, Chip_I2C_EventHandler);
	NVIC_EnableIRQ(I2C0_IRQn);	
}

void Board_PDM_Status_Update(PDM_STATUS_T *pdm_status, uint8_t *i2c_rx_buffer, bool cs) {
	int tmp;
	uint32_t battery_voltage_mVolts, battery_charge_percent;
	int32_t battery_current_mAmps;

	tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_SLAVE_ADDRESS, 0x08, i2c_rx_buffer, 2);
	battery_voltage_mVolts = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];
	battery_voltage_mVolts = 23600*battery_voltage_mVolts/0xFFFF;
	Board_UART_Print("Voltage Data (mV): ");
	Board_UART_PrintNum(battery_voltage_mVolts, 10, false);
	Board_UART_Print(" Length: ");
	Board_UART_PrintNum(tmp, 10, true);

	tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_SLAVE_ADDRESS, 0x02, i2c_rx_buffer, 2);
	battery_charge_percent = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];
	battery_charge_percent = 100*battery_charge_percent/0xFFFF;
	Board_UART_Print("Accumulated Charge Data (%): ");
	Board_UART_PrintNum(battery_charge_percent, 10, false);
	Board_UART_Print(" Length: ");
	Board_UART_PrintNum(tmp, 10, true);

	tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_SLAVE_ADDRESS, 0x0E, i2c_rx_buffer, 2);
	battery_current_mAmps = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];
	battery_current_mAmps = 60*(battery_current_mAmps-0x7FFF)*1000/(50*0x7FFF);
	
	Board_UART_Print("Current Data (mA): ");
	if(battery_current_mAmps < 0) {
		battery_current_mAmps = battery_current_mAmps * -1;
		Board_UART_Print("-");
	}
	Board_UART_PrintNum(battery_current_mAmps, 10, false);
	Board_UART_Print(" Length: ");
	Board_UART_PrintNum(tmp, 10, true); 
			

	if(cs) {
		pdm_status->critical_systems_bus_battery = battery_voltage_mVolts < LOW_VOLTAGE_THRESHHOLD;
		pdm_status->critical_systems_dc_dc = battery_current_mAmps >= 0;
		Board_UART_Println("Critical Systems Check");
		Board_UART_Println("");
	}
	else {
		pdm_status->low_voltage_bus_battery = battery_voltage_mVolts < LOW_VOLTAGE_THRESHHOLD;
		pdm_status->low_voltage_dc_dc = battery_current_mAmps >= 0;
		Board_UART_Println("Low Voltage Systems Check");
		Board_UART_Println("");
	}
}
