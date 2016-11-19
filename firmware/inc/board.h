#ifndef __BOARD_H_
#define __BOARD_H_

#include "chip.h"
#include "util.h"
#include <string.h>

// -------------------------------------------------------------
// Global Variables

extern const uint32_t OscRateIn; 
volatile uint32_t msTicks; 						// Running count of milliseconds since start

// -------------------------------------------------------------
// Configuration Macros

typedef struct {
	bool low_voltage_bus_battery;
	bool low_voltage_dc_dc;
	bool critical_systems_bus_battery;
	bool critical_systems_dc_dc;
	bool pdm_on;
	bool mux_on;
	bool lv_on;
	bool cs_on;
} PDM_STATUS_T;

// -------------------------------------------------------------
// Pin Descriptions

#define LED0_PORT 2
#define LED0_PIN 5

#define LED1_PORT 0
#define LED1_PIN 6

#define LED2_PORT 0
#define LED2_PIN 7

#define LED3_PORT 2
#define LED3_PIN 9

#define UART_RX_PORT 1
#define UART_RX_PIN 6
#define UART_RX_IOCON IOCON_PIO1_6

#define UART_TX_PORT 1
#define UART_TX_PIN 7
#define UART_TX_IOCON IOCON_PIO1_7

//PDM
#define LOW_VOLTAGE_THRESHOLD 10000
#define ONE_TICK 1000
#define FREQ_OFFSET 1


// -------------------------------------------------------------
// Computed Macros
#define FREQ_THRESHOLD ONE_TICK/(PDM_PACKET__freq + FREQ_OFFSET)

#define LED0 LED0_PORT, LED0_PIN
#define LED1 LED1_PORT, LED1_PIN
#define LED2 LED2_PORT, LED2_PIN
#define LED3 LED3_PORT, LED3_PIN

#define UART_RX UART_RX_PORT, UART_RX_PIN
#define UART_TX UART_TX_PORT, UART_TX_PIN

#define Board_LED_On(led) {Chip_GPIO_SetPinState(LPC_GPIO, led, true);}
#define Board_LED_Off(led) {Chip_GPIO_SetPinState(LPC_GPIO, led, false);}
 
// -------------------------------------------------------------
// Board Level Function Prototypes

int8_t Board_SysTick_Init(void);

void Board_LEDs_Init(void);

void Board_UART_Init(uint32_t baudrate);

void Board_I2C_Init(void);

/**
 * Reset both gas gauges by writing a given byte to both control registers.
 *
 * @param reset_val the byte with which to overwrite the control registers
 * @param i2c_tx_buffer a pointer to the array to write data through I2C
 */
void Board_I2C_Reset(uint8_t reset_val, uint8_t *i2c_tx_buffer);

/**
 * Send a PDM heartbeat over CAN as defined in the CAN spec.
 *
 * @param *pdm_status a pointer to the current status of the PDM
 * @param *msg_obj a pointer to the CAN message object
 */
void Board_CAN_SendHeartbeat(PDM_STATUS_T *pdm_status, CCAN_MSG_OBJ_T *msg_obj, bool comm_err);

void Board_PDM_Status_Debug(PDM_STATUS_T *pdm_status, bool mux_i2c, bool cs_i2c, bool lv_i2c);

/**
 * Update the struct containing the current status of the PDM.
 * 
 * @param pdm_status the struct to be updated
 * @param i2c_rx_buffer the array to store I2C readings
 * @param cs the tested system; true for critical systems, false for low voltage systems
 * @return true if the PDM successfully communicated through I2C, false otherwise
 */
bool Board_PDM_Status_Update(PDM_STATUS_T *pdm_status, uint8_t *i2c_rx_buffer, bool cs);

/**
 * Transmit the given string through the UART peripheral (blocking)
 * 
 * @param str pointer to string to transmit
 * @note	This function will send or place all bytes into the transmit
 *			FIFO. This function will block until the last bytes are in the FIFO.
 */
void Board_UART_Print(const char *str);

/**
 * Transmit a string through the UART peripheral and append a newline and a linefeed character (blocking)
 * 
 * @param str pointer to string to transmit
 * @note	This function will send or place all bytes into the transmit
 *			FIFO. This function will block until the last bytes are in the FIFO.
 */
void Board_UART_Println(const char *str);

/**
 * Transmit a string containing a number through the UART peripheral (blocking)
 * 
 * @param num number to print
 * @param base number base
 * @param crlf append carraige return and line feed
 */
void Board_UART_PrintNum(const int num, uint8_t base, bool crlf);

/**
 * Transmit a byte array through the UART peripheral (blocking)
 * 
 * @param	data		: Pointer to data to transmit
 * @param	num_bytes	: Number of bytes to transmit
 * @note	This function will send or place all bytes into the transmit
 *			FIFO. This function will block until the last bytes are in the FIFO.
 */
void Board_UART_SendBlocking(const void *data, uint8_t num_bytes);

/**
 * Read data through the UART peripheral (non-blocking)
 * 
 * @param	data		: Pointer to bytes array to fill
 * @param	num_bytes	: Size of the passed data array
 * @return	The actual number of bytes read
 * @note	This function reads data from the receive FIFO until either
 *			all the data has been read or the passed buffer is completely full.
 *			This function will not block. This function ignores errors.
 */
int8_t Board_UART_Read(void *data, uint8_t num_bytes);

void Board_CAN_Init(uint32_t baudrate, void (*rx_callback)(uint8_t), void (*tx_callback)(uint8_t), void (*error_callback)(uint32_t));


#endif
