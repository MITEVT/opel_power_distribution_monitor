#include "board.h"

// -------------------------------------------------------------
// Macro Definitions

#define CCAN_BAUD_RATE 500000 					// Desired CAN Baud Rate
#define UART_BAUD_RATE 57600 					// Desired UART Baud Rate

#define BUFFER_SIZE 8

// -------------------------------------------------------------
// Static Variable Declaration

extern volatile uint32_t msTicks;

static uint32_t lastPrint;

static CCAN_MSG_OBJ_T msg_obj; 					// Message Object data structure for manipulating CAN messages
static RINGBUFF_T can_rx_buffer;				// Ring Buffer for storing received CAN messages
static CCAN_MSG_OBJ_T _rx_buffer[BUFFER_SIZE]; 	// Underlying array used in ring buffer

static char str[100];							// Used for composing UART messages
static uint8_t uart_rx_buffer[BUFFER_SIZE]; 	// UART received message buffer

static bool can_error_flag;
static uint32_t can_error_info;

static uint8_t i2c_tx_buffer[20];
static uint8_t i2c_rx_buffer[20];

// -------------------------------------------------------------
// Helper Functions

/**
 * Delay the processor for a given number of milliseconds
 * @param ms Number of milliseconds to delay
 */
void _delay(uint32_t ms) {
	uint32_t curTicks = msTicks;
	while ((msTicks - curTicks) < ms);
}

// -------------------------------------------------------------
// CAN Driver Callback Functions

/*	CAN receive callback */
/*	Function is executed by the Callback handler after
    a CAN message has been received */
void CAN_rx(uint8_t msg_obj_num) {
	// LED_On();
	/* Determine which CAN message has been received */
	msg_obj.msgobj = msg_obj_num;
	/* Now load up the msg_obj structure with the CAN message */
	LPC_CCAN_API->can_receive(&msg_obj);
	if (msg_obj_num == 1) {
		RingBuffer_Insert(&can_rx_buffer, &msg_obj);
	}
}

/*	CAN transmit callback */
/*	Function is executed by the Callback handler after
    a CAN message has been transmitted */
void CAN_tx(uint8_t msg_obj_num) {
	msg_obj_num = msg_obj_num;
}

/*	CAN error callback */
/*	Function is executed by the Callback handler after
    an error has occurred on the CAN bus */
void CAN_error(uint32_t error_info) {
	can_error_info = error_info;
	can_error_flag = true;
}




// -------------------------------------------------------------
// Interrupt Service Routines


// -------------------------------------------------------------
// Main Program Loop



int main(void)
{

	//---------------
	// Initialize SysTick Timer to generate millisecond count
	if (Board_SysTick_Init()) {
		// Unrecoverable Error. Hang.
		while(1);
	}

	//---------------
	// Initialize GPIO and LED as output
	Board_LEDs_Init();
	Board_LED_On(LED0);

	//Initialize pins for pullup resistors
	Board_LV_Check_Init();

	//Initialize I2C
	Board_I2C_Init();

	//---------------
	// Initialize UART Communication
	Board_UART_Init(UART_BAUD_RATE);
	Board_UART_Println("Started up");

	//---------------
	// Initialize CAN  and CAN Ring Buffer

	RingBuffer_Init(&can_rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), BUFFER_SIZE);
	RingBuffer_Flush(&can_rx_buffer);

	Board_CAN_Init(CCAN_BAUD_RATE, CAN_rx, CAN_tx, CAN_error);

	// For your convenience.
	// typedef struct CCAN_MSG_OBJ {
	// 	uint32_t  mode_id;
	// 	uint32_t  mask;
	// 	uint8_t   data[8];
	// 	uint8_t   dlc;
	// 	uint8_t   msgobj;
	// } CCAN_MSG_OBJ_T;

	/* [Tutorial] How do filters work?

		Incoming ID & Mask == Mode_ID for msgobj to accept message

		Incoming ID : 0xabc
		Mask: 		  0xF0F &
		            -----------
		              0xa0c

		mode_id == 0xa0c for msgobj to accept message

	*/

	msg_obj.msgobj = 1;
	msg_obj.mode_id = 0x000;
	msg_obj.mask = 0x000;
	LPC_CCAN_API->config_rxmsgobj(&msg_obj);

	/* [Tutorial] How do I send a CAN Message?

		There are 32 Message Objects in the CAN Peripherals Message RAM.
		We need to pick one that isn't setup for receiving messages and use it to send.

		For this exmaple we'll pick 31

		msg_obj.msgobj = 31;
		msg_obj.mode_id = 0x600; 		// CAN ID of Message to Send
		msg_obj.dlc = 8; 				// Byte length of CAN Message
		msg_obj.data[0] = 0xAA; 		// Fill your bytes here
		msg_obj.data[1] = ..;
		..
		msg_obj.data[7] = 0xBB:

		Now its time to send
		LPC_CCAN_API->can_transmit(&msg_obj);

	*/

	can_error_flag = false;
	can_error_info = 0;
	
	lastPrint = msTicks;	

	PDM_STATUS_T pdm_status;
	
	int tmp;
	uint32_t cs_battery_voltage_mVolts, cs_battery_charge_percent;
	int32_t cs_battery_current_mAmps;
	i2c_tx_buffer[0] = 0x01;
	i2c_tx_buffer[1] = 0xF8;
	tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_SLAVE_ADDRESS, i2c_tx_buffer, 2);
	Board_UART_Print("Hello World: ");
	Board_UART_PrintNum(tmp, 10, true);

	while (1) {
		//Board_LV_Status_Update(&pdm_status);

		if(msTicks - lastPrint > 2500){					// 10 times per second
			lastPrint = msTicks;					// Store the current time, to allow the process to be done in another 1/5 second
			/*
			msg_obj.msgobj = 0;
			msg_obj.mode_id = 0x550;
			msg_obj.dlc = 1;
			msg_obj.data[0] = pdm_status.low_voltage_status * 0x01 | low_voltage_bus_battery * 0x02 | low_voltage_dc_dc * 0x04 | critical_systems_status * 0x08 | critical_systems_bus_battery * 0x10 | critical_systems_dc_dc * 0x20;

			LPC_CCAN_API->can_transmit(&msg_obj); */

			tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_SLAVE_ADDRESS, 0x08, i2c_rx_buffer, 2);
			cs_battery_voltage_mVolts = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];
			cs_battery_voltage_mVolts = 23600*cs_battery_voltage_mVolts/0xFFFF;
			Board_UART_Print("Voltage Data (mV): ");
			Board_UART_PrintNum(cs_battery_voltage_mVolts, 10, false);
			Board_UART_Print(" Length: ");
			Board_UART_PrintNum(tmp, 10, true);

			tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_SLAVE_ADDRESS, 0x02, i2c_rx_buffer, 2);
			cs_battery_charge_percent = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];
			cs_battery_charge_percent = 100*cs_battery_charge_percent/0xFFFF;
			Board_UART_Print("Accumulated Charge Data (C): ");
			Board_UART_PrintNum(cs_battery_charge_percent, 10, false);
			Board_UART_Print(" Length: ");
			Board_UART_PrintNum(tmp, 10, true);

			tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_SLAVE_ADDRESS, 0x0E, i2c_rx_buffer, 2);
			cs_battery_current_mAmps = ((uint16_t)i2c_rx_buffer[0] << 8) | (uint16_t)i2c_rx_buffer[1];
			cs_battery_current_mAmps = 60*(cs_battery_current_mAmps-0x7FFF)*1000/(50*0x7FFF);
			
			Board_UART_Print("Current Data (mA): ");
			if(cs_battery_current_mAmps < 0) {
				cs_battery_current_mAmps = cs_battery_current_mAmps * -1;
				Board_UART_Print("-");
			}
			Board_UART_PrintNum(cs_battery_current_mAmps, 10, false);
			Board_UART_Print(" Length: ");
			Board_UART_PrintNum(tmp, 10, true); 
			
			Board_UART_Println("");
		}
	}
}
