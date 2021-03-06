

// Declarations for main.cpp

#pragma once



void evStart();
void evStep( int value );
void evDraw();
void evReshape( int newSizeX, int newSizeY );

void evKeyboardDown       ( unsigned char key, int x, int y );
void evKeyboardUp         ( unsigned char key, int x, int y );
void evKeyboardSpecialDown( int           key, int x, int y );
void evKeyboardSpecialUp  ( int           key, int x, int y );