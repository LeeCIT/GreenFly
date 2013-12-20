

// Graphical functions

#pragma once



void setClipRect( float zoomMagFactor=1.0f );

void setViewportAspectCorrect( int windowSizeX, int windowSizeY );



void drawCircle       ( bool outline=false );
void drawRing         ( float thickness    );
void drawTriangle     ( bool outline=false );
void drawRightTriangle( bool outline=false );
void drawSquare       ( bool outline=false );
void drawTrapezoid    ( bool outline=false );


void drawCog( int teeth, float cogProportion );

void drawBackgroundGradient();
void drawBackgroundCogs();

void drawCollisionCircles();