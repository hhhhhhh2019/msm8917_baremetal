#ifndef SPMI_H_
#define SPMI_H_

#include "stdint.h"

#define SPMI_CNFG   0x200a000
#define SPMI_CORE   0x200f000
#define SPMI_CHNLS  0x2400000
#define SPMI_OBSRVR 0x2c00000
#define SPMI_INTR   0x3800000

#define PMIC_ARB_CMD    0x00
#define PMIC_ARB_CONFIG 0x04
#define PMIC_ARB_STATUS 0x08
#define PMIC_ARB_WDATA0 0x10
#define PMIC_ARB_WDATA1 0x14
#define PMIC_ARB_RDATA0 0x18
#define PMIC_ARB_RDATA1 0x1C

#define PMIC_ARB_CMD_OPCODE_SHIFT      27
#define PMIC_ARB_CMD_PRIORITY_SHIFT    26
#define PMIC_ARB_CMD_SLAVE_ID_SHIFT    20
#define PMIC_ARB_CMD_ADDR_SHIFT        12
#define PMIC_ARB_CMD_ADDR_OFFSET_SHIFT 4
#define PMIC_ARB_CMD_BYTE_CNT_SHIFT    0

#define PMIC_ARB_OP_EXT_WRITEL   0
#define PMIC_ARB_OP_EXT_READL    1
#define PMIC_ARB_OP_EXT_WRITE    2
#define PMIC_ARB_OP_RESET        3
#define PMIC_ARB_OP_SLEEP        4
#define PMIC_ARB_OP_SHUTDOWN     5
#define PMIC_ARB_OP_WAKEUP       6
#define PMIC_ARB_OP_AUTHENTICATE 7
#define PMIC_ARB_OP_MSTR_READ    8
#define PMIC_ARB_OP_MSTR_WRITE   9
#define PMIC_ARB_OP_EXT_READ     13
#define PMIC_ARB_OP_WRITE        14
#define PMIC_ARB_OP_READ         15
#define PMIC_ARB_OP_ZERO_WRITE   16

void pmic_reg_write(u8 slaveid, u8 addr, u8 offset, u8 val);
void pmic_write_cmd(u8 slaveid, u8 addr, u8 offset, u8* buf, u8 size);

#endif // SPMI_H_
