/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


#define WIDTH   80
#define HEIGHT  80


/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];


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
           i >= WIDTH || j >= HEIGHT )
        continue;

      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}


void
show_image( void )
{
  int  i, j;


  for ( i = 0; i < HEIGHT; i++ )
  {
  	printf("%02d", i);
    for ( j = 0; j < WIDTH; j++ )
      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '+'
                                                    : '*' );
    putchar( '\n' );
  }
}


int
main( int     argc,
      char**  argv )
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;

  char*         filename;
  char*         text;

  double        angle;
  int           target_height;
  int           n, num_chars;

  //int chinese_str[] = {0x97e6, 0x4e1c, 0x5c71, 0x0067};
  //char *str = "Î¤¶«É½";
  wchar_t *chinese_str = L"Î¤gif";
  unsigned int *p = (wchar_t *)chinese_str;
  int i;
  FT_BBox bbox;
  FT_Glyph  glyph;

	printf("Uniocde: \n");
  for (i = 0; i < wcslen(chinese_str); i++)
  {
  	printf("0x%x ", p[i]);
  }
  printf("\n");
  //return 0;

  if ( argc != 2 )
  {
    fprintf ( stderr, "usage: %s font\n", argv[0] );
    exit( 1 );
  }

  filename      = argv[1];                           /* first argument     */
  //text          = argv[2];                           /* second argument    */
  //num_chars     = strlen( text );
  angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = HEIGHT;

  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */

  error = FT_New_Face( library, argv[1], 0, &face ); /* create face object */
  /* error handling omitted */

#if 0
  /* use 50pt at 100dpi */
  error = FT_Set_Char_Size( face, 50 * 64, 0,
                            100, 0 );                /* set character size */

	/* pixels = 50 /72 * 100 = 69  */
#else
	FT_Set_Pixel_Sizes(face, 24, 0);
#endif
  /* error handling omitted */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (0,40) relative to the upper left corner  */
  pen.x = 0 * 64;
  pen.y = ( target_height - 40 ) * 64;

  for ( n = 0; n < wcslen(chinese_str); n++ )
  {
    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, chinese_str[n], FT_LOAD_RENDER );
    if ( error )
      continue;                 /* ignore errors */

    error = FT_Get_Glyph( face->glyph, &glyph );
	if (error)
	{
		printf("FT_Get_Glyph error!\n");
		return -1;
	}

    FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox );

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top );

	printf("Unicode: 0x%x\n", chinese_str[n]);
	printf("origin.x/64 = %d, origin.y/64 = %d\n", pen.x/64, pen.y/64);
	printf("xMin = %d, xMax = %d, yMin = %d, yMax = %d\n", bbox.xMin, bbox.xMax, bbox.yMin, bbox.yMax);
    printf("slot->advance.x/64 = %d, slot->advance.y/64 = %d\n", slot->advance.x/64, slot->advance.y/64);

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  show_image();

  FT_Done_Face    ( face );
  FT_Done_FreeType( library );

  return 0;
}

/* EOF */
