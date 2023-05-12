
#include "driver_ina219_shot.h"
static ina219_handle_t gs_handle;        // ina219 handle
uint8_t ina219_shot_init(ina219_address_t addr_pin, double r)
{
    uint8_t res;
    uint16_t calibration;
    /* link interface function */
    DRIVER_INA219_LINK_INIT(&gs_handle, ina219_handle_t);
    DRIVER_INA219_LINK_IIC_INIT(&gs_handle, ina219_interface_iic_init);
    DRIVER_INA219_LINK_IIC_DEINIT(&gs_handle, ina219_interface_iic_deinit);
    DRIVER_INA219_LINK_IIC_READ(&gs_handle, ina219_interface_iic_read);
    DRIVER_INA219_LINK_IIC_WRITE(&gs_handle, ina219_interface_iic_write);
    DRIVER_INA219_LINK_DELAY_MS(&gs_handle, ina219_interface_delay_ms);
    DRIVER_INA219_LINK_DEBUG_PRINT(&gs_handle, ina219_interface_debug_print);
    /* set addr pin */
    res = ina219_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    /* set the r */
    res = ina219_set_resistance(&gs_handle, r);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set resistance failed.\n");
       
        return 1;
    }
    /* init */
    res = ina219_init(&gs_handle);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: init failed.\n");
       
        return 1;
    }
    /* set bus voltage range */
    res = ina219_set_bus_voltage_range(&gs_handle, INA219_SHOT_DEFAULT_BUS_VOLTAGE_RANGE);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage range failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set bus voltage adc mode */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_SHOT_DEFAULT_BUS_VOLTAGE_ADC_MODE);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set shunt voltage adc mode */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_SHOT_DEFAULT_SHUNT_VOLTAGE_ADC_MODE);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set pga */
    res = ina219_set_pga(&gs_handle, INA219_SHOT_DEFAULT_PGA);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* calculate calibration */
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set calibration */
    res = ina219_set_calibration(&gs_handle, calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    return 0;
}
uint8_t ina219_shot_read(float *mV, float *mA, float *mW)
{
    uint8_t res;
    int16_t s_raw;
    uint16_t u_raw;
    /* set shunt bus voltage triggered */
    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        return 1;
    }
    /* read bus voltage */
    res = ina219_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, mV);
    if (res != 0)
    {
        return 1;
    }
    /* read current */
    res = ina219_read_current(&gs_handle, (int16_t *)&s_raw, mA);
    if (res != 0)
    {
        return 1;
    }
    /* read power */
    res = ina219_read_power(&gs_handle, (uint16_t *)&u_raw, mW);
    if (res != 0)
    {
        return 1;
    }
    return 0;
}
uint8_t ina219_shot_deinit(void)
{
    uint8_t res;
    res = ina219_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    return 0;
}
