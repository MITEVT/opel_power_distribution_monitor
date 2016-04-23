//Multiplexer Channel Values
#define I2C_MUX_CHANNEL_0 0x01
#define I2C_MUX_CHANNEL_1 0x02


//I2C Control Register Values
#define I2C_GG_SLEEP 0x39
#define I2C_GG_DEFAULT 0x3C
#define I2C_GG_CONTINUOUS 0xF8

//Register Locations
#define I2C_GG_CTRL_REG 0x01
#define I2C_GG_CURR_REG 0x0E
#define I2C_GG_VOLT_REG 0x08
#define I2C_GG_CHRG_REG 0x02

//I2C Protocol Values
#define DEFAULT_I2C I2C0
#define SPEED_100KHZ 100000
#define I2C_GG_SLAVE_ADDRESS 0x64
#define I2C_MUX_SLAVE_ADDRESS 0x70
