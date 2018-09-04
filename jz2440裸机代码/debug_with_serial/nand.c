
#define LARGER_NAND_PAGE

#define GSTATUS1        (*(volatile unsigned int *)0x560000B0)
#define BUSY            1

#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

#define NAND_SECTOR_SIZE_LP    2048
#define NAND_BLOCK_MASK_LP     (NAND_SECTOR_SIZE_LP - 1)

typedef unsigned int S3C24X0_REG32;


/* NAND FLASH (see S3C2410 manual chapter 6) */
typedef struct {
    S3C24X0_REG32   NFCONF;
    S3C24X0_REG32   NFCMD;
    S3C24X0_REG32   NFADDR;
    S3C24X0_REG32   NFDATA;
    S3C24X0_REG32   NFSTAT;
    S3C24X0_REG32   NFECC;
} S3C2410_NAND;

/* NAND FLASH (see S3C2440 manual chapter 6, www.100ask.net) */
typedef struct {
    S3C24X0_REG32   NFCONF;
    S3C24X0_REG32   NFCONT;
    S3C24X0_REG32   NFCMD;
    S3C24X0_REG32   NFADDR;
    S3C24X0_REG32   NFDATA;
    S3C24X0_REG32   NFMECCD0;
    S3C24X0_REG32   NFMECCD1;
    S3C24X0_REG32   NFSECCD;
    S3C24X0_REG32   NFSTAT;
    S3C24X0_REG32   NFESTAT0;
    S3C24X0_REG32   NFESTAT1;
    S3C24X0_REG32   NFMECC0;
    S3C24X0_REG32   NFMECC1;
    S3C24X0_REG32   NFSECC;
    S3C24X0_REG32   NFSBLK;
    S3C24X0_REG32   NFEBLK;
} S3C2440_NAND;


typedef struct {
    void (*nand_reset)(void);
    void (*wait_idle)(void);
    void (*nand_select_chip)(void);
    void (*nand_deselect_chip)(void);
    void (*write_cmd)(int cmd);
    void (*write_addr)(unsigned int addr);
    unsigned char (*read_data)(void);
}t_nand_chip;

//static S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
//static S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

//static t_nand_chip nand_chip;

/* ¹©Íâ²¿µ÷ÓÃµÄº¯Êý */
void nand_init(void);
void nand_read(unsigned char *buf, unsigned long start_addr, int size);

/* NAND Flash²Ù×÷µÄ×ÜÈë¿Ú, ËüÃÇ½«µ÷ÓÃS3C2410»òS3C2440µÄÏàÓ¦º¯Êý */
static void nand_reset(void);
static void wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void write_cmd(int cmd);
static void write_addr(unsigned int addr);
static unsigned char read_data(void);

/* S3C2410µÄNAND Flash´¦Àíº¯Êý */
static void s3c2410_nand_reset(void);
static void s3c2410_wait_idle(void);
static void s3c2410_nand_select_chip(void);
static void s3c2410_nand_deselect_chip(void);
static void s3c2410_write_cmd(int cmd);
static void s3c2410_write_addr(unsigned int addr);
static unsigned char s3c2410_read_data();

/* S3C2440µÄNAND Flash´¦Àíº¯Êý */
static void s3c2440_nand_reset(void);
static void s3c2440_wait_idle(void);
static void s3c2440_nand_select_chip(void);
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void);

/* S3C2410µÄNAND Flash²Ù×÷º¯Êý */

/* ¸´Î» */
static void s3c2410_nand_reset(void)
{
    s3c2410_nand_select_chip();
    s3c2410_write_cmd(0xff);  // ¸´Î»ÃüÁî
    s3c2410_wait_idle();
    s3c2410_nand_deselect_chip();
}

/* µÈ´ýNAND Flash¾ÍÐ÷ */
static void s3c2410_wait_idle(void)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* ·¢³öÆ¬Ñ¡ÐÅºÅ */
static void s3c2410_nand_select_chip(void)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;

    s3c2410nand->NFCONF &= ~(1<<11);
    for(i=0; i<10; i++);    
}

/* È¡ÏûÆ¬Ñ¡ÐÅºÅ */
static void s3c2410_nand_deselect_chip(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
    s3c2410nand->NFCONF |= (1<<11);
}

/* ·¢³öÃüÁî */
static void s3c2410_write_cmd(int cmd)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;

    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFCMD;
    *p = cmd;
}

/* ·¢³öµØÖ· */
static void s3c2410_write_addr(unsigned int addr)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFADDR;
    
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}

/* ¶ÁÈ¡Êý¾Ý */
static unsigned char s3c2410_read_data(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFDATA;
    return *p;
}

/* S3C2440µÄNAND Flash²Ù×÷º¯Êý */

/* ¸´Î» */
static void s3c2440_nand_reset(void)
{
    s3c2440_nand_select_chip();
    s3c2440_write_cmd(0xff);  // ¸´Î»ÃüÁî
    s3c2440_wait_idle();
    s3c2440_nand_deselect_chip();
}

/* µÈ´ýNAND Flash¾ÍÐ÷ */
static void s3c2440_wait_idle(void)
{
    int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* ·¢³öÆ¬Ñ¡ÐÅºÅ */
static void s3c2440_nand_select_chip(void)
{
    int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    s3c2440nand->NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

/* È¡ÏûÆ¬Ñ¡ÐÅºÅ */
static void s3c2440_nand_deselect_chip(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    s3c2440nand->NFCONT |= (1<<1);
}

/* ·¢³öÃüÁî */
static void s3c2440_write_cmd(int cmd)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
    *p = cmd;
}

/* ·¢³öµØÖ· */
static void s3c2440_write_addr(unsigned int addr)
{
    int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
    
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}


static void s3c2440_write_addr_lp(unsigned int addr)
{
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
	int col, page;

	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;
	
	*p = col & 0xff;			/* Column Address A0~A7 */
	for(i=0; i<10; i++);		
	*p = (col >> 8) & 0x0f; 	/* Column Address A8~A11 */
	for(i=0; i<10; i++);
	*p = page & 0xff;			/* Row Address A12~A19 */
	for(i=0; i<10; i++);
	*p = (page >> 8) & 0xff;	/* Row Address A20~A27 */
	for(i=0; i<10; i++);
	*p = (page >> 16) & 0x03;	/* Row Address A28~A29 */
	for(i=0; i<10; i++);
}


/* ¶ÁÈ¡Êý¾Ý */
static unsigned char s3c2440_read_data(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
    return *p;
}


/* ÔÚµÚÒ»´ÎÊ¹ÓÃNAND FlashÇ°£¬¸´Î»Ò»ÏÂNAND Flash */
static void nand_reset(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
		s3c2410_nand_reset();
    }
	else
	{
		s3c2440_nand_reset();
	}
}

static void wait_idle(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
		s3c2410_wait_idle();
    }
	else
	{
		s3c2440_wait_idle();
	}
}

static void nand_select_chip(void)
{
    int i;
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
    	s3c2410_nand_select_chip();
    }
	else
	{
    	s3c2440_nand_select_chip();
	}
    for(i=0; i<10; i++);
}

static void nand_deselect_chip(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
	    s3c2410_nand_deselect_chip();
    }
	else
	{
	    s3c2440_nand_deselect_chip();
	}
}

static void write_cmd(int cmd)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
	    s3c2410_write_cmd(cmd);
    }
	else
	{
	    s3c2440_write_cmd(cmd);
	}
}
static void write_addr(unsigned int addr)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
	    s3c2410_write_addr(addr);
    }
	else
	{
#ifdef LARGER_NAND_PAGE
	    s3c2440_write_addr_lp(addr);
#else
		s3c2440_write_addr(addr);
#endif
	}
}

static unsigned char read_data(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
	    return s3c2410_read_data();
    }
	else
	{
	    return s3c2440_read_data();
	}
}


/* ³õÊ¼»¯NAND Flash */
void nand_init(void)
{
	S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

#define TACLS   0
#define TWRPH0  3
#define TWRPH1  0

    /* ÅÐ¶ÏÊÇS3C2410»¹ÊÇS3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
//        nand_chip.nand_reset         = s3c2410_nand_reset;
//        nand_chip.wait_idle          = s3c2410_wait_idle;
//        nand_chip.nand_select_chip   = s3c2410_nand_select_chip;
//        nand_chip.nand_deselect_chip = s3c2410_nand_deselect_chip;
//        nand_chip.write_cmd          = s3c2410_write_cmd;
//        nand_chip.write_addr         = s3c2410_write_addr;
//        nand_chip.read_data          = s3c2410_read_data;

		/* Ê¹ÄÜNAND Flash¿ØÖÆÆ÷, ³õÊ¼»¯ECC, ½ûÖ¹Æ¬Ñ¡, ÉèÖÃÊ±Ðò */
        s3c2410nand->NFCONF = (1<<15)|(1<<12)|(1<<11)|(TACLS<<8)|(TWRPH0<<4)|(TWRPH1<<0);
    }
    else
    {
//        nand_chip.nand_reset         = s3c2440_nand_reset;
//        nand_chip.wait_idle          = s3c2440_wait_idle;
//        nand_chip.nand_select_chip   = s3c2440_nand_select_chip;
//        nand_chip.nand_deselect_chip = s3c2440_nand_deselect_chip;
//        nand_chip.write_cmd          = s3c2440_write_cmd;
#ifdef LARGER_NAND_PAGE
//        nand_chip.write_addr         = s3c2440_write_addr_lp;
#else
//		nand_chip.write_addr		 = s3c2440_write_addr;
#endif
//        nand_chip.read_data          = s3c2440_read_data;

		/* ÉèÖÃÊ±Ðò */
        s3c2440nand->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
        /* Ê¹ÄÜNAND Flash¿ØÖÆÆ÷, ³õÊ¼»¯ECC, ½ûÖ¹Æ¬Ñ¡ */
        s3c2440nand->NFCONT = (1<<4)|(1<<1)|(1<<0);
    }
    
    /* ¸´Î»NAND Flash */
    nand_reset();
}


/* ¶Áº¯Êý */
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
    int i, j;

#ifdef LARGER_NAND_PAGE
    if ((start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP)) {
        return ;    /* µØÖ·»ò³¤¶È²»¶ÔÆë */
    }
#else
    if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
        return ;    /* µØÖ·»ò³¤¶È²»¶ÔÆë */
    }
#endif	

    /* Ñ¡ÖÐÐ¾Æ¬ */
    nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* ·¢³öREAD0ÃüÁî */
      write_cmd(0);

      /* Write Address */
      write_addr(i);
#ifdef LARGER_NAND_PAGE
      write_cmd(0x30);		
#endif
      wait_idle();

#ifdef LARGER_NAND_PAGE
      for(j=0; j < NAND_SECTOR_SIZE_LP; j++, i++) {
#else
	  for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
#endif
          *buf = read_data();
          buf++;
      }
    }

    /* È¡ÏûÆ¬Ñ¡ÐÅºÅ */
    nand_deselect_chip();
    
    return ;
}

