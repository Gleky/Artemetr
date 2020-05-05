#pragma once

// HARDWARE PARAMETERS
#define connnectionSpeed 115200
#define xMaxPos 465
#define yMaxPos 450


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

