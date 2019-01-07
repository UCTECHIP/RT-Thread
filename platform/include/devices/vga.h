#ifndef SDJ2102_VGA_H
#define SDJ2102_VGA_H

//// VGA_register ///
#define VGA_CTRL	0x00
#define VGA_STAT	0x04
#define VGA_HTIM	0x08
#define VGA_VTIM	0x0c
#define VGA_HVLEN	0x10
#define VGA_VBARA	0x14
#define VGA_VBARB	0x18

/// Control register(CTRL) ///
			//bits 31-16 are reserved//
#define VGA_BL		1<<15
#define VGA_CSL		1<<14
#define VGA_VSL		1<<13
#define VGA_HSL		1<<12
#define VGA_PC		1<<11
#define VGA_CD_H	1<<10
#define VGA_CD_L	1<<9
#define VGA_VBL_H	1<<8
#define VGA_VBL_L	1<<7
#define	VGA_CBSWE	1<<6 
#define	VGA_VBSWE	1<<5
#define	VGA_CBSIE	1<<4
#define	VGA_VBSIE	1<<3
#define	VGA_HIE		1<<2
#define VGA_VIE		1<<1
#define	VGA_VEN		1<<0


/// Status register(STAT) ///
			//bits 31-18 are reserved//
#define	VGA_ACMP	1<<17 
#define	VGA_AVMP	1<<16
			//bits 15-8 are reserved//
#define	VGA_CBSINT	1<<7
#define	VGA_VBSINT	1<<6
#define	VGA_HINT	1<<5
#define	VGA_VINT	1<<4
			//bits 3-2 are reserved//
#define	VGA_LUINT	1<<1
#define VGA_SINT	1<<0


/// Horizontal Timing Register(HTIM) ///
#define VGA_THSYNC	1<<24	//[31:24]	Horizontal pulse width
#define VGA_THGDEL	1<<16	//[23:16]	gate delay time
#define VGA_THGATE	1<<0	//[15:0]	gate time


/// Vertcial Timing Register(VTIM) ///
#define VGA_TVSYNC      1<<24   //[31:24]       vertical pulse width
#define VGA_TVGDEL      1<<16   //[23:16]       gate delay time
#define VGA_TVGATE      1<<0    //[15:0]        gate time

/// Horizontal and Vertcial Length registers(HVLEN) ///
#define VGA_THLEN	1<<16	//[31:16]	Horizontal length
#define VGA_TVLEN	1<<0	//[15:0]	Vertical length


/// VGA VBARA(VBARA) ///
#define VBARA		1<<2	//[31:2]
				//[1:0]=00

/// VGA VBARB(VBARB) ///	
#define VBARB		1<<2	//[31:2]
				//[1:0]=00
void vga_init();
void vga_start();
void vga_close();
void vga_change();
void vga_htim(char thsync,char thgdel,int thgate);
void vga_vtim(char tvsync,char tvgdel,int tvgate);
void vga_hvlen(int thlen ,int tvlen);
void vga_vbara(int vbara);
void vga_vbarb(int vbarb);
#endif
