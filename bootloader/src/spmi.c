#include "spmi.h"
#include "utils.h"

void pmic_reg_write(u8 slaveid, u8 addr, u8 offset, u8 val) {
    pmic_write_cmd(slaveid, addr, offset, &val, 1);
}

void pmic_write_cmd(u8 slaveid, u8 addr, u8 offset, u8 *buf, u8 size) {
    u8 channel_id = 0; // TODO: pick channel

    // TODO: disable irq

    if (size <= 4) {
        writeu32(SPMI_CHNLS + channel_id * 0x8000 + PMIC_ARB_WDATA0,
                 *(u32*)(buf + 0) & (1 << (size * 8)) - 1);
    } else {
        writeu32(SPMI_CHNLS + channel_id * 0x8000 + PMIC_ARB_WDATA0, *(u32*)(buf + 0));
        writeu32(SPMI_CHNLS + channel_id * 0x8000 + PMIC_ARB_WDATA1,
                 *(u32*)(buf + 4) & (1 << ((size - 4) * 8)) - 1);
    }

    writeu32(
             SPMI_CHNLS + channel_id * 0x8000 + PMIC_ARB_CMD,
             (PMIC_ARB_OP_EXT_WRITEL << PMIC_ARB_CMD_OPCODE_SHIFT) |
             (slaveid << PMIC_ARB_CMD_SLAVE_ID_SHIFT) |
             (addr << PMIC_ARB_CMD_ADDR_SHIFT) |
             (offset << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT) |
             ((size - 1) << PMIC_ARB_CMD_BYTE_CNT_SHIFT) // idk why -1
    );

    u32 status;
    u64 timeout = 1000000;
    while (!(status = readu32(SPMI_CHNLS + channel_id * 0x8000 + PMIC_ARB_STATUS)) && (timeout-- > 0));

    // TODO: error check
}
