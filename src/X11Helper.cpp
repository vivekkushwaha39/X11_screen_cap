/*
 * X11Helper.cpp
 *
 *  Created on: 28-Jan-2019
 *      Author: VivekK4
 */

#include "X11Helper.h"
#include <png.h>
#include <linux/input.h>
#include "debug.h"
using namespace std;
// TODO: send touch event on X=78 and Y=253


X11Helper::X11Helper() {
	/*display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);

	XWindowAttributes attributes = {0};
	XGetWindowAttributes(display, root, &attributes);
	printf("Display is  %dX%d\n",attributes.width, attributes.height);
	printf("Sleeping for 15 sec");
	sleep(15);

	XImage *pImage = NULL;
	pImage = XGetImage(display, root, 0, 0, attributes.width, attributes.height,AllPlanes, ZPixmap);

	XCloseDisplay(display);*/
	display = NULL;
	pXImg = NULL;
	isOpened = false;
}

X11Helper::~X11Helper()
{
	// TODO: vivekk4: delete screen pointer if it is allocated
}


bool X11Helper::OpenDisplay()
{
	if ( GetStatus() == DISPLAY_OPENED )
	{
		syslog(LOG_DEBUG, "display is already opened");
		return true;
	}
	syslog(LOG_DEBUG, "opening display");
	isOpened = true;
	display = XOpenDisplay(NULL);

	if ( display == NULL )
	{
		syslog(LOG_DEBUG, "Unable to open display");
		isOpened = false;
	}
	else
	{
		rootWindowDrawable =  DefaultRootWindow(display);
		XGetWindowAttributes(display,rootWindowDrawable, &rootWindowAttr);
	}

	syslog(LOG_DEBUG, "Display is %d", display);
	return isOpened;
}

bool X11Helper::CaptureScreen()
{

	syslog(LOG_DEBUG, "capturing screen");
	if ( display == NULL )
	{
		syslog(LOG_DEBUG, "display is null not able to capture");
	}

	bool isCaptured = true;
	if ( pXImg !=  NULL )
	{
		XFree(pXImg);
	}

	pXImg = XGetImage(display, rootWindowDrawable, 0, 0, rootWindowAttr.width, rootWindowAttr.height,
			AllPlanes,ZPixmap );
	isCaptured = true;

	syslog(LOG_DEBUG, "end capturing screen");
	return isCaptured;
}


bool X11Helper::SaveAsPng(string fileName)
{
	bool status = true;

	syslog(LOG_DEBUG, "saving screen");
	if ( pXImg == NULL )
	{
		// Screen was not captured need to allocate pXimg first
		syslog(LOG_DEBUG, "image is null not saving");
		return false;
	}

	if ( fileName.compare("") == 0 )
		fileName = "flash/screencap.png";

	bool isSaved = false;

	if( pXImg != NULL )
	{
		unsigned long red_mask = pXImg->red_mask;
		unsigned long green_mask = pXImg->green_mask;
		unsigned long blue_mask = pXImg->blue_mask;


		cout<<"Image mask "<<pXImg->bits_per_pixel;

		// Initialize PNG
		png_structp pngPtr = NULL;
		png_infop pngInfoPtr = NULL;
		pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);




		if ( pngPtr == NULL )
		{
			// Unable to allocate mem
			return false;
		}
		pngInfoPtr = png_create_info_struct(pngPtr);


		if (setjmp(png_jmpbuf(pngPtr)))
		{
			png_destroy_write_struct(&pngPtr, &pngInfoPtr);
			return false;
		}


		FILE *outFile = fopen(fileName.c_str(), "wb");
		if ( !outFile )
		{
			cout<<"Unable to open file "<<fileName.c_str();
			png_destroy_write_struct(&pngPtr, &pngInfoPtr);
			return false;
		}
		cout<<"File is created"<<endl;
		png_set_IHDR (pngPtr, pngInfoPtr, rootWindowAttr.width, rootWindowAttr.height,
		     8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);


		png_init_io(pngPtr, outFile);
		cout<<"Png initio completed"<<endl;
		char *title = "ScreenShot";
		png_text pngText;
		pngText.compression = PNG_TEXT_COMPRESSION_NONE;
		pngText.key = "Title";
		pngText.text = "Screencap" ;
		pngText.text_length = 9;
		png_set_text(pngPtr, pngInfoPtr, &pngText, 1);
		png_write_info(pngPtr, pngInfoPtr);

		cout<<"Info is written"<<endl;

		png_bytepp png_row_col = (png_bytepp) png_malloc(pngPtr, rootWindowAttr.height * sizeof (png_bytep));
		for ( int x = 0; x < rootWindowAttr.height; x++ )
		{
			png_bytep png_row = (png_bytep)png_malloc (pngPtr, sizeof (uint8_t) * rootWindowAttr.width * 24 );

			if ( png_row == NULL )
			{
				SET_STATUS_BREAK(status)
			}

			png_row_col[x] = png_row;

			for ( int y = 0 ; y <rootWindowAttr.width; y++)
			{

				unsigned long pixel = XGetPixel(pXImg,y,x);
				unsigned char blue = pixel & blue_mask;
				unsigned char green = (pixel & green_mask) >> 8;
				unsigned char red = (pixel & red_mask) >> 16;

				png_byte *ptr = &(png_row[y*3]);
				ptr[0] = red;
				ptr[1] = green;
				ptr[2] = blue;
			}

		}


		cout<<"Converted data to PNG pixel"<<endl;

		if ( status == false)
		{
			for ( int y = 0; y < rootWindowAttr.height; y++ )
			{
				png_free (pngPtr, png_row_col[y]);
			}

			if (pngInfoPtr != NULL) png_free_data (pngPtr, pngInfoPtr, PNG_FREE_ALL, -1);
			if (pngPtr != NULL) png_destroy_write_struct (&pngPtr, (png_infopp)NULL);
			return false;
		}

		png_set_rows(pngPtr, pngInfoPtr, png_row_col);
		png_write_png(pngPtr, pngInfoPtr, PNG_TRANSFORM_IDENTITY, NULL);


		for ( int y = 0; y < rootWindowAttr.height; y++ )
		{
			png_free (pngPtr, png_row_col[y]);
		}
		png_free (pngPtr, png_row_col);

		fclose (outFile);
		cout<<"closing file "<<endl;
		if (pngInfoPtr != NULL) png_free_data (pngPtr, pngInfoPtr, PNG_FREE_ALL, -1);
		if (pngPtr != NULL) png_destroy_write_struct (&pngPtr, (png_infopp)NULL);

	}
	syslog(LOG_DEBUG, "end saving screen");
	return isSaved;
}


int X11Helper::GetStatus()
{
	int status = UNINITIALIZED;

	if ( display != NULL )
	{
		status = DISPLAY_OPENED;
	}

	syslog(LOG_DEBUG, "status is %d", status);
	return status;
}

bool X11Helper::CloseDisplay()
{
	if ( pXImg != NULL )
		XFree(pXImg);
	if ( display != NULL )
		XFree(display);

	return true;
}
