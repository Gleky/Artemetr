#pragma once

// HARDWARE PARAMETERS
#define connnectionSpeed 115200
#define xMaxPos 465
#define yMaxPos 450
#define homeX 455
#define homeY 440

#define horisontalPackCount 5
#define horisontalCellCount 4
#define verticalPackCount 3
#define verticalCellCount 6
#define packWidth 58
#define packHeight 96.3
#define leftTableMargin 14
#define botTableMargin 18
#define xSpace 27.3
#define ySpace 31

//Connection commands
#define movingCamId "MOVING_CAM"
#define connectRequest "CONNECT_TO_MOVING_CAM"
#define connectApprove "CONNECTION_SUCCESS"
#define closeConnection "CLOSE"


/////Position messages////
#define currentCamPosition 'P'
#define moveCamTo 'M'

#define commandSize 5
#define commandIndex 0
#define xPosIndex 1
#define yPosIndex 3


/////Backlight messages
#define setBacklightState 'L'
#define backlightStateIndex 1

