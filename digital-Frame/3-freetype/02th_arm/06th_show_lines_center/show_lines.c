#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

typedef struct TGlyph_ { 
	FT_UInt index; /* glyph index */ 
	FT_Vector pos; /* glyph origin on the baseline */ 
	FT_Glyph image; /* glyph image */ 
} TGlyph, *PGlyph; 


#define MAX_GLYPHS  100

int fd_fb;
struct fb_var_screeninfo var;	/* Current var */
struct fb_fix_screeninfo fix;	/* Current fix */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;


/* color : 0x00RRGGBB */
void lcd_put_pixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = fbmem+y*line_width+x*pixel_width;
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (var.bits_per_pixel)
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", var.bits_per_pixel);
			break;
		}
	}
}



/* Replace this function with something useful. */

void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;

	//printf("x = %d, y = %d\n", x, y);

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= var.xres || j >= var.yres )
        continue;

      //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
      lcd_put_pixel(i, j, bitmap->buffer[q * bitmap->width + p]);
    }
  }
}


int Get_Glyphs_Frm_Wstr(FT_Face face, wchar_t * wstr, TGlyph glyphs[])
{
	int n;
	PGlyph glyph = glyphs;
	int pen_x = 0;
	int pen_y = 0;
	int error;
	FT_GlyphSlot  slot = face->glyph;;
	
		
	for (n = 0; n < wcslen(wstr); n++)
	{
		glyph->index = FT_Get_Char_Index( face, wstr[n]); 
		/* store current pen position */ 
		glyph->pos.x = pen_x; 
		glyph->pos.y = pen_y;		

		/* load时是把glyph放入插槽face->glyph */
		error = FT_Load_Glyph(face, glyph->index, FT_LOAD_DEFAULT);
		if ( error ) 
			continue;

		error = FT_Get_Glyph(face->glyph, &glyph->image ); 
		if ( error ) 
			continue;

		/* translate the glyph image now */ 
		/* 这使得glyph->image里含有位置信息 */
		FT_Glyph_Transform(glyph->image, 0, &glyph->pos );

		pen_x += slot->advance.x;  /* 1/64 point */

		/* increment number of glyphs */ 
		glyph++;		
	}

	/* count number of glyphs loaded */ 
	return (glyph - glyphs);
}

void compute_string_bbox(TGlyph glyphs[], FT_UInt num_glyphs, FT_BBox *abbox )
{
	FT_BBox bbox; 
	int n;
	
	bbox.xMin = bbox.yMin = 32000; 
	bbox.xMax = bbox.yMax = -32000;

	for ( n = 0; n < num_glyphs; n++ )
	{
		FT_BBox glyph_bbox;
		
		FT_Glyph_Get_CBox(glyphs[n].image, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox );

		if (glyph_bbox.xMin < bbox.xMin)
			bbox.xMin = glyph_bbox.xMin;

		if (glyph_bbox.yMin < bbox.yMin)
			bbox.yMin = glyph_bbox.yMin;

		if (glyph_bbox.xMax > bbox.xMax)
			bbox.xMax = glyph_bbox.xMax;

		if (glyph_bbox.yMax > bbox.yMax)
			bbox.yMax = glyph_bbox.yMax;
	}

	*abbox = bbox;
}


void Draw_Glyphs(TGlyph glyphs[], FT_UInt num_glyphs, FT_Vector pen)
{
	int n;
	int error;
	
	for (n = 0; n < num_glyphs; n++)
	{
		FT_Glyph_Transform(glyphs[n].image, 0, &pen);
		/* convert glyph image to bitmap (destroy the glyph copy!) */ 
		error = FT_Glyph_To_Bitmap(&glyphs[n].image, FT_RENDER_MODE_NORMAL, 0, /* no additional translation */ 
                              		1 ); 		/* destroy copy in "image" */
		if ( !error ) 
		{ 
			FT_BitmapGlyph bit = (FT_BitmapGlyph)glyphs[n].image; 
			draw_bitmap(&bit->bitmap, bit->left, var.yres - bit->top); 
			FT_Done_Glyph(glyphs[n].image ); 
		}
	}
}

int main(int argc, char **argv)
{
	wchar_t *wstr1 = L"百问网gif";
	wchar_t *wstr2 = L"www.100ask.net";

	FT_Library	  library;
	FT_Face 	  face;
	int error;
    FT_Vector     pen;
	FT_GlyphSlot  slot;
	int i;
	FT_BBox bbox;

	int line_box_ymin = 10000;
	int line_box_ymax = 0;

	int line_box_width;
	int line_box_height;

	TGlyph glyphs[MAX_GLYPHS]; /* glyphs table */ 
	FT_UInt num_glyphs;


	if (argc != 2)
	{
		printf("Usage : %s <font_file>\n", argv[0]);
		return -1;
	}
		

	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}

	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}

	if (ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix))
	{
		printf("can't get fix\n");
		return -1;
	}

	line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
	fbmem = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fbmem == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}

	/* 清屏: 全部设为黑色 */
	memset(fbmem, 0, screen_size);

	/* 显示矢量字体 */
	error = FT_Init_FreeType( &library );			   /* initialize library */
	/* error handling omitted */
	
	error = FT_New_Face( library, argv[1], 0, &face ); /* create face object */
	/* error handling omitted */	
	slot = face->glyph;

	FT_Set_Pixel_Sizes(face, 24, 0);

	/* wstr1 */
	num_glyphs = Get_Glyphs_Frm_Wstr(face, wstr1, glyphs);
	
	compute_string_bbox(glyphs, num_glyphs, &bbox);
	line_box_width  = bbox.xMax - bbox.xMin;
	line_box_height = bbox.yMax - bbox.yMin;

	pen.x = (var.xres - line_box_width)/2 * 64;
	pen.y = (var.yres - line_box_height)/2 * 64;

	Draw_Glyphs(glyphs, num_glyphs, pen);

	/* wstr2 */
	num_glyphs = Get_Glyphs_Frm_Wstr(face, wstr2, glyphs);

	compute_string_bbox(glyphs, num_glyphs, &bbox);
	line_box_width  = bbox.xMax - bbox.xMin;
	line_box_height = bbox.yMax - bbox.yMin;

	pen.x = (var.xres - line_box_width)/2 * 64;
	pen.y = pen.y - 24 * 64;
	Draw_Glyphs(glyphs, num_glyphs, pen);


	return 0;	
}

