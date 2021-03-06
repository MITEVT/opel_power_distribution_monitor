#include "board.h"
#include "pdm_i2c_bus.h"
#include "can_spec_constants.h"

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
	Board_LED_On(LED1);
	Board_LED_On(LED2);
	Board_LED_On(LED3);

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

	//Set mask to only accept messages from Driver Interface
	msg_obj.msgobj = 1;
	msg_obj.mode_id = DI_PACKET__id;
	msg_obj.mask = 0x555;
	LPC_CCAN_API->config_rxmsgobj(&msg_obj);

	can_error_flag = false;
	can_error_info = 0;
	
	lastPrint = msTicks;	

	PDM_STATUS_T pdm_status;
	
	int tmp;
	bool lv_i2c = true, cs_i2c = true, mux_i2c = true;

	//Board_I2C_Reset(I2C_GG_CONTINUOUS, i2c_tx_buffer);

	//Open I2C Channel 0
	i2c_tx_buffer[0] = I2C_MUX_CHANNEL_0;
	tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);
	Board_UART_Print("Opened Channel 0: ");
	Board_UART_PrintNum(tmp, 10, true);

	//Set Gas Gauge 0 to continuous data collection
	i2c_tx_buffer[0] = I2C_GG_CTRL_REG;
	i2c_tx_buffer[1] = I2C_GG_CONTINUOUS;
	tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, i2c_tx_buffer, 2);
	Board_UART_Print("Set I2C0 to continuous data collection: ");
	Board_UART_PrintNum(tmp, 10, true);

	//Open I2C Channel 1
	i2c_tx_buffer[0] = I2C_MUX_CHANNEL_1;
	tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);
	Board_UART_Print("Opened Channel 1: ");
	Board_UART_PrintNum(tmp, 10, true);

	//Set Gas Gauge 1 to continuous data collection
	i2c_tx_buffer[0] = I2C_GG_CTRL_REG;
	i2c_tx_buffer[1] = I2C_GG_CONTINUOUS;
	tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, i2c_tx_buffer, 2);
	Board_UART_Print("Set I2C1 to continuous data collection: ");
	Board_UART_PrintNum(tmp, 10, true);

	

	while (1) {
		
		//Set PDM status based on CAN messages from Driver Interface
		if (!RingBuffer_IsEmpty(&can_rx_buffer)) {					
			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&can_rx_buffer, &temp_msg);
			//Test for DI OFF or SHUTDOWN IMPENDING message
			if((temp_msg.data[3] << 8 | temp_msg.data[2]) == ____DI_PACKET__DRIVE_STATUS__SHUTDOWN_IMPENDING ||	
					(temp_msg.data[3] << 8 | temp_msg.data[2]) == ____DI_PACKET__DRIVE_STATUS__OFF) {	
				if(pdm_status.pdm_on) {							
					pdm_status.pdm_on = false;
					Board_LED_Off(LED0);
					Board_I2C_Reset(I2C_GG_SLEEP, i2c_tx_buffer);
				}
			}
			else {
				if(!(pdm_status.pdm_on)) {
					pdm_status.pdm_on = true;
					Board_LED_On(LED0);
					Board_I2C_Reset(I2C_GG_CONTINUOUS, i2c_tx_buffer);
				}
			}
		}
		

		//Reset gas gauge 0 if it has been diconnected and then reconnected to power
		i2c_tx_buffer[0] = I2C_MUX_CHANNEL_0;
		tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);	//Open I2C Channel 0
		tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, I2C_GG_CTRL_REG, i2c_rx_buffer, 1);	
		Board_UART_PrintNum(i2c_rx_buffer[0],16,true);
		if((uint16_t)i2c_rx_buffer[0] == I2C_GG_DEFAULT) {					//Test for default values in control register
			if(pdm_status.pdm_on) {
				Board_I2C_Reset(I2C_GG_CONTINUOUS, i2c_tx_buffer);
			}
			else {
				Board_I2C_Reset(I2C_GG_SLEEP, i2c_tx_buffer);
			}
			//Send a heartbeat with a com error
			Board_CAN_SendHeartbeat(&pdm_status, &msg_obj, true);
		}

			
		//Reset gas gauge 1 if it has been diconnected and then reconnected to power	
		i2c_tx_buffer[0] = I2C_MUX_CHANNEL_1;
		tmp = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);	//Open 12C Channel 1
		tmp = Chip_I2C_MasterCmdRead(DEFAULT_I2C, I2C_GG_SLAVE_ADDRESS, I2C_MUX_CHANNEL_0, i2c_rx_buffer, 1);
		Board_UART_PrintNum(i2c_rx_buffer[0],16,true);
		if((uint16_t)i2c_rx_buffer[0] == I2C_GG_DEFAULT) {					//Test for default values in control register
			if(pdm_status.pdm_on) {
				Board_I2C_Reset(I2C_GG_CONTINUOUS, i2c_tx_buffer);
			}
			else {
				Board_I2C_Reset(I2C_GG_SLEEP, i2c_tx_buffer);
			}
			//Send a heartbeat with a com error
			Board_CAN_SendHeartbeat(&pdm_status, &msg_obj, true);
		}


		/* Update PDM and Debug LED code */

		//Attempt to open I2C Channel 0
		i2c_tx_buffer[0] = I2C_MUX_CHANNEL_0;
		mux_i2c = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);	
		//Attempt to update Critical Systems PDM Struct
		cs_i2c = Board_PDM_Status_Update(&pdm_status, i2c_rx_buffer, true);			
		//Attempt to open I2C Channel 1
		i2c_tx_buffer[0] = I2C_MUX_CHANNEL_1;
		mux_i2c = Chip_I2C_MasterSend(DEFAULT_I2C, I2C_MUX_SLAVE_ADDRESS, i2c_tx_buffer, 1);	
		//Attempt to update Low Voltage PDM struct
		lv_i2c = Board_PDM_Status_Update(&pdm_status, i2c_rx_buffer, false);			
		//Run debug logic and update state
		Board_PDM_Status_Debug(&pdm_status, mux_i2c, cs_i2c, lv_i2c);

		if(msTicks - lastPrint > FREQ_THRESHOLD){				// 10 times per second
			lastPrint = msTicks;				// Store the current time, to allow the process to be done in another 1/10 seconds
			
			Board_CAN_SendHeartbeat(&pdm_status, &msg_obj, false);
		}
	}
}
