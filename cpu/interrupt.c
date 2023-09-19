#include "interrupt.h"

#include "gdt.h"
#include "idt.h"

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);
extern void isr48(void);
extern void isr49(void);
extern void isr50(void);
extern void isr51(void);
extern void isr52(void);
extern void isr53(void);
extern void isr54(void);
extern void isr55(void);
extern void isr56(void);
extern void isr57(void);
extern void isr58(void);
extern void isr59(void);
extern void isr60(void);
extern void isr61(void);
extern void isr62(void);
extern void isr63(void);
extern void isr64(void);
extern void isr65(void);
extern void isr66(void);
extern void isr67(void);
extern void isr68(void);
extern void isr69(void);
extern void isr70(void);
extern void isr71(void);
extern void isr72(void);
extern void isr73(void);
extern void isr74(void);
extern void isr75(void);
extern void isr76(void);
extern void isr77(void);
extern void isr78(void);
extern void isr79(void);
extern void isr80(void);
extern void isr81(void);
extern void isr82(void);
extern void isr83(void);
extern void isr84(void);
extern void isr85(void);
extern void isr86(void);
extern void isr87(void);
extern void isr88(void);
extern void isr89(void);
extern void isr90(void);
extern void isr91(void);
extern void isr92(void);
extern void isr93(void);
extern void isr94(void);
extern void isr95(void);
extern void isr96(void);
extern void isr97(void);
extern void isr98(void);
extern void isr99(void);
extern void isr100(void);
extern void isr101(void);
extern void isr102(void);
extern void isr103(void);
extern void isr104(void);
extern void isr105(void);
extern void isr106(void);
extern void isr107(void);
extern void isr108(void);
extern void isr109(void);
extern void isr110(void);
extern void isr111(void);
extern void isr112(void);
extern void isr113(void);
extern void isr114(void);
extern void isr115(void);
extern void isr116(void);
extern void isr117(void);
extern void isr118(void);
extern void isr119(void);
extern void isr120(void);
extern void isr121(void);
extern void isr122(void);
extern void isr123(void);
extern void isr124(void);
extern void isr125(void);
extern void isr126(void);
extern void isr127(void);
extern void isr128(void);
extern void isr129(void);
extern void isr130(void);
extern void isr131(void);
extern void isr132(void);
extern void isr133(void);
extern void isr134(void);
extern void isr135(void);
extern void isr136(void);
extern void isr137(void);
extern void isr138(void);
extern void isr139(void);
extern void isr140(void);
extern void isr141(void);
extern void isr142(void);
extern void isr143(void);
extern void isr144(void);
extern void isr145(void);
extern void isr146(void);
extern void isr147(void);
extern void isr148(void);
extern void isr149(void);
extern void isr150(void);
extern void isr151(void);
extern void isr152(void);
extern void isr153(void);
extern void isr154(void);
extern void isr155(void);
extern void isr156(void);
extern void isr157(void);
extern void isr158(void);
extern void isr159(void);
extern void isr160(void);
extern void isr161(void);
extern void isr162(void);
extern void isr163(void);
extern void isr164(void);
extern void isr165(void);
extern void isr166(void);
extern void isr167(void);
extern void isr168(void);
extern void isr169(void);
extern void isr170(void);
extern void isr171(void);
extern void isr172(void);
extern void isr173(void);
extern void isr174(void);
extern void isr175(void);
extern void isr176(void);
extern void isr177(void);
extern void isr178(void);
extern void isr179(void);
extern void isr180(void);
extern void isr181(void);
extern void isr182(void);
extern void isr183(void);
extern void isr184(void);
extern void isr185(void);
extern void isr186(void);
extern void isr187(void);
extern void isr188(void);
extern void isr189(void);
extern void isr190(void);
extern void isr191(void);
extern void isr192(void);
extern void isr193(void);
extern void isr194(void);
extern void isr195(void);
extern void isr196(void);
extern void isr197(void);
extern void isr198(void);
extern void isr199(void);
extern void isr200(void);
extern void isr201(void);
extern void isr202(void);
extern void isr203(void);
extern void isr204(void);
extern void isr205(void);
extern void isr206(void);
extern void isr207(void);
extern void isr208(void);
extern void isr209(void);
extern void isr210(void);
extern void isr211(void);
extern void isr212(void);
extern void isr213(void);
extern void isr214(void);
extern void isr215(void);
extern void isr216(void);
extern void isr217(void);
extern void isr218(void);
extern void isr219(void);
extern void isr220(void);
extern void isr221(void);
extern void isr222(void);
extern void isr223(void);
extern void isr224(void);
extern void isr225(void);
extern void isr226(void);
extern void isr227(void);
extern void isr228(void);
extern void isr229(void);
extern void isr230(void);
extern void isr231(void);
extern void isr232(void);
extern void isr233(void);
extern void isr234(void);
extern void isr235(void);
extern void isr236(void);
extern void isr237(void);
extern void isr238(void);
extern void isr239(void);
extern void isr240(void);
extern void isr241(void);
extern void isr242(void);
extern void isr243(void);
extern void isr244(void);
extern void isr245(void);
extern void isr246(void);
extern void isr247(void);
extern void isr248(void);
extern void isr249(void);
extern void isr250(void);
extern void isr251(void);
extern void isr252(void);
extern void isr253(void);
extern void isr254(void);
extern void isr255(void);

#define GATE_SPLIT_OFFSET(handler)   \
	(((__uint128_t)handler & 0xFFFF) \
		| (((__uint128_t)handler & 0xFFFF'FFFF'FFFF'0000) >> 16) << 48)
#define GATE_DESCR(descr) ((__uint128_t)descr << 16)
#define GATE_TYPE(type)   ((__uint128_t)type << 40)

#define GATE_TYPE_INT (0xE00'0000'0000)

/**
 * @brief Register the interrupt handler stubs in the IDT.
 */
void idt_register_stubs(void) {
	idt[0] = GATE_SPLIT_OFFSET(isr0) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[1] = GATE_SPLIT_OFFSET(isr1) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[2] = GATE_SPLIT_OFFSET(isr2) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[3] = GATE_SPLIT_OFFSET(isr3) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[4] = GATE_SPLIT_OFFSET(isr4) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[5] = GATE_SPLIT_OFFSET(isr5) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[6] = GATE_SPLIT_OFFSET(isr6) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[7] = GATE_SPLIT_OFFSET(isr7) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[8] = GATE_SPLIT_OFFSET(isr8) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[9] = GATE_SPLIT_OFFSET(isr9) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	       | SEG_DPL_0 | GATE_TYPE_INT;
	idt[10] = GATE_SPLIT_OFFSET(isr10) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[11] = GATE_SPLIT_OFFSET(isr11) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[12] = GATE_SPLIT_OFFSET(isr12) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[13] = GATE_SPLIT_OFFSET(isr13) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[14] = GATE_SPLIT_OFFSET(isr14) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[15] = GATE_SPLIT_OFFSET(isr15) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[16] = GATE_SPLIT_OFFSET(isr16) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[17] = GATE_SPLIT_OFFSET(isr17) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[18] = GATE_SPLIT_OFFSET(isr18) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[19] = GATE_SPLIT_OFFSET(isr19) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[20] = GATE_SPLIT_OFFSET(isr20) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[21] = GATE_SPLIT_OFFSET(isr21) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[22] = GATE_SPLIT_OFFSET(isr22) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[23] = GATE_SPLIT_OFFSET(isr23) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[24] = GATE_SPLIT_OFFSET(isr24) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[25] = GATE_SPLIT_OFFSET(isr25) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[26] = GATE_SPLIT_OFFSET(isr26) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[27] = GATE_SPLIT_OFFSET(isr27) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[28] = GATE_SPLIT_OFFSET(isr28) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[29] = GATE_SPLIT_OFFSET(isr29) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[30] = GATE_SPLIT_OFFSET(isr30) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[31] = GATE_SPLIT_OFFSET(isr31) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[32] = GATE_SPLIT_OFFSET(isr32) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[33] = GATE_SPLIT_OFFSET(isr33) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[34] = GATE_SPLIT_OFFSET(isr34) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[35] = GATE_SPLIT_OFFSET(isr35) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[36] = GATE_SPLIT_OFFSET(isr36) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[37] = GATE_SPLIT_OFFSET(isr37) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[38] = GATE_SPLIT_OFFSET(isr38) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[39] = GATE_SPLIT_OFFSET(isr39) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[40] = GATE_SPLIT_OFFSET(isr40) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[41] = GATE_SPLIT_OFFSET(isr41) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[42] = GATE_SPLIT_OFFSET(isr42) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[43] = GATE_SPLIT_OFFSET(isr43) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[44] = GATE_SPLIT_OFFSET(isr44) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[45] = GATE_SPLIT_OFFSET(isr45) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[46] = GATE_SPLIT_OFFSET(isr46) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[47] = GATE_SPLIT_OFFSET(isr47) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[48] = GATE_SPLIT_OFFSET(isr48) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[49] = GATE_SPLIT_OFFSET(isr49) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[50] = GATE_SPLIT_OFFSET(isr50) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[51] = GATE_SPLIT_OFFSET(isr51) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[52] = GATE_SPLIT_OFFSET(isr52) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[53] = GATE_SPLIT_OFFSET(isr53) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[54] = GATE_SPLIT_OFFSET(isr54) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[55] = GATE_SPLIT_OFFSET(isr55) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[56] = GATE_SPLIT_OFFSET(isr56) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[57] = GATE_SPLIT_OFFSET(isr57) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[58] = GATE_SPLIT_OFFSET(isr58) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[59] = GATE_SPLIT_OFFSET(isr59) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[60] = GATE_SPLIT_OFFSET(isr60) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[61] = GATE_SPLIT_OFFSET(isr61) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[62] = GATE_SPLIT_OFFSET(isr62) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[63] = GATE_SPLIT_OFFSET(isr63) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[64] = GATE_SPLIT_OFFSET(isr64) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[65] = GATE_SPLIT_OFFSET(isr65) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[66] = GATE_SPLIT_OFFSET(isr66) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[67] = GATE_SPLIT_OFFSET(isr67) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[68] = GATE_SPLIT_OFFSET(isr68) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[69] = GATE_SPLIT_OFFSET(isr69) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[70] = GATE_SPLIT_OFFSET(isr70) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[71] = GATE_SPLIT_OFFSET(isr71) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[72] = GATE_SPLIT_OFFSET(isr72) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[73] = GATE_SPLIT_OFFSET(isr73) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[74] = GATE_SPLIT_OFFSET(isr74) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[75] = GATE_SPLIT_OFFSET(isr75) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[76] = GATE_SPLIT_OFFSET(isr76) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[77] = GATE_SPLIT_OFFSET(isr77) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[78] = GATE_SPLIT_OFFSET(isr78) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[79] = GATE_SPLIT_OFFSET(isr79) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[80] = GATE_SPLIT_OFFSET(isr80) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[81] = GATE_SPLIT_OFFSET(isr81) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[82] = GATE_SPLIT_OFFSET(isr82) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[83] = GATE_SPLIT_OFFSET(isr83) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[84] = GATE_SPLIT_OFFSET(isr84) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[85] = GATE_SPLIT_OFFSET(isr85) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[86] = GATE_SPLIT_OFFSET(isr86) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[87] = GATE_SPLIT_OFFSET(isr87) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[88] = GATE_SPLIT_OFFSET(isr88) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[89] = GATE_SPLIT_OFFSET(isr89) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[90] = GATE_SPLIT_OFFSET(isr90) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[91] = GATE_SPLIT_OFFSET(isr91) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[92] = GATE_SPLIT_OFFSET(isr92) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[93] = GATE_SPLIT_OFFSET(isr93) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[94] = GATE_SPLIT_OFFSET(isr94) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[95] = GATE_SPLIT_OFFSET(isr95) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[96] = GATE_SPLIT_OFFSET(isr96) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[97] = GATE_SPLIT_OFFSET(isr97) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[98] = GATE_SPLIT_OFFSET(isr98) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[99] = GATE_SPLIT_OFFSET(isr99) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	        | SEG_DPL_0 | GATE_TYPE_INT;
	idt[100] = GATE_SPLIT_OFFSET(isr100) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[101] = GATE_SPLIT_OFFSET(isr101) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[102] = GATE_SPLIT_OFFSET(isr102) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[103] = GATE_SPLIT_OFFSET(isr103) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[104] = GATE_SPLIT_OFFSET(isr104) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[105] = GATE_SPLIT_OFFSET(isr105) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[106] = GATE_SPLIT_OFFSET(isr106) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[107] = GATE_SPLIT_OFFSET(isr107) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[108] = GATE_SPLIT_OFFSET(isr108) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[109] = GATE_SPLIT_OFFSET(isr109) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[110] = GATE_SPLIT_OFFSET(isr110) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[111] = GATE_SPLIT_OFFSET(isr111) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[112] = GATE_SPLIT_OFFSET(isr112) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[113] = GATE_SPLIT_OFFSET(isr113) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[114] = GATE_SPLIT_OFFSET(isr114) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[115] = GATE_SPLIT_OFFSET(isr115) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[116] = GATE_SPLIT_OFFSET(isr116) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[117] = GATE_SPLIT_OFFSET(isr117) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[118] = GATE_SPLIT_OFFSET(isr118) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[119] = GATE_SPLIT_OFFSET(isr119) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[120] = GATE_SPLIT_OFFSET(isr120) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[121] = GATE_SPLIT_OFFSET(isr121) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[122] = GATE_SPLIT_OFFSET(isr122) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[123] = GATE_SPLIT_OFFSET(isr123) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[124] = GATE_SPLIT_OFFSET(isr124) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[125] = GATE_SPLIT_OFFSET(isr125) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[126] = GATE_SPLIT_OFFSET(isr126) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[127] = GATE_SPLIT_OFFSET(isr127) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[128] = GATE_SPLIT_OFFSET(isr128) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[129] = GATE_SPLIT_OFFSET(isr129) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[130] = GATE_SPLIT_OFFSET(isr130) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[131] = GATE_SPLIT_OFFSET(isr131) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[132] = GATE_SPLIT_OFFSET(isr132) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[133] = GATE_SPLIT_OFFSET(isr133) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[134] = GATE_SPLIT_OFFSET(isr134) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[135] = GATE_SPLIT_OFFSET(isr135) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[136] = GATE_SPLIT_OFFSET(isr136) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[137] = GATE_SPLIT_OFFSET(isr137) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[138] = GATE_SPLIT_OFFSET(isr138) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[139] = GATE_SPLIT_OFFSET(isr139) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[140] = GATE_SPLIT_OFFSET(isr140) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[141] = GATE_SPLIT_OFFSET(isr141) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[142] = GATE_SPLIT_OFFSET(isr142) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[143] = GATE_SPLIT_OFFSET(isr143) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[144] = GATE_SPLIT_OFFSET(isr144) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[145] = GATE_SPLIT_OFFSET(isr145) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[146] = GATE_SPLIT_OFFSET(isr146) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[147] = GATE_SPLIT_OFFSET(isr147) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[148] = GATE_SPLIT_OFFSET(isr148) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[149] = GATE_SPLIT_OFFSET(isr149) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[150] = GATE_SPLIT_OFFSET(isr150) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[151] = GATE_SPLIT_OFFSET(isr151) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[152] = GATE_SPLIT_OFFSET(isr152) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[153] = GATE_SPLIT_OFFSET(isr153) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[154] = GATE_SPLIT_OFFSET(isr154) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[155] = GATE_SPLIT_OFFSET(isr155) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[156] = GATE_SPLIT_OFFSET(isr156) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[157] = GATE_SPLIT_OFFSET(isr157) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[158] = GATE_SPLIT_OFFSET(isr158) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[159] = GATE_SPLIT_OFFSET(isr159) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[160] = GATE_SPLIT_OFFSET(isr160) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[161] = GATE_SPLIT_OFFSET(isr161) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[162] = GATE_SPLIT_OFFSET(isr162) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[163] = GATE_SPLIT_OFFSET(isr163) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[164] = GATE_SPLIT_OFFSET(isr164) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[165] = GATE_SPLIT_OFFSET(isr165) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[166] = GATE_SPLIT_OFFSET(isr166) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[167] = GATE_SPLIT_OFFSET(isr167) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[168] = GATE_SPLIT_OFFSET(isr168) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[169] = GATE_SPLIT_OFFSET(isr169) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[170] = GATE_SPLIT_OFFSET(isr170) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[171] = GATE_SPLIT_OFFSET(isr171) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[172] = GATE_SPLIT_OFFSET(isr172) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[173] = GATE_SPLIT_OFFSET(isr173) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[174] = GATE_SPLIT_OFFSET(isr174) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[175] = GATE_SPLIT_OFFSET(isr175) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[176] = GATE_SPLIT_OFFSET(isr176) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[177] = GATE_SPLIT_OFFSET(isr177) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[178] = GATE_SPLIT_OFFSET(isr178) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[179] = GATE_SPLIT_OFFSET(isr179) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[180] = GATE_SPLIT_OFFSET(isr180) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[181] = GATE_SPLIT_OFFSET(isr181) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[182] = GATE_SPLIT_OFFSET(isr182) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[183] = GATE_SPLIT_OFFSET(isr183) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[184] = GATE_SPLIT_OFFSET(isr184) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[185] = GATE_SPLIT_OFFSET(isr185) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[186] = GATE_SPLIT_OFFSET(isr186) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[187] = GATE_SPLIT_OFFSET(isr187) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[188] = GATE_SPLIT_OFFSET(isr188) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[189] = GATE_SPLIT_OFFSET(isr189) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[190] = GATE_SPLIT_OFFSET(isr190) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[191] = GATE_SPLIT_OFFSET(isr191) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[192] = GATE_SPLIT_OFFSET(isr192) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[193] = GATE_SPLIT_OFFSET(isr193) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[194] = GATE_SPLIT_OFFSET(isr194) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[195] = GATE_SPLIT_OFFSET(isr195) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[196] = GATE_SPLIT_OFFSET(isr196) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[197] = GATE_SPLIT_OFFSET(isr197) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[198] = GATE_SPLIT_OFFSET(isr198) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[199] = GATE_SPLIT_OFFSET(isr199) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[200] = GATE_SPLIT_OFFSET(isr200) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[201] = GATE_SPLIT_OFFSET(isr201) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[202] = GATE_SPLIT_OFFSET(isr202) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[203] = GATE_SPLIT_OFFSET(isr203) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[204] = GATE_SPLIT_OFFSET(isr204) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[205] = GATE_SPLIT_OFFSET(isr205) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[206] = GATE_SPLIT_OFFSET(isr206) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[207] = GATE_SPLIT_OFFSET(isr207) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[208] = GATE_SPLIT_OFFSET(isr208) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[209] = GATE_SPLIT_OFFSET(isr209) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[210] = GATE_SPLIT_OFFSET(isr210) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[211] = GATE_SPLIT_OFFSET(isr211) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[212] = GATE_SPLIT_OFFSET(isr212) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[213] = GATE_SPLIT_OFFSET(isr213) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[214] = GATE_SPLIT_OFFSET(isr214) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[215] = GATE_SPLIT_OFFSET(isr215) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[216] = GATE_SPLIT_OFFSET(isr216) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[217] = GATE_SPLIT_OFFSET(isr217) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[218] = GATE_SPLIT_OFFSET(isr218) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[219] = GATE_SPLIT_OFFSET(isr219) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[220] = GATE_SPLIT_OFFSET(isr220) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[221] = GATE_SPLIT_OFFSET(isr221) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[222] = GATE_SPLIT_OFFSET(isr222) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[223] = GATE_SPLIT_OFFSET(isr223) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[224] = GATE_SPLIT_OFFSET(isr224) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[225] = GATE_SPLIT_OFFSET(isr225) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[226] = GATE_SPLIT_OFFSET(isr226) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[227] = GATE_SPLIT_OFFSET(isr227) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[228] = GATE_SPLIT_OFFSET(isr228) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[229] = GATE_SPLIT_OFFSET(isr229) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[230] = GATE_SPLIT_OFFSET(isr230) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[231] = GATE_SPLIT_OFFSET(isr231) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[232] = GATE_SPLIT_OFFSET(isr232) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[233] = GATE_SPLIT_OFFSET(isr233) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[234] = GATE_SPLIT_OFFSET(isr234) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[235] = GATE_SPLIT_OFFSET(isr235) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[236] = GATE_SPLIT_OFFSET(isr236) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[237] = GATE_SPLIT_OFFSET(isr237) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[238] = GATE_SPLIT_OFFSET(isr238) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[239] = GATE_SPLIT_OFFSET(isr239) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[240] = GATE_SPLIT_OFFSET(isr240) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[241] = GATE_SPLIT_OFFSET(isr241) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[242] = GATE_SPLIT_OFFSET(isr242) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[243] = GATE_SPLIT_OFFSET(isr243) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[244] = GATE_SPLIT_OFFSET(isr244) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[245] = GATE_SPLIT_OFFSET(isr245) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[246] = GATE_SPLIT_OFFSET(isr246) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[247] = GATE_SPLIT_OFFSET(isr247) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[248] = GATE_SPLIT_OFFSET(isr248) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[249] = GATE_SPLIT_OFFSET(isr249) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[250] = GATE_SPLIT_OFFSET(isr250) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[251] = GATE_SPLIT_OFFSET(isr251) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[252] = GATE_SPLIT_OFFSET(isr252) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[253] = GATE_SPLIT_OFFSET(isr253) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[254] = GATE_SPLIT_OFFSET(isr254) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
	idt[255] = GATE_SPLIT_OFFSET(isr255) | GATE_DESCR(GDT_KERNEL_CS)
	         | SEG_PRESENT | SEG_DPL_0 | GATE_TYPE_INT;
}
