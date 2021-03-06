#ifndef DRIVERS_LSM_LSM303C_TOOLS_H_
#define DRIVERS_LSM_LSM303C_TOOLS_H_

#include <stdint.h>

extern I2C_HandleTypeDef* i2c_lsm303c;

int32_t lsm303c_platform_init(void);

uint32_t lsm303c_get_m_data_mG(float* magn);


#endif /* DRIVERS_LSM_LSM303C_TOOLS_H_ */
