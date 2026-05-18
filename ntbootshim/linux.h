// linux protocol defs from limine, adapted to use nt types

#pragma once

#include <basetsd.h>

// The following definitions and typedef struct _were copied and adapted from Linux
// kernel headers released under GPL-2.0 WITH Linux-syscall-note
// allowing their inclusion in non GPL compliant code.

#define EDD_MBR_SIG_MAX			  16
#define E820_MAX_ENTRIES_ZEROPAGE 128
#define EDDMAXNR				  6

#pragma pack(push, 1)

typedef struct _screen_info
{
	UINT8 orig_x;			  /* 0x00 */
	UINT8 orig_y;			  /* 0x01 */
	UINT16 ext_mem_k;		  /* 0x02 */
	UINT16 orig_video_page;	  /* 0x04 */
	UINT8 orig_video_mode;	  /* 0x06 */
	UINT8 orig_video_cols;	  /* 0x07 */
	UINT8 flags;			  /* 0x08 */
	UINT8 unused2;			  /* 0x09 */
	UINT16 orig_video_ega_bx; /* 0x0a */
	UINT16 unused3;			  /* 0x0c */
	UINT8 orig_video_lines;	  /* 0x0e */
	UINT8 orig_video_isVGA;	  /* 0x0f */
	UINT16 orig_video_points; /* 0x10 */

	/* VESA graphic mode -- linear frame buffer */
	UINT16 lfb_width;			/* 0x12 */
	UINT16 lfb_height;			/* 0x14 */
	UINT16 lfb_depth;			/* 0x16 */
	UINT32 lfb_base;			/* 0x18 */
	UINT32 lfb_size;			/* 0x1c */
	UINT16 cl_magic, cl_offset; /* 0x20 */
	UINT16 lfb_linelength;		/* 0x24 */
	UINT8 red_size;				/* 0x26 */
	UINT8 red_pos;				/* 0x27 */
	UINT8 green_size;			/* 0x28 */
	UINT8 green_pos;			/* 0x29 */
	UINT8 blue_size;			/* 0x2a */
	UINT8 blue_pos;				/* 0x2b */
	UINT8 rsvd_size;			/* 0x2c */
	UINT8 rsvd_pos;				/* 0x2d */
	UINT16 vesapm_seg;			/* 0x2e */
	UINT16 vesapm_off;			/* 0x30 */
	UINT16 pages;				/* 0x32 */
	UINT16 vesa_attributes;		/* 0x34 */
	UINT32 capabilities;		/* 0x36 */
	UINT32 ext_lfb_base;		/* 0x3a */
	UINT8 _reserved[2];			/* 0x3e */
} LINUX_SCREEN_INFO, *PLINUX_SCREEN_INFO;

#define VIDEO_TYPE_MDA	0x10 /* Monochrome Text Display    */
#define VIDEO_TYPE_CGA	0x11 /* CGA Display             */
#define VIDEO_TYPE_EGAM 0x20 /* EGA/VGA in Monochrome Mode    */
#define VIDEO_TYPE_EGAC 0x21 /* EGA in Color Mode        */
#define VIDEO_TYPE_VGAC 0x22 /* VGA+ in Color Mode        */
#define VIDEO_TYPE_VLFB 0x23 /* VESA VGA in graphic mode    */

#define VIDEO_TYPE_PICA_S3	 0x30 /* ACER PICA-61 local S3 video    */
#define VIDEO_TYPE_MIPS_G364 0x31 /* MIPS Magnum 4000 G364 video  */
#define VIDEO_TYPE_SGI		 0x33 /* Various SGI graphics hardware */

#define VIDEO_TYPE_TGAC 0x40 /* DEC TGA */

#define VIDEO_TYPE_SUN	  0x50 /* Sun frame buffer. */
#define VIDEO_TYPE_SUNPCI 0x51 /* Sun PCI based frame buffer. */

#define VIDEO_TYPE_PMAC 0x60 /* PowerMacintosh frame buffer. */

#define VIDEO_TYPE_EFI 0x70 /* EFI graphic mode        */

#define VIDEO_FLAGS_NOCURSOR (1 << 0) /* The video mode has no cursor set */

#define VIDEO_CAPABILITY_SKIP_QUIRKS (1 << 0)
#define VIDEO_CAPABILITY_64BIT_BASE	 (1 << 1) /* Frame buffer base is 64-bit */

typedef struct _setup_header
{
	UINT8 setup_sects;
	UINT16 root_flags;
	UINT32 syssize;
	UINT16 ram_size;
	UINT16 vid_mode;
	UINT16 root_dev;
	UINT16 boot_flag;
	UINT16 jump;
	UINT32 header;
	UINT16 version;
	UINT32 realmode_swtch;
	UINT16 start_sys_seg;
	UINT16 kernel_version;
	UINT8 type_of_loader;
	UINT8 loadflags;
	UINT16 setup_move_size;
	UINT32 code32_start;
	UINT32 ramdisk_image;
	UINT32 ramdisk_size;
	UINT32 bootsect_kludge;
	UINT16 heap_end_ptr;
	UINT8 ext_loader_ver;
	UINT8 ext_loaderype;
	UINT32 cmd_line_ptr;
	UINT32 initrd_addr_max;
	UINT32 kernel_alignment;
	UINT8 relocatable_kernel;
	UINT8 min_alignment;
	UINT16 xloadflags;
	UINT32 cmdline_size;
	UINT32 hardware_subarch;
	UINT64 hardware_subarch_data;
	UINT32 payload_offset;
	UINT32 payload_length;
	UINT64 setup_data;
	UINT64 pref_address;
	UINT32 init_size;
	UINT32 handover_offset;
	UINT32 kernel_info_offset;
} LINUX_SETUP_HEADER, *PLINUX_SETUP_HEADER;

typedef struct _apm_bios_info
{
	UINT16 version;
	UINT16 cseg;
	UINT32 offset;
	UINT16 cseg_16;
	UINT16 dseg;
	UINT16 flags;
	UINT16 cseg_len;
	UINT16 cseg_16_len;
	UINT16 dseg_len;
} LINUX_APM_BIOS_INFO, *PLINUX_APM_BIOS_INFO;

typedef struct _ist_info
{
	UINT32 signature;
	UINT32 command;
	UINT32 event;
	UINT32 perf_level;
} LINUX_IST_INFO, *PLINUX_IST_INFO;

typedef struct _sys_desc_table
{
	UINT16 length;
	UINT8 table[14];
} LINUX_SYS_DESC_TABLE, *PLINUX_SYS_DESC_TABLE;

typedef struct _olpc_ofw_header
{
	UINT32 ofw_magic; /* OFW signature */
	UINT32 ofw_version;
	UINT32 cif_handler; /* callback into OFW */
	UINT32 irq_descable;
} LINUX_OLPC_OFW_HEADER, *PLINUX_OLPC_OFW_HEADER;

typedef struct _edid_info
{
	UINT8 dummy[128];
} LINUX_EDID_INFO, *PLINUX_EDID_INFO;

typedef struct _efi_info
{
	UINT32 efi_loader_signature;
	UINT32 efi_systab;
	UINT32 efi_memdesc_size;
	UINT32 efi_memdesc_version;
	UINT32 efi_memmap;
	UINT32 efi_memmap_size;
	UINT32 efi_systab_hi;
	UINT32 efi_memmap_hi;
} LINUX_EFI_INFO, *PLINUX_EFI_INFO;

#define E820_RAM	  1
#define E820_RESERVED 2
#define E820_ACPI	  3
#define E820_NVS	  4
#define E820_UNUSABLE 5
#define E820_PMEM	  7

typedef struct _boot_e820_entry
{
	UINT64 addr;
	UINT64 size;
	UINT32 type;
} LINUX_BOOT_E820_INFO, *PLINUX_BOOT_E820_INFO;

typedef struct _edd_device_params
{
	UINT16 length;
	UINT16 info_flags;
	UINT32 num_default_cylinders;
	UINT32 num_default_heads;
	UINT32 sectors_perrack;
	UINT64 number_of_sectors;
	UINT16 bytes_per_sector;
	UINT32 dpte_ptr;			   /* 0xFFFFFFFF for our purposes */
	UINT16 key;					   /* = 0xBEDD */
	UINT8 device_path_info_length; /* = 44 */
	UINT8 reserved2;
	UINT16 reserved3;
	UINT8 host_busype[4];
	UINT8 interfaceype[8];
	union {
		struct
		{
			UINT16 base_address;
			UINT16 reserved1;
			UINT32 reserved2;
		} isa;
		struct
		{
			UINT8 bus;
			UINT8 slot;
			UINT8 function;
			UINT8 channel;
			UINT32 reserved;
		} pci;
		/* pcix is same as pci */
		struct
		{
			UINT64 reserved;
		} ibnd;
		struct
		{
			UINT64 reserved;
		} xprs;
		struct
		{
			UINT64 reserved;
		} htpt;
		struct
		{
			UINT64 reserved;
		} unknown;
	} interface_path;
	union {
		struct
		{
			UINT8 device;
			UINT8 reserved1;
			UINT16 reserved2;
			UINT32 reserved3;
			UINT64 reserved4;
		} ata;
		struct
		{
			UINT8 device;
			UINT8 lun;
			UINT8 reserved1;
			UINT8 reserved2;
			UINT32 reserved3;
			UINT64 reserved4;
		} atapi;
		struct
		{
			UINT16 id;
			UINT64 lun;
			UINT16 reserved1;
			UINT32 reserved2;
		} scsi;
		struct
		{
			UINT64 serial_number;
			UINT64 reserved;
		} usb;
		struct
		{
			UINT64 eui;
			UINT64 reserved;
		} i1394;
		struct
		{
			UINT64 wwid;
			UINT64 lun;
		} fibre;
		struct
		{
			UINT64 identityag;
			UINT64 reserved;
		} i2o;
		struct
		{
			UINT32 array_number;
			UINT32 reserved1;
			UINT64 reserved2;
		} raid;
		struct
		{
			UINT8 device;
			UINT8 reserved1;
			UINT16 reserved2;
			UINT32 reserved3;
			UINT64 reserved4;
		} sata;
		struct
		{
			UINT64 reserved1;
			UINT64 reserved2;
		} unknown;
	} device_path;
	UINT8 reserved4;
	UINT8 checksum;
} LINUX_EDD_DEVICE_PARAMS, *PLINUX_EDD_DEVICE_PARAMS;

typedef struct _edd_info
{
	UINT8 device;
	UINT8 version;
	UINT16 interface_support;
	UINT16 legacy_max_cylinder;
	UINT8 legacy_max_head;
	UINT8 legacy_sectors_perrack;
	_edd_device_params params;
} LINUX_EDD_INFO, *PLINUX_EDD_INFO;

typedef struct _boot_params
{
	_screen_info screen_info;						/* 0x000 */
	_apm_bios_info apm_bios_info;					/* 0x040 */
	UINT8 _pad2[4];									/* 0x054 */
	UINT64 tboot_addr;								/* 0x058 */
	_ist_info ist_info;								/* 0x060 */
	UINT64 acpi_rsdp_addr;							/* 0x070 */
	UINT8 _pad3[8];									/* 0x078 */
	UINT8 hd0_info[16]; /* obsolete! */				/* 0x080 */
	UINT8 hd1_info[16]; /* obsolete! */				/* 0x090 */
	_sys_desc_table sys_desc_table; /* obsolete! */ /* 0x0a0 */
	_olpc_ofw_header olpc_ofw_header;				/* 0x0b0 */
	UINT32 ext_ramdisk_image;						/* 0x0c0 */
	UINT32 ext_ramdisk_size;						/* 0x0c4 */
	UINT32 ext_cmd_line_ptr;						/* 0x0c8 */
	UINT8 _pad4[116];								/* 0x0cc */
	_edid_info edid_info;							/* 0x140 */
	_efi_info efi_info;								/* 0x1c0 */
	UINT32 alt_mem_k;								/* 0x1e0 */
	UINT32 scratch; /* Scratch field! */			/* 0x1e4 */
	UINT8 e820_entries;								/* 0x1e8 */
	UINT8 eddbuf_entries;							/* 0x1e9 */
	UINT8 edd_mbr_sig_buf_entries;					/* 0x1ea */
	UINT8 kbd_status;								/* 0x1eb */
	UINT8 secure_boot;								/* 0x1ec */
	UINT8 _pad5[2];									/* 0x1ed */
	/*
	 * The sentinel is set to a nonzero value (0xff) in header.S.
	 *
	 * A bootloader is supposed to only take setup_header and put
	 * it into a clean boot_params buffer. If it turns out that
	 * it is clumsy or too generous with the buffer, it most
	 * probably will pick up the sentinel variable too. The fact
	 * that this variable then is still 0xff will let kernel
	 * know that some variables in boot_params are invalid and
	 * kernel should zero out certain portions of boot_params.
	 */
	UINT8 sentinel;						  /* 0x1ef */
	UINT8 _pad6[1];						  /* 0x1f0 */
	_setup_header hdr; /* setup header */ /* 0x1f1 */
	UINT8 _pad7[0x290 - 0x1f1 - sizeof(_setup_header)];
	UINT32 edd_mbr_sig_buffer[EDD_MBR_SIG_MAX];				/* 0x290 */
	_boot_e820_entry e820_table[E820_MAX_ENTRIES_ZEROPAGE]; /* 0x2d0 */
	UINT8 _pad8[48];										/* 0xcd0 */
	_edd_info eddbuf[EDDMAXNR];								/* 0xd00 */
	UINT8 _pad9[276];										/* 0xeec */
} LINUX_BOOT_PARAMS, *PLINUX_BOOT_PARAMS;

#pragma pack(pop)

#define SETUP_HEADER_OFFSET 0x1f1
#define JUMP_OFFSET 0x202

#define VMLINUZ_NAME L"vmlinuz.sys"
#define INITRD_NAME	 L"initrd.sys"

typedef void (*PFN_LINUX_STARTUP_64)();
