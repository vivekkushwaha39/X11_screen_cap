/*
 * Main.cpp
 *
 *  Created on: Jan 31, 2019
 *      Author: vivek
 */
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <png.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#include "ScreenShareApp.h"

using namespace std;
/*int main(int argc, char **argv) {
	Display * display1 = NULL;
	XWindowAttributes  winAttribute;

	display1 = XOpenDisplay(NULL);

	if ( display1 == NULL)
		return -1;

	Window root = RootWindow(display1, DefaultScreen(display1));
	XGetWindowAttributes(display1, root, &winAttribute);
	cout<<winAttribute.width<<"X"<<winAttribute.height;
	cout<<"sleeping for 10 sec."<<endl;
	sleep(10);
	XImage *img = XGetImage(display1, root, 0, 0,
			winAttribute.width,
			winAttribute.height, AllPlanes, ZPixmap);

	unsigned long red_mask = img->red_mask;
	unsigned long green_mask = img->green_mask;
	unsigned long blue_mask = img->blue_mask;


	cout<<"Image mask "<<img->bits_per_pixel;

	// Initialize PNG
	png_structp pngPtr = NULL;
	png_infop pngInfoPtr = NULL;
	pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);




	if ( pngPtr == NULL )
	{
		XCloseDisplay(display1);
		return -1;
	}
	pngInfoPtr = png_create_info_struct(pngPtr);


	if (setjmp(png_jmpbuf(pngPtr))) {
		XCloseDisplay(display1);
		png_destroy_write_struct(&pngPtr, &pngInfoPtr);
		return -2;
	}

	FILE *outFile = fopen("Screen.png", "wb");

	png_set_IHDR (pngPtr, pngInfoPtr, winAttribute.width, winAttribute.height,
	     8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
	     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);


	png_init_io(pngPtr, outFile);

	char *title = "ScreenShot";
	png_text pngText;
	pngText.compression = PNG_ITXT_COMPRESSION_NONE;
	pngText.key = "Title";
	pngText.text = title;
	png_set_text(pngPtr, pngInfoPtr, &pngText, 1);
	png_write_info(pngPtr, pngInfoPtr);


	png_bytepp png_row_col = (png_bytepp) png_malloc(pngPtr, winAttribute.height * sizeof (png_bytep));
	for ( int x = 0; x < winAttribute.height; x++ ){
		png_bytep png_row = (png_bytep)png_malloc (pngPtr, sizeof (uint8_t) * winAttribute.width * 24 );

		png_row_col[x] = png_row;

		for ( int y = 0 ; y <winAttribute.width; y++){

			unsigned long pixel = XGetPixel(img,y,x);
			unsigned char blue = pixel & blue_mask;
			unsigned char green = (pixel & green_mask) >> 8;
			unsigned char red = (pixel & red_mask) >> 16;

			png_byte *ptr = &(png_row[y*3]);
			ptr[0] = red;
			ptr[1] = green;
			ptr[2] = blue;
		}

	}
	png_set_rows(pngPtr, pngInfoPtr, png_row_col);
	png_write_png(pngPtr, pngInfoPtr, PNG_TRANSFORM_IDENTITY, NULL);

	for (int y = 0; y < winAttribute.height; y++) {
		png_free (pngPtr, png_row_col[y]);
	}
	png_free (pngPtr, png_row_col);

	fclose (outFile);
	if (pngInfoPtr != NULL) png_free_data (pngPtr, pngInfoPtr, PNG_FREE_ALL, -1);
	if (pngPtr != NULL) png_destroy_write_struct (&pngPtr, (png_infopp)NULL);

	XCloseDisplay(display1);



	return 0;

}*/

void SignalHandler(int);
ScreenShareApp app;
int main(int argc, char **argv)
{
	signal(SIGILL | SIGABRT, SignalHandler);
	app.StartCapturing();

	return 0;
}

void SignalHandler(int data)
{
	syslog(LOG_DEBUG, "got sigint");
	app.Signal(data);
}
