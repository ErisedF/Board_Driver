/1.*******************************/
fsl_common.h 通用状态定义
fsl_iomuxc.h 所有引角的定义
MCIMX6Y2.h   寄存器结构体


/2.********************************/

cc.h 为自己定义，是数据类型的定义

/3.********************************/
值与结构体一一对应
fsl_iomuxc.h ：#define IOMUXC_GPIO1_IO03_GPIO1_IO03                         0x020E0068U, 0x5U, 0x00000000U, 0x0U, 0x020E02F4U
MCIMX6Y2.h ：static inline void IOMUXC_SetPinMux(uint32_t muxRegister,
                                    uint32_t muxMode,
                                    uint32_t inputRegister,
                                    uint32_t inputDaisy,
                                    uint32_t configRegister,
                                    uint32_t inputOnfield)