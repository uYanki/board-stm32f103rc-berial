
#include "driver_ina219_read_test.h"
static ina219_handle_t gs_handle;        // ina219 handle
uint8_t ina219_read_test(ina219_address_t addr_pin, double r, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint16_t calibration;
    ina219_info_t info;
    /* link interface function */
    DRIVER_INA219_LINK_INIT(&gs_handle, ina219_handle_t);
    DRIVER_INA219_LINK_IIC_INIT(&gs_handle, ina219_interface_iic_init);
    DRIVER_INA219_LINK_IIC_DEINIT(&gs_handle, ina219_interface_iic_deinit);
    DRIVER_INA219_LINK_IIC_READ(&gs_handle, ina219_interface_iic_read);
    DRIVER_INA219_LINK_IIC_WRITE(&gs_handle, ina219_interface_iic_write);
    DRIVER_INA219_LINK_DELAY_MS(&gs_handle, ina219_interface_delay_ms);
    DRIVER_INA219_LINK_DEBUG_PRINT(&gs_handle, ina219_interface_debug_print);
    /* get information */
    res = ina219_info(&info);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ina219_interface_debug_print("ina219: chip is %s.\n", info.chip_name);
        ina219_interface_debug_print("ina219: manufacturer is %s.\n", info.manufacturer_name);
        ina219_interface_debug_print("ina219: interface is %s.\n", info.interface);
        ina219_interface_debug_print("ina219: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ina219_interface_debug_print("ina219: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ina219_interface_debug_print("ina219: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ina219_interface_debug_print("ina219: max current is %0.2fmA.\n", info.max_current_ma);
        ina219_interface_debug_print("ina219: max temperature is %0.1fC.\n", info.temperature_max);
        ina219_interface_debug_print("ina219: min temperature is %0.1fC.\n", info.temperature_min);
    }
    /* start read test */
    ina219_interface_debug_print("ina219: start read test.\n");
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
    /* set bus voltage range 32V */
    res = ina219_set_bus_voltage_range(&gs_handle, INA219_BUS_VOLTAGE_RANGE_32V);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage range failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set bus voltage adc mode 12 bit 1 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set shunt voltage adc mode 12 bit 1 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* set shunt bus voltage continuous */
    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 40 mV.\n");
    /* set pga 40 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_40_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_set_calibration(&gs_handle, calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* read shunt voltage */
        res = ina219_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read shunt voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina219_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read bus voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina219_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read current failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina219_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read power failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: power is %0.3fmW.\n", m);
        
        ina219_interface_delay_ms(1000);
    }
    ina219_interface_debug_print("ina219: set pga 80 mV.\n");
    /* set pga 80 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_80_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_set_calibration(&gs_handle, calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* read shunt voltage */
        res = ina219_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read shunt voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina219_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read bus voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina219_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read current failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina219_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read power failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: power is %0.3fmW.\n", m);
        
        ina219_interface_delay_ms(1000);
    }
    ina219_interface_debug_print("ina219: set pga 160 mV.\n");
    /* set pga 160 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_160_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_set_calibration(&gs_handle, calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* read shunt voltage */
        res = ina219_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read shunt voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina219_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read bus voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina219_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read current failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina219_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read power failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: power is %0.3fmW.\n", m);
        
        ina219_interface_delay_ms(1000);
    }
    ina219_interface_debug_print("ina219: set pga 320 mV.\n");
    /* set pga 320 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_320_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    res = ina219_set_calibration(&gs_handle, calibration);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* read shunt voltage */
        res = ina219_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read shunt voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina219_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read bus voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina219_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read current failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina219_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read power failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: power is %0.3fmW.\n", m);
        
        ina219_interface_delay_ms(1000);
    }
    ina219_interface_debug_print("ina219: set triggered mode.\n");
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* set shunt bus voltage triggered */
        res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: set mode failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read shunt voltage */
        res = ina219_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read shunt voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina219_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read bus voltage failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina219_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read current failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina219_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina219_interface_debug_print("ina219: read power failed.\n");
            (void)ina219_deinit(&gs_handle);
            
            return 1;
        }
        ina219_interface_debug_print("ina219: power is %0.3fmW.\n", m);
        
        ina219_interface_delay_ms(1000);
    }
    /* finish read test */
    (void)ina219_deinit(&gs_handle);
    ina219_interface_debug_print("ina219: finish read test.\n");
    return 0;
}
