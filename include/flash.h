// SPI STM flash commands:
// stolen from BFloader

#define SPM_RDID              0x9f    // Read ID
#define SPM_WREN              0x06    // Write enable
#define SPM_WRDI              0x04    // Write disable
#define SPM_RDSR              0x05    // Read status register
#define SPM_WRSR              0x01    // Write status register
#define SPM_READ              0x03    // Read data bytes
#define SPM_PP                0x02    // Page program
#define SPM_DP                0xb9    // Deep power down
#define SPM_RES               0xab    // Release from deep power down
                                      // and read signature
#define SPM_FLASH_SE          0xd8    // Sector erase
#define SPM_FLASH_BE          0xc7    // Bulk erase
#define SPM_FLASH_FAST_READ   0x0B    // Read data bytes fast

#define SPM_SR_SRWD           0x80    // SR write protection (HW)

