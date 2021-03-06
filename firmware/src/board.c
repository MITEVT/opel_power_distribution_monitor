#include "board.h"
#include "pdm_i2c_bus.h"
#include "can_spec_constants.h"

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
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED2, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED3, true);
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

void Board_I2C_Reset(uint8_t reset_val, uint8_t *i2c_tx_buffer) {
	//Open I2C Channel 0
	i2c_tx_buffer[0] = I2C_MUX_CHANNEL_0;
	Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);
	
	//Write reset_val to gas gauge 0 control register
	i2c_tx_buffer[0] = I2C_GG_CTRL_REG;
	i2c_tx_buffer[1] = reset_val;
	Chip_I2C_MasterSend(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, i2c_tx_buffer, 2);

	//Open I2C Channel 1
	i2c_tx_buffer[0] = I2C_MUX_CHANNEL_1;
	Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);
	
	//Write reset_val to gas gauge 1 control register
	i2c_tx_buffer[0] = I2C_GG_CTRL_REG;
	i2c_tx_buffer[1] = reset_val;
	Chip_I2C_MasterSend(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, i2c_tx_buffer, 2);
}

void Board_CAN_SendHeartbeat(PDM_STATUS_T *pdm_status, CCAN_MSG_OBJ_T *msg_obj, bool comm_err) {
	/* CAN message code */
	msg_obj->msgobj = 0;
	msg_obj->mode_id = PDM_PACKET__id;
	msg_obj->dlc = 1;

	bool total_error = pdm_status->mux_on && pdm_status->cs_on && pdm_status->lv_on || comm_err;

	uint32_t data = (pdm_status->low_voltage_bus_battery & 0x1) |				//Transfer data from PDM struct 
			((pdm_status->low_voltage_dc_dc & 0x1) << 1) |				//to 5 bit binary
			((pdm_status->critical_systems_bus_battery & 0x1) << 2) |
			((pdm_status->critical_systems_dc_dc & 0x1) << 3) |
			(pdm_status->pdm_on << 4) |
			(total_error << 5);
	uint32_t error = 0x1 & (data > 1);

	msg_obj->data[0] = ((error << 6) | data);
	Board_UART_PrintNum(msg_obj->data[0], 2, true);
	LPC_CCAN_API->can_transmit(msg_obj);							//Send the CAN message
}

void Board_PDM_Status_Debug(PDM_STATUS_T *pdm_status, bool mux_i2c, bool cs_i2c, bool lv_i2c) {
	if(mux_i2c && !(pdm_status->mux_on)) {
		pdm_status->mux_on = true;								//Update I2C Multiplexer status
		Board_LED_On(LED1);
	}
	else if(!mux_i2c && pdm_status->mux_on) {
		pdm_status->mux_on = false;
		Board_LED_Off(LED1);
	}

	if(cs_i2c && !(pdm_status->cs_on)) {		//Attempt to update Critical Systems PDM struct	
		pdm_status->cs_on = true;							//Update Critical Systems gas gauge status
		Board_LED_On(LED2);
	}
	else if(!cs_i2c && pdm_status->cs_on) {
		pdm_status->cs_on = false;
		Board_LED_Off(LED2);
	}

	if(lv_i2c && !(pdm_status->lv_on)) {			//Attempt to update Low Voltage PDM struct
		pdm_status->lv_on = true;							//Update Low Voltage gas gauge status
		Board_LED_On(LED3);
	}
	else if(!lv_i2c && pdm_status->lv_on){
		pdm_status->lv_on = false;
		Board_LED_Off(LED3);
	}	
}

bool Board_PDM_Status_Update(PDM_STATUS_T *pdm_status, uint8_t *i2c_rx_buffer, bool cs) {
	int tmp;
	uint32_t battery_voltage_mVolts, battery_charge_percent;
	int32_t battery_current_mAmps;

	tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, I2C_GG_VOLT_REG, i2c_rx_buffer, 2);		//Read raw voltage data from gas gauge
	battery_voltage_mVolts = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];			//Convert two bytes to hex
	battery_voltage_mVolts = 23600*battery_voltage_mVolts/0xFFFF;							//Convert to milliVolts
	Board_UART_Print("Voltage Data (mV): ");
	Board_UART_PrintNum(battery_voltage_mVolts, 10, false);
	Board_UART_Print(" Length: ");
	Board_UART_PrintNum(tmp, 10, true);

	tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, I2C_GG_CHRG_REG, i2c_rx_buffer, 2);		//Read raw charge data from gas gauge
	battery_charge_percent = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];			//Convert two bytes to hex
	battery_charge_percent = 100*battery_charge_percent/0xFFFF;							//Convert to percentage charged
	Board_UART_Print("Accumulated Charge Data (%): ");
	Board_UART_PrintNum(battery_charge_percent, 10, false);
	Board_UART_Print(" Length: ");
	Board_UART_PrintNum(tmp, 10, true);

	tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, I2C_GG_CURR_REG, i2c_rx_buffer, 2);		//Read raw current data from gas gauge
	battery_current_mAmps = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];				//Convert two bytes to hex
	battery_current_mAmps = 60*(battery_current_mAmps-0x7FFF)*1000/(50*0x7FFF);					//Convert to milliAmps
	Board_UART_Print("Current Data (mA): ");
	if(battery_current_mAmps < 0) {
		battery_current_mAmps = battery_current_mAmps * -1;
		Board_UART_Print("-");
	}
	Board_UART_PrintNum(battery_current_mAmps, 10, false);
	Board_UART_Print(" Length: ");
	Board_UART_PrintNum(tmp, 10, true); 
			
	//Update the correct system struct
	if(cs) { 
		//Update the critical systems PDM struct
		pdm_status->critical_systems_bus_battery = battery_voltage_mVolts < LOW_VOLTAGE_THRESHOLD;
		pdm_status->critical_systems_dc_dc = battery_current_mAmps >= 0;
		Board_UART_Println("Critical Systems Check");
		Board_UART_Println("");
	}
	else { 
		//Update the low voltage systems PDM struct
		pdm_status->low_voltage_bus_battery = battery_voltage_mVolts < LOW_VOLTAGE_THRESHOLD;
		pdm_status->low_voltage_dc_dc = battery_current_mAmps >= 0;
		Board_UART_Println("Low Voltage Systems Check");
		Board_UART_Println("");
	}

	return tmp == 2;
}
