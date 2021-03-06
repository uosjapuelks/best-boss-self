/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "commonHeads.h"
#include "bossDriver.h"
#include "bossVoice.h"
#include "bossLED.h"
#include "bossBrain.h"
#include "bossUART.h"


//osSemaphoreId_t bossBrain;			//semaphore id

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

int statusUpdate = 0;
void handleConnection(uint8_t option) {
	statusUpdate = (option == WIFI_CONNECT) ? 1 : 0;
}

void bBrain (void *argument) {
  // ...
  for (;;) {
		osSemaphoreAcquire(bossBrain,osWaitForever);
		
		uint8_t request_t, option_n;
		request_t = getType();
		option_n = getOption();
		
		switch (request_t) {
			case DEFAULT_FUNCTIONS:
				handleAutoSwitch(option_n);
				if (option_n == AUTOSEMAPHORE) {
					osSemaphoreRelease(bossAuto);
				}
				handleConnection(option_n);
				//Connect wifi to board (green led flash) & Auto Mode
				//Auto: 0x02 ; StopAuto: 0x03 ; Connect: 0x01
				break;
			case DRIVEMODES:
				rewrite_driveMode(option_n);
				break;
			case DIRECTION_OPTIONS:
				rewrite_direction(option_n);
				break;
			case AUDIO_CONTROL:
				audio_choice = option_n;
				break;
			default:
				//flashRED;
				stop();
				break;
		}
		osSemaphoreDelete(bossBrain);
		
		//If auto mode is off dont release user drive
		if (auto_modeOn == 0) {
			osSemaphoreRelease(bossDrive);
		}
	}
}

void bDrive (void *arg) {
	for (;;) {
		osSemaphoreAcquire(bossDrive, osWaitForever);
		executeDrive();
	}
}

void bAuto (void *arg) {
	for (;;) {
		osSemaphoreAcquire(bossAuto, osWaitForever);
		driverless_mode();
	}
}

void bAudio (void *arg) {
	for(;;) {
		switch(audio_choice) {
			case SILENCE:
				stop_music();
				break;
			case CHALLENGE_TIME:
				play_moving_song();
				break;
			case ENDING_TIME:
				play_end_song();
				break;
			case WIFI_CONNECT:
				play_wifi_song();
				break;
		}
	}
}

void bGreenFront(void *arg) {
	for(;;) {
		if (statusUpdate == 1) {
			twoGreenFlash();
		}
		else if (isDriving() == 1) {
			runFrontGreenLED();
		}
		else if (isDriving() == 0) {
			onFrontGreenLED();
		}
	}
}

void bRedFront(void *arg) {
	for(;;) {
		if (isDriving() == 1) {
			movingRearRED();
		}
		else if (isDriving() == 0) {
			stopRearRED();
		}
	}
}
 
int main (void) {
 
  // System Initialization
	SystemCoreClockUpdate();
	
	initUART2();
	InitRGB();
	initFrontGreenLEDGPIO();
	initRearRedLEDGPIO();
	InitMotor();
	InitAudio();
	
	stop();
	offRGB();
	offRearRedLED();
	offFrontGreenLED();
	
  // ...
	
	bossBrain = osSemaphoreNew(1,0,NULL);
	bossAuto = osSemaphoreNew(1,0,NULL);
 
	osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(bBrain, NULL, NULL);    // Create application brain thread
	osThreadNew(bDrive, NULL, NULL);		// Create application drive thread
	osThreadNew(bAudio, NULL, NULL);    // Create application brain thread
	osThreadNew(bRedFront, NULL, NULL);		// Create application drive thread
	osThreadNew(bGreenFront, NULL, NULL);    // Create application brain thread
	osThreadNew(bAuto, NULL, NULL);		// Create application drive thread
	osKernelStart();                      // Start thread execution
  for (;;) {}
}
