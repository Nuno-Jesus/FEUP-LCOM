#include "graphics_card.h"

static void *video_mem;  /* frame-buffer VM address(static global variable)*/ 
static mmap_t mem_map;

struct minix_mem_range mr, m2;

unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */

unsigned int hres;
unsigned int vres;
unsigned int bpp;   //Bytes per pixel

vbe_mode_info_t mode_info;		    

int r;


int (vbe_get_mode_intel)(uint16_t mode, vbe_mode_info_t * mode_info) {
  lm_alloc(sizeof(vbe_mode_info_t), &mem_map);
  
  reg86_t reg;
  memset(&reg,0,sizeof(reg));

  reg.ax = 0x4F01; /* vbe get mode info */
  /* translate the buffer linear adress to a far pointer */
  reg.es = PB2BASE(mem_map.phys); /* set a segment base */
  reg.di = PB2OFF(mem_map.phys); /* set the offset accordingly */
  reg.cx = mode;
  reg.intno = 0x10;

  if (sys_int86(&reg) != OK)
    return 1; 

  memcpy(mode_info, mem_map.virt, mem_map.size);
  lm_free(&mem_map);

  return 0;
}


int (set_graphics_card_mode)(uint16_t mode) {
  if (vbe_get_mode_intel(mode, &mode_info) != OK) 
    return 1;


  vram_base = mode_info.PhysBasePtr;
  vram_size = mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel + 7) / 8;
  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  bpp = (mode_info.BitsPerPixel + 7) / 8;


  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  reg86_t r86;

  memset(&r86, 0, sizeof(reg86_t));	/* zero the structure */

  r86.ax = 0x4F02;
  r86.bx = 1<<14|mode; // set bit 14: linear framebuffer
  r86.intno = 0x10;

  if (sys_int86(&r86) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}


int (paint_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t * ptr;
  ptr = (uint8_t*)video_mem + (x * bpp) + (y * hres * bpp);

  for (unsigned int i = 0; i < bpp; i++) {
    *(ptr + i) = color & 0x000000FF;  //32 bits to 8 bits
    color = color >> 8;
  }

  return 0;
}


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned int i = 0 ; i < len ; i++) {
    if(paint_pixel(x + i, y, color) != OK) 
      return 1;

  }
  return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (unsigned int i = 0 ; i < height ; i++) {
    if(vg_draw_hline(x, y + i, width, color) != OK) 
      return 1;

  }
  return 0;
}
