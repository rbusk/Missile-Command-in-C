/*
A simple graphics library for CSE 20211 
Originally created by Prof. Doug Thain.
Modified by Prof. Ramzi Bualuan
Last edit: Nov 28, 2014
*/

#ifndef GFX_H
#define GFX_H

#include <X11/Xlib.h>

/* Open a new graphics window. */
void gfx_open( int width, int height, const char *title );

/* Flush all previous output to the window. */
void gfx_flush();

/* Change the current drawing color. */
void gfx_color( int red, int green, int blue );

/* Clear the graphics window to the background color. */
void gfx_clear();

/* Change the current background color. */
void gfx_clear_color( int red, int green, int blue );

/* Check to see if an event is waiting. */
int gfx_event_waiting();

/* Wait for the user to press a key or mouse button. */
char gfx_wait();

/* Return the X and Y coordinates of the last event. */
int gfx_xpos();
int gfx_ypos();

/* Return the X and Y dimensions of the screen (monitor). */
int gfx_xsize();
int gfx_ysize();

/* Display a string at (x,y) */
void gfx_text( int x, int y , char *text );

/* Draw a point at (x,y) */
void gfx_point( int x, int y );

/* Draw a line from (x1,y1) to (x2,y2) */
void gfx_line( int x1, int y1, int x2, int y2 );

/* Draw a circle centered at (xc,yc) with radius r */
void gfx_circle( int xc, int yc, int r);

/* Draw an ellipse centered at (xc,yc) with radii r1 and r2 */
void gfx_ellipse( int xc, int yc, int r1, int r2);

/* Draw a rectangle with top-left corner at (x,y) with positive width w and height h */
void gfx_rectangle( int x, int y, int w, int h );

/* Draw a filled rectangle; similar to gfx_rectangle, but its size is "one pixel in" at all four sides */
void gfx_fill_rectangle( int x, int y, int w, int h );

/* Draw a polygon whose corners (num_pts of them) are in the pointsarr array (uses Xlib's XPoint struct) */
void gfx_polygon( XPoint *pointsarr, int num_pts );
 
/* Draw a triangle that goes through points (x1,y1), (x2,y2), (x3,y3) */
void gfx_triangle( int x1, int y1, int x2, int y2, int x3, int y3 );

#endif

