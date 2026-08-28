#include "devices/spmi.h"

static i16 channels[1 << 12]; // 8 + 4 бит

void pmic_arb_init() {
    /* u32 arb_ver = *SPMI_CORE; */

    /* printf("arb_ver: %x\n", arb_ver); */

    for (u32 i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) {
        channels[i] = -1;
    }

    for (u32 i = 0; i < 256; i++) {
        u32 reg = *PMIC_ARB_REG_CHNL(i);

        if (reg == 0)
            continue;

        u8 slave_id     = (reg >> 16) & 0xf;
        u8 ppid_address = (reg >> 8) & 0xff;

        channels[CHNL_IDX(slave_id, ppid_address)] = i;

        /* fb_put_str("    "); */
        /* fb_put_hex(i, 2); */
        /* fb_put_str(": "); */
        /* /\* fb_put_hex(reg, 6); *\/ */
        /* fb_put_hex(slave_id, 1); */
        /* fb_put_char(' '); */
        /* fb_put_hex(ppid_address, 2); */
        /* if (i % 6 == 5) { */
        /*     fb_put_char('\n'); */
        /* } else { */
        /*     fb_put_str("  "); */
        /* } */
    }
}

void pmic_arb_write_wdata4(u8 chnl, u8 reg, u8 d0, u8 d1, u8 d2, u8 d3) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    *PMIC_ARB_CHNLn_WDATA(chnl, reg) =
             d0 | (d1 << 8) | (d2 << 16) | (d3 << 24));
#else
    *PMIC_ARB_CHNLn_WDATA(chnl, reg) = d3 | (d2 << 8) | (d1 << 16) | (d0 << 24);
#endif
}

void pmic_arb_write_wdata(u8 chnl, u8 reg, u32 data) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #error (src/spmi.c: pmic_arb_write_wdata: convert from big endian)
#else
    *PMIC_ARB_CHNLn_WDATA(chnl, reg) = data;
#endif
}

void pmic_arb_read_wdata(u8 chnl, u8 reg, u8 count, u8* data) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    u32 val = *PMIC_ARB_CHNLn_RDATA(chnl, reg);
    for (u32 i = 0; i < count; i++) {
        data[i] = (val >> (count - i) * 8) & 0xff;
    }
#else
    u32 val = *PMIC_ARB_CHNLn_RDATA(chnl, reg);
    for (u32 i = 0; i < count; i++) {
        data[i] = (val >> i * 8) & 0xff;
    }
#endif
}

i32 pmic_arb_read(u8 sid, u16 addr, u8 len, u8* data) {
    u8 ppid   = (addr >> 8) & 0xff;
    u8 offset = addr & 0xff;

    i32 apid = channels[CHNL_IDX(sid, ppid)];

    if (apid == -1)
        return -1;

    u32 cmd = (PMIC_ARB_OP_EXT_READL << PMIC_ARB_CMD_OPCODE_SHIFT)
            | (0 << PMIC_ARB_CMD_PRIORITY_SHIFT) | // TODO
              (offset << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT)
            | ((len - 1) << PMIC_ARB_CMD_BYTE_CNT_SHIFT);

    *PMIC_ARB_CHNLn_CMD(apid) = cmd;

    volatile u32 val;
    volatile u32 timeout = 1000000;
    do {
        val = *PMIC_ARB_CHNLn_STATUS(apid);
        timeout--;
    } while (timeout > 0 && !(val & PMIC_ARB_STATUS_DONE));

    if (timeout == 0)
        return -2;

    u32 error = val ^ PMIC_ARB_STATUS_DONE;

    if (error) {
        return error;
    }

    for (u32 i = 0; i < len; i += 4) {
        pmic_arb_read_wdata(apid, i / 4, len - i > 4 ? 4 : len - i, data + i);
    }

    return 0;
}

i32 pmic_arb_write(u8 sid, u16 addr, u8 len, u8* data) {
    u8 ppid   = (addr >> 8) & 0xff;
    u8 offset = addr & 0xff;

    i32 apid = channels[CHNL_IDX(sid, ppid)];

    if (apid == -1)
        return -1;

    u32 cmd = (PMIC_ARB_OP_EXT_WRITEL << PMIC_ARB_CMD_OPCODE_SHIFT)
            | (0 << PMIC_ARB_CMD_PRIORITY_SHIFT) | // TODO
              (offset << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT)
            | ((len - 1) << PMIC_ARB_CMD_BYTE_CNT_SHIFT);

    *PMIC_ARB_CHNLn_CMD(apid) = cmd;

    volatile u32 val;
    volatile u32 timeout = 1000000;
    do {
        val = *PMIC_ARB_CHNLn_STATUS(apid);
        timeout--;
    } while (timeout > 0 && !(val & PMIC_ARB_STATUS_DONE));

    if (timeout == 0)
        return -2;

    u32 error = val ^ PMIC_ARB_STATUS_DONE;

    if (error) {
        return error;
    }

    return 0;
}
