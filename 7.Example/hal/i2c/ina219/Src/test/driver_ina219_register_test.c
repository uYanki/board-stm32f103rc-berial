
#include "driver_ina219_register_test.h"
#include <stdlib.h>
static ina219_handle_t gs_handle;        // ina219 handle
uint8_t ina219_register_test(ina219_address_t addr_pin)
{
    uint8_t res;
    double r, r_check;
    uint16_t data, data_check;
    ina219_info_t info;
    ina219_address_t addr;
    ina219_bus_voltage_range_t range;
    ina219_pga_t pga;
    ina219_adc_mode_t mode;
    ina219_mode_t chip_mode;
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
    /* start register test */
    ina219_interface_debug_print("ina219: start register test.\n");
    /* ina219_set_addr_pin/ina219_get_addr_pin test */
    ina219_interface_debug_print("ina219: ina219_set_addr_pin/ina219_get_addr_pin test.\n");
    /* set address 0 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_0);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 0.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_0 ? "ok" : "error");
    /* set address 1 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_1);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 1.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_1 ? "ok" : "error");
    /* set address 2 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_2);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 2.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_2 ? "ok" : "error");
    /* set address 3 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_3);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 3.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_3 ? "ok" : "error");
    /* set address 4 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_4);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 4.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_4 ? "ok" : "error");
    /* set address 5 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_5);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 5.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_5 ? "ok" : "error");
    /* set address 6 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_6);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 6.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_6 ? "ok" : "error");
    /* set address 7 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_7);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 7.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_7 ? "ok" : "error");
    /* set address 8 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_8);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 8.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_8 ? "ok" : "error");
    /* set address 9 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_9);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 9.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_9 ? "ok" : "error");
    /* set address 10 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_A);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 10.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_A ? "ok" : "error");
    /* set address 11 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_B);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 11.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_B ? "ok" : "error");
    /* set address 12 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_C);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 12.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_C ? "ok" : "error");
    /* set address 13 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_D);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 13.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_D ? "ok" : "error");
    /* set address 14 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_E);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 14.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_E ? "ok" : "error");
    /* set address 15 */
    res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_F);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set addr pin 15.\n");
    res = ina219_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get addr pin failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check addr pin %s.\n", addr == INA219_ADDRESS_F ? "ok" : "error");
    /* ina219_set_resistance/ina219_get_resistance test */
    ina219_interface_debug_print("ina219: ina219_set_resistance/ina219_get_resistance test.\n");
    /* generate the r */
    r = (double)(rand() % 100) / 1000.0;
    res = ina219_set_resistance(&gs_handle, r);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set resistance failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: set resistance %f.\n", r);
    res = ina219_get_resistance(&gs_handle, (double *)&r_check);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get resistance failed.\n");
       
        return 1;
    }
    ina219_interface_debug_print("ina219: check resistance %f.\n", r_check);
    /* set addr pin */
    res = ina219_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set addr pin failed.\n");
       
        return 1;
    }
    /* init */
    res = ina219_init(&gs_handle);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: init failed.\n");
       
        return 1;
    }
    /* ina219_set_bus_voltage_range/ina219_get_bus_voltage_range test */
    ina219_interface_debug_print("ina219: ina219_set_bus_voltage_range/ina219_get_bus_voltage_range test.\n");
    /* set bus voltage range 16V */
    res = ina219_set_bus_voltage_range(&gs_handle, INA219_BUS_VOLTAGE_RANGE_16V);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage range failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage range 16V.\n");
    res = ina219_get_bus_voltage_range(&gs_handle, &range);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage range failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check bus voltage range %s.\n", range == INA219_BUS_VOLTAGE_RANGE_16V ? "ok" : "error");
    /* set bus voltage range 32V */
    res = ina219_set_bus_voltage_range(&gs_handle, INA219_BUS_VOLTAGE_RANGE_32V);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage range failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage range 32V.\n");
    res = ina219_get_bus_voltage_range(&gs_handle, &range);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage range failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check bus voltage range %s.\n", range == INA219_BUS_VOLTAGE_RANGE_32V ? "ok" : "error");
    /* ina219_set_pga/ina219_get_pga test */
    ina219_interface_debug_print("ina219: ina219_set_pga/ina219_get_pga test.\n");
    /* set 40 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_40_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 40 mV.\n");
    res = ina219_get_pga(&gs_handle, &pga);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check pga %s.\n", pga == INA219_PGA_40_MV ? "ok" : "error");
    /* set 80 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_80_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 80 mV.\n");
    res = ina219_get_pga(&gs_handle, &pga);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check pga %s.\n", pga == INA219_PGA_80_MV ? "ok" : "error");
    /* set 160 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_160_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 160 mV.\n");
    res = ina219_get_pga(&gs_handle, &pga);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check pga %s.\n", pga == INA219_PGA_160_MV ? "ok" : "error");
    /* set 320 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_320_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 320 mV.\n");
    res = ina219_get_pga(&gs_handle, &pga);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check pga %s.\n", pga == INA219_PGA_320_MV ? "ok" : "error");
    /* ina219_set_bus_voltage_adc_mode/ina219_get_bus_voltage_adc_mode test */
    ina219_interface_debug_print("ina219: ina219_set_bus_voltage_adc_mode/ina219_get_bus_voltage_adc_mode test.\n");
    /* 9 bit 1 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_9_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 9 bit 1 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_9_BIT_1_SAMPLES ? "ok" : "error");
    /* 10 bit 1 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_10_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 10 bit 1 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_10_BIT_1_SAMPLES ? "ok" : "error");
    /* 11 bit 1 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_11_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 11 bit 1 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_11_BIT_1_SAMPLES ? "ok" : "error");
    /* 12 bit 1 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 1 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_1_SAMPLES ? "ok" : "error");
    /* 12 bit 2 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_2_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 2 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_2_SAMPLES ? "ok" : "error");
    /* 12 bit 4 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_4_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 4 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_4_SAMPLES ? "ok" : "error");
    /* 12 bit 8 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_8_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 8 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_8_SAMPLES ? "ok" : "error");
    /* 12 bit 16 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_16_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 16 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_16_SAMPLES ? "ok" : "error");
    /* 12 bit 32 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_32_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 32 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_32_SAMPLES ? "ok" : "error");
    /* 12 bit 64 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_64_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 64 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_64_SAMPLES ? "ok" : "error");
    /* 12 bit 128 sample */
    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_128_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage adc mode 12 bit 128 sample.\n");
    res = ina219_get_bus_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get bus voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_128_SAMPLES ? "ok" : "error");
    /* ina219_set_shunt_voltage_adc_mode/ina219_get_shunt_voltage_adc_mode test */
    ina219_interface_debug_print("ina219: ina219_set_shunt_voltage_adc_mode/ina219_get_shunt_voltage_adc_mode test.\n");
    /* 9 bit 1 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_9_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 9 bit 1 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_9_BIT_1_SAMPLES ? "ok" : "error");
    /* 10 bit 1 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_10_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 10 bit 1 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_10_BIT_1_SAMPLES ? "ok" : "error");
    /* 11 bit 1 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_11_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 11 bit 1 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_11_BIT_1_SAMPLES ? "ok" : "error");
    /* 12 bit 1 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_1_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 1 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_1_SAMPLES ? "ok" : "error");
    /* 12 bit 2 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_2_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 2 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_2_SAMPLES ? "ok" : "error");
    /* 12 bit 4 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_4_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 4 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_4_SAMPLES ? "ok" : "error");
    /* 12 bit 8 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_8_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 8 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_8_SAMPLES ? "ok" : "error");
    /* 12 bit 16 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_16_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 16 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_16_SAMPLES ? "ok" : "error");
    /* 12 bit 32 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_32_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 32 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_32_SAMPLES ? "ok" : "error");
    /* 12 bit 64 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_64_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 64 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_64_SAMPLES ? "ok" : "error");
    /* 12 bit 128 sample */
    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_128_SAMPLES);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage adc mode 12 bit 128 sample.\n");
    res = ina219_get_shunt_voltage_adc_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get shunt voltage adc mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check adc mode %s.\n", mode == INA219_ADC_MODE_12_BIT_128_SAMPLES ? "ok" : "error");
    /* ina219_set_mode/ina219_get_mode test */
    ina219_interface_debug_print("ina219: ina219_set_mode/ina219_get_mode test.\n");
    /* set power down */
    res = ina219_set_mode(&gs_handle, INA219_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set power down mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_POWER_DOWN ? "ok" : "error");
    /* set adc off */
    res = ina219_set_mode(&gs_handle, INA219_MODE_ADC_OFF);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set adc off mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_ADC_OFF ? "ok" : "error");
    /* set shunt voltage continuous */
    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage continuous mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_SHUNT_VOLTAGE_CONTINUOUS ? "ok" : "error");
    /* set bus voltage continuous */
    res = ina219_set_mode(&gs_handle, INA219_MODE_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage continuous mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_BUS_VOLTAGE_CONTINUOUS ? "ok" : "error");
    /* set shunt bus voltage continuous */
    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt bus voltage continuous mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS ? "ok" : "error");
    /* set shunt voltage triggered */
    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt voltage triggered mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_SHUNT_VOLTAGE_TRIGGERED ? "ok" : "error");
    /* set bus voltage triggered */
    res = ina219_set_mode(&gs_handle, INA219_MODE_BUS_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set bus voltage triggered mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_BUS_VOLTAGE_TRIGGERED ? "ok" : "error");
    /* set shunt bus voltage triggered */
    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set shunt bus voltage triggered mode.\n");
    res = ina219_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get mode failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check chip mode %s.\n", chip_mode == INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED ? "ok" : "error");
    /* ina219_calculate_calibration test */
    ina219_interface_debug_print("ina219: ina219_calculate_calibration test.\n");
    /* set pga 40 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_40_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 40 mV.\n");
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&data_check);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: calculate calibration %d.\n", data_check);
    /* set pga 80 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_80_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 80 mV.\n");
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&data_check);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: calculate calibration %d.\n", data_check);
    /* set pga 160 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_160_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 160 mV.\n");
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&data_check);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: calculate calibration %d.\n", data_check);
    /* set pga 320 mV */
    res = ina219_set_pga(&gs_handle, INA219_PGA_320_MV);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set pga failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set pga 320 mV.\n");
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&data_check);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: calculate calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: calculate calibration %d.\n", data_check);
    /* ina219_set_calibration/ina219_get_calibration test */
    ina219_interface_debug_print("ina219: ina219_set_calibration/ina219_get_calibration test.\n");
    data = rand() % 65536;
    res = ina219_set_calibration(&gs_handle, data);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: set calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: set calibration %d.\n", data);
    res = ina219_get_calibration(&gs_handle, (uint16_t *)&data_check);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: get calibration failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    ina219_interface_debug_print("ina219: check calibration %s.\n", data == data_check ? "ok" : "error");
    /* ina219_soft_reset test */
    ina219_interface_debug_print("ina219: ina219_soft_reset test.\n");
    res = ina219_soft_reset(&gs_handle);
    if (res != 0)
    {
        ina219_interface_debug_print("ina219: soft reset failed.\n");
        (void)ina219_deinit(&gs_handle);
        
        return 1;
    }
    /* finish register test */
    (void)ina219_deinit(&gs_handle);
    ina219_interface_debug_print("ina219: finish register test.\n");
    return 0;
}
