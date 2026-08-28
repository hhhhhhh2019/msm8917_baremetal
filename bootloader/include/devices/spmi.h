#ifndef SPMI_H_
#define SPMI_H_

#define SPMI_CORE   0x200f000ULL
#define SPMI_CHNLS  0x2400000ULL
#define SPMI_OBSRVR 0x2c00000ULL
#define SPMI_INTR   0x3800000ULL
#define SPMI_CNFG   0x200a000ULL

#define CHNL_IDX(slave, ppid) ((slave << 8) | ppid)

#define PMIC_ARB_REG_CHNL(n) ((u32*)(SPMI_CORE + 0x800 + (n) * 4))

#define PMIC_ARB_CHNLn(n)          ((u32*)(SPMI_CHNLS + (n) * 0x8000))
#define PMIC_ARB_CHNLn_CMD(n)      (PMIC_ARB_CHNLn(n) + 0)
#define PMIC_ARB_CHNLn_CONFIG(n)   (PMIC_ARB_CHNLn(n) + 1)
#define PMIC_ARB_CHNLn_STATUS(n)   (PMIC_ARB_CHNLn(n) + 2)
#define PMIC_ARB_CHNLn_WDATA(n, x) (PMIC_ARB_CHNLn(n) + 4 + (x))
#define PMIC_ARB_CHNLn_RDATA(n, x) (PMIC_ARB_CHNLn(n) + 6 + (x))

#define PMIC_ARB_IRQ(n)        ((u32*)(SPMI_INTR + 0x1000 * (n)))
#define PMIC_ARB_IRQ_ENABLE(n) (PMIC_ARB_IRQ(n) + 0)
#define PMIC_ARB_IRQ_STATUS(n) (PMIC_ARB_IRQ(n) + 1)
#define PMIC_ARB_IRQ_CLEAR(n)  (PMIC_ARB_IRQ(n) + 2)

/* #define PMIC_ARB_IRQ_ENABLE(n) ((u32*)(SPMI_INTR + 4 * (n) + 0x200)) */
/* #define PMIC_ARB_IRQ_STATUS(n) ((u32*)(SPMI_INTR + 4 * (n) + 0x600)) */
/* #define PMIC_ARB_IRQ_CLEAR(n)  ((u32*)(SPMI_INTR + 4 * (n) + 0xa00)) */

#define SPMI_IRQ (0xbe + 32)

#define PMIC_ARB_CMD_OPCODE_SHIFT   27
#define PMIC_ARB_CMD_PRIORITY_SHIFT 26
/* #define PMIC_ARB_CMD_SLAVE_ID_SHIFT          20 */
/* #define PMIC_ARB_CMD_ADDR_SHIFT              12 */
#define PMIC_ARB_CMD_ADDR_OFFSET_SHIFT 4
#define PMIC_ARB_CMD_BYTE_CNT_SHIFT    0

enum pmic_arb_cmd_op_code {
    PMIC_ARB_OP_EXT_WRITEL   = 0,
    PMIC_ARB_OP_EXT_READL    = 1,
    PMIC_ARB_OP_EXT_WRITE    = 2,
    PMIC_ARB_OP_RESET        = 3,
    PMIC_ARB_OP_SLEEP        = 4,
    PMIC_ARB_OP_SHUTDOWN     = 5,
    PMIC_ARB_OP_WAKEUP       = 6,
    PMIC_ARB_OP_AUTHENTICATE = 7,
    PMIC_ARB_OP_MSTR_READ    = 8,
    PMIC_ARB_OP_MSTR_WRITE   = 9,
    PMIC_ARB_OP_EXT_READ     = 13,
    PMIC_ARB_OP_WRITE        = 14,
    PMIC_ARB_OP_READ         = 15,
    PMIC_ARB_OP_ZERO_WRITE   = 16,
};

enum pmic_arb_chnl_status {
    PMIC_ARB_STATUS_DONE    = (1 << 0),
    PMIC_ARB_STATUS_FAILURE = (1 << 1),
    PMIC_ARB_STATUS_DENIED  = (1 << 2),
    PMIC_ARB_STATUS_DROPPED = (1 << 3),
};

void pmic_arb_init();
i32 pmic_arb_read(u8 sid, u16 addr, u8 len, u8* data);
i32 pmic_arb_write(u8 sid, u16 addr, u8 len, u8* data);

#endif // SPMI_H_
