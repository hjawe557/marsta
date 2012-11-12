#ifndef STM32L_USB_H
#define STM32L_USB_H


#define  USB_EP0R_EA                         ((uint16_t)0x000F)

#define  USB_EP0R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP0R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP0R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP0R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP0R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP0R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP0R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP0R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP0R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP0R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP0R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP0R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP0R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP0R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP0R_CTR_RX                     ((uint16_t)0x8000)

#define  USB_EP1R_EA                         ((uint16_t)0x000F)

#define  USB_EP1R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP1R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP1R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP1R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP1R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP1R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP1R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP1R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP1R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP1R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP1R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP1R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP1R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP1R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP1R_CTR_RX                     ((uint16_t)0x8000)

#define  USB_EP3R_EA                         ((uint16_t)0x000F)

#define  USB_EP3R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP3R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP3R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP3R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP3R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP3R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP3R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP3R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP3R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP3R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP3R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP3R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP3R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP3R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP3R_CTR_RX                     ((uint16_t)0x8000)

#if 0
#define  USB_EP4R_EA                         ((uint16_t)0x000F)

#define  USB_EP4R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP4R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP4R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP4R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP4R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP4R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP4R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP4R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP4R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP4R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP4R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP4R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP4R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP4R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP4R_CTR_RX                     ((uint16_t)0x8000)

#define  USB_EP5R_EA                         ((uint16_t)0x000F)

#define  USB_EP5R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP5R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP5R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP5R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP5R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP5R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP5R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP5R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP5R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP5R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP5R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP5R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP5R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP5R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP5R_CTR_RX                     ((uint16_t)0x8000)

#define  USB_EP6R_EA                         ((uint16_t)0x000F)

#define  USB_EP6R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP6R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP6R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP6R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP6R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP6R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP6R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP6R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP6R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP6R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP6R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP6R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP6R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP6R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP6R_CTR_RX                     ((uint16_t)0x8000)

#define  USB_EP7R_EA                         ((uint16_t)0x000F)

#define  USB_EP7R_STAT_TX                    ((uint16_t)0x0030)
#define  USB_EP7R_STAT_TX_0                  ((uint16_t)0x0010)
#define  USB_EP7R_STAT_TX_1                  ((uint16_t)0x0020)

#define  USB_EP7R_DTOG_TX                    ((uint16_t)0x0040)
#define  USB_EP7R_CTR_TX                     ((uint16_t)0x0080)
#define  USB_EP7R_EP_KIND                    ((uint16_t)0x0100)

#define  USB_EP7R_EP_TYPE                    ((uint16_t)0x0600)
#define  USB_EP7R_EP_TYPE_0                  ((uint16_t)0x0200)
#define  USB_EP7R_EP_TYPE_1                  ((uint16_t)0x0400)

#define  USB_EP7R_SETUP                      ((uint16_t)0x0800)

#define  USB_EP7R_STAT_RX                    ((uint16_t)0x3000)
#define  USB_EP7R_STAT_RX_0                  ((uint16_t)0x1000)
#define  USB_EP7R_STAT_RX_1                  ((uint16_t)0x2000)

#define  USB_EP7R_DTOG_RX                    ((uint16_t)0x4000)
#define  USB_EP7R_CTR_RX                     ((uint16_t)0x8000)
#endif
#define  USB_CNTR_FRES                       ((uint16_t)0x0001)
#define  USB_CNTR_PDWN                       ((uint16_t)0x0002)
#define  USB_CNTR_LP_MODE                    ((uint16_t)0x0004)
#define  USB_CNTR_FSUSP                      ((uint16_t)0x0008)
#define  USB_CNTR_RESUME                     ((uint16_t)0x0010)
#define  USB_CNTR_ESOFM                      ((uint16_t)0x0100)
#define  USB_CNTR_SOFM                       ((uint16_t)0x0200)
#define  USB_CNTR_RESETM                     ((uint16_t)0x0400)
#define  USB_CNTR_SUSPM                      ((uint16_t)0x0800)
#define  USB_CNTR_WKUPM                      ((uint16_t)0x1000)
#define  USB_CNTR_ERRM                       ((uint16_t)0x2000)
#define  USB_CNTR_PMAOVRM                    ((uint16_t)0x4000)
#define  USB_CNTR_CTRM                       ((uint16_t)0x8000)

#define  USB_ISTR_EP_ID                      ((uint16_t)0x000F)
#define  USB_ISTR_DIR                        ((uint16_t)0x0010)
#define  USB_ISTR_ESOF                       ((uint16_t)0x0100)
#define  USB_ISTR_SOF                        ((uint16_t)0x0200)
#define  USB_ISTR_RESET                      ((uint16_t)0x0400)
#define  USB_ISTR_SUSP                       ((uint16_t)0x0800)
#define  USB_ISTR_WKUP                       ((uint16_t)0x1000)
#define  USB_ISTR_ERR                        ((uint16_t)0x2000)
#define  USB_ISTR_PMAOVR                     ((uint16_t)0x4000)
#define  USB_ISTR_CTR                        ((uint16_t)0x8000)

#define  USB_FNR_FN                          ((uint16_t)0x07FF)
#define  USB_FNR_LSOF                        ((uint16_t)0x1800)
#define  USB_FNR_LCK                         ((uint16_t)0x2000)
#define  USB_FNR_RXDM                        ((uint16_t)0x4000)
#define  USB_FNR_RXDP                        ((uint16_t)0x8000)

#define  USB_DADDR_ADD                       ((uint8_t)0x7F)   
#define  USB_DADDR_ADD0                      ((uint8_t)0x01)   
#define  USB_DADDR_ADD1                      ((uint8_t)0x02)   
#define  USB_DADDR_ADD2                      ((uint8_t)0x04)   
#define  USB_DADDR_ADD3                      ((uint8_t)0x08)   
#define  USB_DADDR_ADD4                      ((uint8_t)0x10)   
#define  USB_DADDR_ADD5                      ((uint8_t)0x20)   
#define  USB_DADDR_ADD6                      ((uint8_t)0x40)   

#define  USB_DADDR_EF                        ((uint8_t)0x80)   

#define  USB_BTABLE_BTABLE                   ((uint16_t)0xFFF8)

#define  USB_COUNT0_RX_COUNT0_RX             ((uint16_t)0x03FF)
#define  USB_COUNT0_RX_NUM_BLOCK             ((uint16_t)0x7C00)
#define  USB_COUNT0_RX_BLSIZE                ((uint16_t)0x8000)

#endif
