//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifdef ARDUINO
#include "egl_device_ili9341.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------

//============================================================================
// locals
//============================================================================
static const uint8_t s_tft_init_commands[] =
    {
        4, 0xEF, 0x03, 0x80, 0x02,
        4, 0xCF, 0x00, 0XC1, 0X30,
        5, 0xED, 0x64, 0x03, 0X12, 0X81,
        4, 0xE8, 0x85, 0x00, 0x78,
        6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
        2, 0xF7, 0x20,
        3, 0xEA, 0x00, 0x00,
        2, ILI9341_PWCTR1, 0x23,        // Power control
        2, ILI9341_PWCTR2, 0x10,        // Power control
        3, ILI9341_VMCTR1, 0x3e, 0x28,  // VCM control
        2, ILI9341_VMCTR2, 0x86,        // VCM control2
        2, ILI9341_MADCTL, 0x48,        // Memory Access Control
        2, ILI9341_PIXFMT, 0x55,
        3, ILI9341_FRMCTR1, 0x00, 0x18,
        4, ILI9341_DFUNCTR, 0x08, 0x82, 0x27,  // Display Function Control
        2, 0xF2, 0x00,                         // Gamma Function Disable
        2, ILI9341_GAMMASET, 0x01,             // Gamma curve selected
        16, ILI9341_GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
        0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,  // Set Gamma
        16, ILI9341_GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
        0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,  // Set Gamma
        3, 0xb1, 0x00, 0x10,                                   // FrameRate Control 119Hz
        0};
//----------------------------------------------------------------------------

//============================================================================
// graphics_device_ili9341
//============================================================================
graphics_device_ili9341* graphics_device_ili9341::s_active_dev = 0;
//----------------------------------------------------------------------------

graphics_device_ili9341::graphics_device_ili9341()
{
    EGL_ASSERT(!s_active_dev);
    s_active_dev = this;
    m_tile_rt0   = 0;
    set_spi(0);
#ifdef KINETISK
    m_pcs_data    = 0;
    m_pcs_command = 0;
#endif

#if EGL_PLATFORM_NUM_DMA > 0 && EGL_BUILDOP_DMA_TRANSFER == 1
    // init DMA variables
    m_dma_transfers = 0;
#endif
}
//----

graphics_device_ili9341::graphics_device_ili9341(uint8_t pin_cs_, uint8_t pin_dc_, uint8_t pin_sclk_, uint8_t pin_mosi_, uint8_t pin_miso_, uint8_t pin_reset_)
{
    EGL_ASSERT(!s_active_dev);
    s_active_dev = this;
    m_tile_rt0   = 0;
    set_spi(0);
#ifdef KINETISK
    m_pcs_data    = 0;
    m_pcs_command = 0;
#endif

#if EGL_PLATFORM_NUM_DMA > 0 && EGL_BUILDOP_DMA_TRANSFER == 1
    // init DMA variables
    m_dma_transfers = 0;
#endif

    // setup SPI pins
    init(pin_cs_, pin_dc_, pin_sclk_, pin_mosi_, pin_miso_, pin_reset_);
}
//----

graphics_device_ili9341::~graphics_device_ili9341()
{
    s_active_dev = 0;
}
//----

void graphics_device_ili9341::set_spi(uint8_t spi_, uint32_t spi_clock_)
{
    m_spi_clock = spi_clock_;
    m_spi       = 0;
    m_spi_chl   = 0;
    switch (spi_) {
#if EGL_PLATFORM_NUM_SPI >= 1
        // SPI0
        case 0: {
            m_spi     = &SPI;
            m_spi_chl = 0;
#ifdef EGL_PLATFORM_TEENSY4X
            m_spi_imxrt = &IMXRT_LPSPI4_S;
#elif defined(KINETISK)
            m_spi_kinetisk           = &KINETISK_SPI0;
            m_spi_kinetisk_fifo_wait = 3 << 12;
#endif
        } break;
#endif

#if EGL_PLATFORM_NUM_SPI >= 2
        // SPI1
        case 1: {
            m_spi     = &SPI1;
            m_spi_chl = 1;
#ifdef EGL_PLATFORM_TEENSY4X
            m_spi_imxrt = &IMXRT_LPSPI3_S;
#elif defined(KINETISK)
            m_spi_kinetisk           = &KINETISK_SPI1;
            m_spi_kinetisk_fifo_wait = 0 << 12;
#endif
        } break;
#endif

#if EGL_PLATFORM_NUM_SPI >= 3
        // SPI2
        case 2: {
            m_spi     = &SPI2;
            m_spi_chl = 2;
#ifdef EGL_PLATFORM_TEENSY4X
            m_spi_imxrt = &IMXRT_LPSPI1_S;
#elif defined(KINETISK)
            m_spi_kinetisk           = &KINETISK_SPI2;
            m_spi_kinetisk_fifo_wait = 0 << 12;
#endif
        } break;
#endif

        // unsupported
        default: EGL_ERRORF("Unsupported SPI %i.\r\n", spi_);
    }
}
//----

void graphics_device_ili9341::init(uint8_t pin_cs_, uint8_t pin_dc_, uint8_t pin_sclk_, uint8_t pin_mosi_, uint8_t pin_miso_, uint8_t pin_reset_)
{
#ifdef EGL_PLATFORM_TEENSY4X
    // setup CS and DC pins
    m_spi->begin();
    m_num_pending_rx = 0;
    m_cs_port        = portOutputRegister(pin_cs_);
    m_cs_mask        = digitalPinToBitMask(pin_cs_);
    pinMode(pin_cs_, OUTPUT);
    direct_write_hi(m_cs_port, m_cs_mask);
    m_dc_port = 0;
    m_dc_mask = 0;
    if (m_spi->pinIsChipSelect(pin_dc_))
        m_spi->setCS(pin_dc_);
    else {
        m_dc_port = portOutputRegister(pin_dc_);
        m_dc_mask = digitalPinToBitMask(pin_dc_);
        pinMode(pin_dc_, OUTPUT);
        direct_write_hi(m_dc_port, m_dc_mask);
    }
    update_tcr(LPSPI_TCR_PCS(1) | LPSPI_TCR_FRAMESZ(7), true);
#elif defined(KINETISK)
    m_spi->setMOSI(pin_mosi_);
    m_spi->setMISO(pin_miso_);
    m_spi->setSCK(pin_sclk_);
    m_spi->begin();

    // setup CS and DC pins
    m_pcs_command = 0;
    m_pcs_data    = 0;
    m_cs_port     = 0;
    m_cs_mask     = 0;
    if (m_spi->pinIsChipSelect(pin_cs_, pin_dc_)) {
        m_pcs_data    = m_spi->setCS(pin_cs_);
        m_pcs_command = m_pcs_data | m_spi->setCS(pin_dc_);
    } else if (m_spi->pinIsChipSelect(pin_dc_)) {
        m_pcs_command = m_spi->setCS(pin_dc_);
        pinMode(pin_cs_, OUTPUT);
        m_cs_port = portOutputRegister(digitalPinToPort(pin_cs_));
        m_cs_mask = digitalPinToBitMask(pin_cs_);
    }
#else
    m_cs_port = portOutputRegister(pin_cs_);
    m_cs_mask = digitalPinToBitMask(pin_cs_);
    pinMode(pin_cs_, OUTPUT);
    *m_cs_port |= m_cs_mask;
    m_dc_port = 0;
    m_dc_mask = 0;
    m_spi->setMOSI(pin_mosi_);
    m_spi->setMISO(pin_miso_);
    m_spi->setSCK(pin_sclk_);
    m_spi->begin();
    if (m_spi->pinIsChipSelect(pin_dc_))
        m_spi->setCS(pin_dc_);
    else {
        m_dc_port = portOutputRegister(pin_dc_);
        m_dc_mask = digitalPinToBitMask(pin_dc_);
        pinMode(pin_dc_, OUTPUT);
        *m_dc_port |= m_dc_mask;
    }
#endif

    // setup reset pin
    if (pin_reset_ < 0xff) {
        pinMode(pin_reset_, OUTPUT);
        digitalWrite(pin_reset_, HIGH);
        delay(5);
        digitalWrite(pin_reset_, LOW);
        delay(20);
        digitalWrite(pin_reset_, HIGH);
        delay(150);
    }

    // initialize TFT
    begin_spi_transition();
    const uint8_t* addr = s_tft_init_commands;
    while (1) {
        uint8_t count = *addr++;
        if (!count--)
            break;
        writecmd_cont(*addr++);
        update_tcr_data8();
        while (count--)
            writedata8_cont(*addr++);
    }
    writecmd_last(ILI9341_SLPOUT);
    end_spi_transition();

    // switch on the display
    delay(120);
    begin_spi_transition();
    writecmd_last(ILI9341_DISPON);
    end_spi_transition();

    // clear the screen to black
    begin_spi_transition();
    set_data_window(0, 0, fb_width - 1, fb_height - 1);
    writecmd_cont(ILI9341_RAMWR);
    update_tcr_data16();
    for (unsigned i = 0; i < fb_width * fb_height - 1; ++i)
        writedata16_cont(0);
    writedata16_last(0);
    EGL_LOG("ILI9341 graphics device initialized!\r\n");
}
//----

void graphics_device_ili9341::init_rasterizer(const rasterizer_cfg& rcfg_, const rasterizer_tiling_cfg& tcfg_, const rasterizer_vertex_cache_cfg& vccfg_)
{
    graphics_device<graphics_device_ili9341>::init(rcfg_, tcfg_, vccfg_);
    m_tile_rt0    = (fb_format_t*)rcfg_.rts[0];
    m_tile_width  = tcfg_.tile_width;
    m_tile_height = tcfg_.tile_height;
}
//----

void graphics_device_ili9341::init_dma(rasterizer_data_transfer* transfers_, uint8_t num_transfers_, fb_format_t* dma_buffer_, size_t dma_buffer_size_)
{
#if EGL_PLATFORM_NUM_DMA > 0 && EGL_BUILDOP_DMA_TRANSFER == 1
    // setup DMA buffers
    EGL_ASSERT(transfers_ && dma_buffer_);
    m_dma_transfers      = transfers_;
    m_dma_transfers_size = num_transfers_;
    m_dma_transfers_rpos = 0;
    m_dma_transfers_wpos = 0;
    m_dma_buffer         = dma_buffer_;
    m_dma_buffer_size    = dma_buffer_size_ / sizeof(*m_dma_buffer);
    m_dma_buffer_rpos    = 0;
    m_dma_buffer_wpos    = 0;

    // setup DMA settings
#ifdef EGL_PLATFORM_TEENSY4X
    static const SPIClass::SPI_Hardware_t* s_spis[] =
        {
            &SPIClass::spiclass_lpspi4_hardware,
            &SPIClass::spiclass_lpspi3_hardware,
            &SPIClass::spiclass_lpspi1_hardware,
        };
    const SPIClass::SPI_Hardware_t& spi_hw = *s_spis[m_spi_chl];
    m_dma_settings.destination(m_spi_imxrt->TDR);
#elif defined(KINETISK)
    static const SPIClass::SPI_Hardware_t* s_spis[] =
        {
            &SPIClass::spi0_hardware,
            &SPIClass::spi1_hardware,
            &SPIClass::spi2_hardware,
        };
    const SPIClass::SPI_Hardware_t& spi_hw = *s_spis[m_spi_chl];
    m_dma_settings.destination(m_spi_kinetisk->PUSHR);
#endif
    m_dma_settings.TCD->ATTR_DST = 1;
    m_dma_settings.replaceSettingsOnCompletion(m_dma_settings);
    m_dma_settings.interruptAtCompletion();
    m_dma_settings.disableOnCompletion();
    m_dma_chl.begin(true);
    m_dma_chl.triggerAtHardwareEvent(spi_hw.tx_dma_channel);
    m_dma_chl = m_dma_settings;
    m_dma_chl.attachInterrupt(dma_interrupt);
#else  // EGL_BUILDOP_DMA_TRANSFER
    EGL_ERROR("DMA support disabled\r\n");
#endif
}
//----------------------------------------------------------------------------

void graphics_device_ili9341::flush_dma()
{
#if EGL_PLATFORM_NUM_DMA > 0 && EGL_BUILDOP_DMA_TRANSFER == 1
    // wait for all DMA transfers to complete
    while (m_dma_transfers_rpos != m_dma_transfers_wpos)
        ;
#endif
}
//----------------------------------------------------------------------------

void graphics_device_ili9341::submit_tile(uint8_t tx_, uint8_t ty_, const vec2u16& reg_min_, const vec2u16& reg_end_, uint16_t thread_idx_)
{
    // access update pos, size and data
    uint16_t     x             = tx_ * m_tile_width + reg_min_.x;
    uint16_t     y             = ty_ * m_tile_height + reg_min_.y;
    uint16_t     update_width  = reg_end_.x - reg_min_.x;
    uint16_t     update_height = reg_end_.y - reg_min_.y;
    fb_format_t* data          = m_tile_rt0 + reg_min_.x + reg_min_.y * m_tile_width;

#if EGL_PLATFORM_NUM_DMA > 0 && EGL_BUILDOP_DMA_TRANSFER == 1
    // check for DMA transfer
    if (m_dma_transfers) {
        // add DMA transfers for the tile
        do {
            // get DMA transfer size
            size_t max_transfer_px = m_dma_buffer_size - m_dma_buffer_wpos;
            if (max_transfer_px < update_width * 2) {
                m_dma_buffer_wpos = 0;
                max_transfer_px   = m_dma_buffer_size;
            }
            uint16_t transfer_height = (uint16_t)min(size_t(update_height), max_transfer_px / update_width);
            size_t   transfer_px     = update_width * transfer_height;
            update_height -= transfer_height;

            // wait for space in transfer/data buffers for the DMA transfer (spinlock)
            uint8_t next_dma_transfers_wpos = m_dma_transfers_wpos + 1;
            if (next_dma_transfers_wpos == m_dma_transfers_size)
                next_dma_transfers_wpos = 0;
            while ((m_dma_buffer_rpos >= m_dma_buffer_wpos && m_dma_buffer_wpos + transfer_px > m_dma_buffer_rpos && m_dma_transfers_rpos != m_dma_transfers_wpos) || (m_dma_transfers_rpos == next_dma_transfers_wpos))
                ;

            // copy tile data to the DMA buffer
            fb_format_t* dma_buf = m_dma_buffer + m_dma_buffer_wpos;
            if (m_tile_shader) {
                m_tile_shader->copy_region(dma_buf, render_targets(), depth_target(), size_t(data - m_tile_rt0), x, y, update_width, transfer_height, m_tile_width);
                data += m_tile_width * transfer_height;
            } else {
                if (m_tile_width == update_width) {
                    // copy the full buffer at once
                    mem_copy(dma_buf, data, transfer_px * sizeof(*m_dma_buffer));
                    data += transfer_px;
                } else {
                    // copy the buffer scanline at the time
                    uint16_t     dma_scan_size = update_width * sizeof(*m_dma_buffer);
                    fb_format_t* dma_buf_end   = dma_buf + transfer_px;
                    do {
                        mem_copy(dma_buf, data, dma_scan_size);
                        dma_buf += update_width;
                        data += m_tile_width;
                    } while (dma_buf < dma_buf_end);
                }
            }

            // setup DMA transfer
            rasterizer_data_transfer& transfer = m_dma_transfers[m_dma_transfers_wpos];
            transfer.data_offset               = m_dma_buffer_wpos;
            transfer.x                         = x;
            transfer.y                         = y;
            y += transfer_height;
            transfer.width  = update_width;
            transfer.height = transfer_height;

            // queue transfer and try to start DMA transfer if none active
            noInterrupts();
            if (m_dma_transfers_wpos != m_dma_transfers_rpos)
                m_dma_transfers_wpos = next_dma_transfers_wpos;
            interrupts();
            if (m_dma_transfers_wpos != m_dma_transfers_rpos || start_dma_transfer(transfer)) {
                m_dma_transfers_wpos = next_dma_transfers_wpos;
                m_dma_buffer_wpos += transfer_px;
            }
        } while (update_height);
    } else
#endif  // EGL_BUILDOP_DMA_TRANSFER
    {
        // synchronously SPI transfer tile update region
        if (m_tile_shader)
            m_tile_shader->transfer_region(render_targets(), depth_target(), size_t(data - m_tile_rt0), x, y, update_width, update_height, m_tile_width);
        else {
            begin_spi_transition();
            set_data_window(x, y, x + update_width - 1, y + update_height - 1);
            writecmd_cont(ILI9341_RAMWR);
            update_tcr_data16();
            fb_format_t* data_end = data + m_tile_width * update_height;
            while (1) {
                fb_format_t *data_scan = data, *data_scan_end = data_scan + update_width - 1;
                while (data_scan < data_scan_end)
                    writedata16_cont((data_scan++)->v);
                data += m_tile_width;
                if (data == data_end) {
                    writedata16_last(data_scan->v);
                    break;
                } else
                    writedata16_cont(data_scan->v);
            }
            end_spi_transition();
        }
    }
}
//----

#if EGL_PLATFORM_NUM_DMA > 0 && EGL_BUILDOP_DMA_TRANSFER == 1
void graphics_device_ili9341::dma_interrupt()
{
    s_active_dev->dma_interrupt_impl();
}
//----

void graphics_device_ili9341::dma_interrupt_impl()
{
    // complete DMA transfer and advance DMA buffer read position
    m_dma_chl.clearInterrupt();
    rasterizer_data_transfer& transfer = m_dma_transfers[m_dma_transfers_rpos];
#ifdef EGL_PLATFORM_TEENSY4X
    while (m_spi_imxrt->FSR & 0x1f)
        ;
    while (m_spi_imxrt->SR & LPSPI_SR_MBF)
        ;
    m_spi_imxrt->FCR = LPSPI_FCR_TXWATER(15);
    m_spi_imxrt->DER = 0;
    m_spi_imxrt->CR  = LPSPI_CR_MEN | LPSPI_CR_RRF | LPSPI_CR_RTF;
    m_spi_imxrt->SR  = 0x3f00;
    size_t data_size = transfer.width * transfer.height;
#elif defined(KINETISK)
    wait_spi_fifo();
    size_t data_size = transfer.width * transfer.height - 1;
#endif
    writecmd_last(ILI9341_NOP);
    end_spi_transition();
    m_dma_buffer_rpos += data_size;

    // move to the next DMA transfer (loop until end-of-transfers or able to start DMA transfer)
    do {
        if (++m_dma_transfers_rpos == m_dma_transfers_size)
            m_dma_transfers_rpos = 0;
    } while (m_dma_transfers_rpos != m_dma_transfers_wpos && !start_dma_transfer(m_dma_transfers[m_dma_transfers_rpos]));
}
//----

bool graphics_device_ili9341::start_dma_transfer(const rasterizer_data_transfer& transfer_)
{
    // setup SPI transfer
    enum { min_dma_pixels = 16 };  // minimum pixels required to trigger DMA transfer
    EGL_STATIC_ASSERT(sizeof(*m_dma_buffer) == 2);
    begin_spi_transition();
    set_data_window(transfer_.x, transfer_.y, transfer_.x + transfer_.width - 1, transfer_.y + transfer_.height - 1);
    writecmd_cont(ILI9341_RAMWR);
    m_dma_buffer_rpos = transfer_.data_offset;
    size_t data_size  = transfer_.width * transfer_.height;

    // check for synchronous transfer
    if (data_size < min_dma_pixels) {
        // write pixels synchronously over SPI
        update_tcr_data16();
        const fb_format_t *buf = m_dma_buffer + m_dma_buffer_rpos, *buf_end = buf + data_size - 1;
        while (buf < buf_end)
            writedata16_cont((buf++)->v);
        writedata16_last(buf->v);
        end_spi_transition();
        m_dma_buffer_rpos += data_size;
        return false;
    }

    // setup DMA transfer
    const fb_format_t* dma_buf = m_dma_buffer + m_dma_buffer_rpos;
    data_size *= sizeof(*m_dma_buffer);
#ifdef EGL_PLATFORM_TEENSY4X
    m_spi_imxrt->FCR = 0;
    update_tcr(LPSPI_TCR_PCS(1) | LPSPI_TCR_FRAMESZ(15) | LPSPI_TCR_RXMSK, true);
    arm_dcache_flush((void*)dma_buf, data_size);
    m_dma_settings.sourceBuffer((const uint16_t*)dma_buf, data_size);
    m_spi_imxrt->DER = LPSPI_DER_TDDE;
    m_spi_imxrt->SR  = 0x3f00;
#elif defined(KINETISK)
    update_tcr_data16();
    writedata16_cont(m_dma_buffer[m_dma_buffer_rpos++].v);
    data_size -= sizeof(*m_dma_buffer);
    m_dma_settings.sourceBuffer((const uint16_t*)dma_buf, data_size);
    m_spi_kinetisk->RSER |= SPI_RSER_TFFF_DIRS | SPI_RSER_TFFF_RE;
    m_spi_kinetisk->MCR &= ~SPI_MCR_HALT;
#endif
    m_dma_chl = m_dma_settings;
    m_dma_chl.enable();
    return true;
}
#endif  // EGL_BUILDOP_DMA_TRANSFER
//----------------------------------------------------------------------------
#endif  // ARDUINO
