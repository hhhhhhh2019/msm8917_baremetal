#ifndef SPMI_H_
#define SPMI_H_

#include "stdint.h"

#define SPMI_CORE   0x200f000
#define SPMI_CHNLS  0x2400000
#define SPMI_OBSRVR 0x2c00000
#define SPMI_INTR   0x3800000
#define SPMI_CNFG   0x200a000

#define CHNL_IDX(slave, ppid) ((slave << 8) | ppid)

#define PMIC_ARB_REG_CHNL(n)              (SPMI_CORE + 0x800 + (n) * 4)

#define PMIC_ARB_CHNLn(n)                 (SPMI_CHNLS  + (n) * 0x8000)
#define PMIC_ARB_CHNLn_CMD(n)             (PMIC_ARB_CHNLn(n) + 0x0)
#define PMIC_ARB_CHNLn_CONFIG(n)          (PMIC_ARB_CHNLn(n) + 0x4)
#define PMIC_ARB_CHNLn_STATUS(n)          (PMIC_ARB_CHNLn(n) + 0x8)
#define PMIC_ARB_CHNLn_WDATA(n, x)        (PMIC_ARB_CHNLn(n) + 0x10 + (x) * 4)
#define PMIC_ARB_CHNLn_RDATA(n, x)        (PMIC_ARB_CHNLn(n) + 0x18 + (x) * 4)

#define SPMI_IRQ 0xbe

/* #define PMIC_ARB_CMD_OPCODE_SHIFT            27 */
/* #define PMIC_ARB_CMD_PRIORITY_SHIFT          26 */
/* /\* #define PMIC_ARB_CMD_SLAVE_ID_SHIFT          20 *\/ */
/* /\* #define PMIC_ARB_CMD_ADDR_SHIFT              12 *\/ */
/* #define PMIC_ARB_CMD_ADDR_OFFSET_SHIFT       4 */
/* #define PMIC_ARB_CMD_BYTE_CNT_SHIFT          0 */

enum pmic_arb_cmd_op_code {
    PMIC_ARB_OP_EXT_WRITEL = 0,
    PMIC_ARB_OP_EXT_READL = 1,
    PMIC_ARB_OP_EXT_WRITE = 2,
    PMIC_ARB_OP_RESET = 3,
    PMIC_ARB_OP_SLEEP = 4,
    PMIC_ARB_OP_SHUTDOWN = 5,
    PMIC_ARB_OP_WAKEUP = 6,
    PMIC_ARB_OP_AUTHENTICATE = 7,
    PMIC_ARB_OP_MSTR_READ = 8,
    PMIC_ARB_OP_MSTR_WRITE = 9,
    PMIC_ARB_OP_EXT_READ = 13,
    PMIC_ARB_OP_WRITE = 14,
    PMIC_ARB_OP_READ = 15,
    PMIC_ARB_OP_ZERO_WRITE = 16,
};

enum pmic_arb_chnl_status {
    PMIC_ARB_STATUS_DONE = (1 << 0),
    PMIC_ARB_STATUS_FAILURE = (1 << 1),
    PMIC_ARB_STATUS_DENIED = (1 << 2),
    PMIC_ARB_STATUS_DROPPED = (1 << 3),
};

void pmic_arb_init();
i32 pmic_arb_read(u8 sid, u16 addr, u8 len, u8* data);




/* #define SPMI_CNFG   0x200a000 */
/* #define SPMI_CORE   0x200f000 */
/* #define SPMI_CHNLS  0x2400000 */
/* #define SPMI_OBSRVR 0x2c00000 */
/* #define SPMI_INTR   0x3800000 */

#define PMIC_ARB_CMD    0x00
#define PMIC_ARB_CONFIG 0x04
/* #define PMIC_ARB_STATUS 0x08 */
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
