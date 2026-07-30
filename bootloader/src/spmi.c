#include "spmi.h"
#include "utils.h"
#include "fb.h"

static i16 channels[1 << 12]; // 8 + 4 бит

void pmic_arb_init() {
    u32 arb_ver = readu32(SPMI_CORE);

    fb_put_str("arb_ver: ");
    fb_put_hex(arb_ver, 0);
    fb_put_char('\n');

    for (u32 i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) {
        channels[i] = -1;
    }

    for (u32 i = 0; i < 256; i++) {
        u32 reg = readu32(PMIC_ARB_REG_CHNL(i));

        if (reg == 0)
            continue;

        u8 slave_id = (reg >> 16) & 0xf;
        u8 ppid_address = (reg >> 8) & 0xff;

        channels[CHNL_IDX(slave_id, ppid_address)] = i;

        fb_put_str("    ");
        fb_put_hex(i, 2);
        fb_put_str(": ");
        /* fb_put_hex(reg, 6); */
        fb_put_hex(slave_id, 1);
        fb_put_char(' ');
        fb_put_hex(ppid_address, 2);
        if (i % 6 == 5) {
            fb_put_char('\n');
        } else {
            fb_put_str("  ");
        }
    }
}

void pmic_arb_write_wdata4(u8 chnl, u8 reg, u8 d0, u8 d1, u8 d2, u8 d3) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    writeu32(PMIC_ARB_CHNLn_WDATA(chnl, reg), d0 | (d1 << 8) | (d2 << 16) | (d3 << 24));
#else
    writeu32(PMIC_ARB_CHNLn_WDATA(chnl, reg), d3 | (d2 << 8) | (d1 << 16) | (d0 << 24));
#endif
}

void pmic_arb_write_wdata(u8 chnl, u8 reg, u32 data) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#error(src/spmi.c: pmic_arb_write_wdata: convert from big endian)
#else
    writeu32(PMIC_ARB_CHNLn_WDATA(chnl, reg), data);
#endif
}

void pmic_arb_read_wdata(u8 chnl, u8 reg, u8 count, u8* data) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    u32 val = readu32(PMIC_ARB_CHNLn_RDATA(chnl, reg));
    for (u32 i = 0; i < count; i++) {
        data[i] = (val >> (count - i) * 8) & 0xff;
    }
#else
    u32 val = readu32(PMIC_ARB_CHNLn_RDATA(chnl, reg));
    for (u32 i = 0; i < count; i++) {
        data[i] = (val >> i * 8) & 0xff;
    }
#endif
}

i32 pmic_arb_read(u8 sid, u16 addr, u8 len, u8* data) {
    u8 ppid = (addr >> 8) & 0xff;
    u8 offset = addr & 0xff;

    i32 apid = channels[CHNL_IDX(sid, ppid)];

    if (apid == -1)
        return -1;

    u32 cmd = (PMIC_ARB_OP_EXT_READL << PMIC_ARB_CMD_OPCODE_SHIFT) |
              (0 << PMIC_ARB_CMD_PRIORITY_SHIFT) | // TODO
              (offset << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT) |
              ((len - 1) << PMIC_ARB_CMD_BYTE_CNT_SHIFT);

    writeu32(PMIC_ARB_CHNLn_CMD(apid), cmd);

    volatile u32 val;
    volatile u32 timeout = 1000000;
    do {
        val = readu32(PMIC_ARB_CHNLn_STATUS(apid));
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
    u8 ppid = (addr >> 8) & 0xff;
    u8 offset = addr & 0xff;

    i32 apid = channels[CHNL_IDX(sid, ppid)];

    if (apid == -1)
        return -1;

    u32 cmd = (PMIC_ARB_OP_EXT_WRITEL << PMIC_ARB_CMD_OPCODE_SHIFT) |
              (0 << PMIC_ARB_CMD_PRIORITY_SHIFT) | // TODO
              (offset << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT) |
              ((len - 1) << PMIC_ARB_CMD_BYTE_CNT_SHIFT);

    writeu32(PMIC_ARB_CHNLn_CMD(apid), cmd);

    volatile u32 val;
    volatile u32 timeout = 1000000;
    do {
        val = readu32(PMIC_ARB_CHNLn_STATUS(apid));
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
             /* (slaveid << PMIC_ARB_CMD_SLAVE_ID_SHIFT) | */
             /* (addr << PMIC_ARB_CMD_ADDR_SHIFT) | */
             (offset << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT) |
             ((size - 1) << PMIC_ARB_CMD_BYTE_CNT_SHIFT) // idk why -1
    );

    u32 status;
    u64 timeout = 1000000;
    while (!(status = readu32(PMIC_ARB_CHNLn_STATUS(channel_id))) && (timeout-- > 0));

    // TODO: error check
}
