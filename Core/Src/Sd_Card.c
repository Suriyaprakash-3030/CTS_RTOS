#include "fatfs.h"

//#define MAX_APPS 3 // Define the maximum number of applications

//volatile uint32_t appElapsedSeconds[MAX_APPS] = {0}; // Time tracked for each app during this session
//uint64_t appCumulativeSeconds[MAX_APPS] = {0};      // Total cumulative time for each app (saved + current)

/*  File Operation   */
FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

char work[512];     // Work area for formatting

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];  // to store strings..

int i=0;

int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void clear_buffer (void)
{
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}

void send_uart (char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}


void sd_Card_Check(void)
{
	 fresult = f_mount(&fs, "/", 1);
	  	if (fresult != FR_OK){
				send_uart ("ERROR!!! in mounting SD CARD...\n\n");
				if (f_mkfs("", FM_FAT32, 0, work, sizeof(work)) != FR_OK) {
					// Handle format error
				}
				fresult = f_mount(&fs, "/", 1);
				if (fresult == FR_OK){
					send_uart ("Mounted SD CARD Sucesfully ...\n\n");
				}
	  		}
	  	else send_uart("SD CARD mounted successfully...\n\n");

	  	/*************** Card capacity details ********************/

	  	/* Check free space */
	 	f_getfree("", &fre_clust, &pfs);
	  	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	  	sprintf (buffer, "SD CARD Total Size: \t%lu\n",total);
	  	send_uart(buffer);
	  	clear_buffer();
	  	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	  	sprintf (buffer, "SD CARD Free Space: \t%lu\n\n",free_space);
	  	send_uart(buffer);
	  	clear_buffer();

	  	/************* The following operation is using PUTS and GETS *********************/

	  	/* Open file to write/ create a file if it doesn't exist */
	  	 fresult = f_open(&fil, "TimeLine.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	  	/*
	  	 f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);


	 	fresult = f_close(&fil);

	  	if (fresult == FR_OK)
	  		send_uart ("File1.txt created and the data is written \n");


	  	fresult = f_open(&fil, "TimeLine.txt", FA_READ);


	  	f_gets(buffer, f_size(&fil), &fil);

	  	send_uart("File1.txt is opened and it contains the data as shown below\n");
	    send_uart(buffer);
	  	send_uart("\n\n");
*/


	  	f_close(&fil);

}

void LoadTimeFromSDCard(uint32_t *hours, uint32_t *minutes, uint32_t *seconds) {
    FIL file;
    char time_str[16];
    FRESULT fresult;

    // Initialize time to zero in case of errors
    *hours = 0;
    *minutes = 0;
    *seconds = 0;

    /* Open file for reading */
    fresult = f_open(&file, "TimeLine.txt", FA_READ);
    if (fresult == FR_OK) {
        // Read the last saved time from the file
        UINT bytes_read;
        fresult = f_read(&file, time_str, sizeof(time_str) - 1, &bytes_read);
        if (fresult == FR_OK) {
            time_str[bytes_read] = '\0'; // Null-terminate the string
            sscanf(time_str, "%02lu:%02lu:%02lu", hours, minutes, seconds); // Parse the time
        }
        f_close(&file);
    } else {
        // Handle case where the file doesn't exist or can't be read
        // Time will remain initialized to 0:0:0
    }
}



void UpdateTimeToSDCard(uint32_t add_hours, uint32_t add_minutes, uint32_t add_seconds) {
    uint32_t current_hours, current_minutes, current_seconds;

    if(log_to_sd_flag)
    {
    	log_to_sd_flag = 0;
		// Load existing time from SD card
		LoadTimeFromSDCard(&current_hours, &current_minutes, &current_seconds);

		// Add the new time values
		uint32_t total_seconds = current_seconds + add_seconds;
		uint32_t total_minutes = current_minutes + add_minutes + (total_seconds / 60);
		uint32_t total_hours = current_hours + add_hours + (total_minutes / 60);

		// Normalize the time
		total_seconds %= 60;
		total_minutes %= 60;

		// Save the updated time back to the SD card
		char time_str[16];
		snprintf(time_str, sizeof(time_str), "%02lu:%02lu:%02lu\n", total_hours, total_minutes, total_seconds);

		FIL file;
		UINT bytes_written;
		FRESULT fresult;

		/* Open file to write/ create a file if it doesn't exist */
		fresult = f_open(&file, "TimeLine.txt", FA_CREATE_ALWAYS | FA_WRITE);
		if (fresult == FR_OK) {
			f_write(&file, time_str, strlen(time_str), &bytes_written);
			f_close(&file);
		} else {
			// Handle error (e.g., SD card not inserted or write failure)
		}
    }
}


/*
// Load cumulative time for a specific application from the SD card
void LoadAppTimeFromSD(const char *appName, int appIndex) {
	FATFS fs;  // file system
    FIL file;
    UINT bytesRead;
    char timeString[32];

    char fileName[32];
    sprintf(fileName, "%s_time.txt", appName); // Construct the file name

    f_mount(&fs, "/", 1); // Mount the SD card
    if (f_open(&file, fileName, FA_READ) == FR_OK) {
        f_read(&file, timeString, sizeof(timeString) - 1, &bytesRead);
        timeString[bytesRead] = '\0'; // Null-terminate the string
        f_close(&file);

        // Parse the saved cumulative time (in seconds)
        uint64_t hours, minutes, seconds;
        sscanf(timeString, "%llu:%llu:%llu", &hours, &minutes, &seconds);
        //appCumulativeSeconds[appIndex] = (hours * 3600) + (minutes * 60) + seconds;
    }
    //f_mount(NULL, SDPath, 1); // Unmount the SD card
}

// Load cumulative time from SD card
void LoadTimeFromSD(void) {
    FIL file;
    UINT bytesRead;
    char timeString[32];

    f_mount(&fs, "/", 1); // Mount the SD card
    if (f_open(&file, "applications_time.txt", FA_READ) == FR_OK) {
        f_read(&file, timeString, sizeof(timeString) - 1, &bytesRead);
        timeString[bytesRead] = '\0'; // Null-terminate the string
        f_close(&file);

        // Parse the saved cumulative time
        cumulativeTime = strtoull(timeString, NULL, 10);
    }
    f_mount(NULL, SDPath, 1); // Unmount the SD card
}


// Load all application times from a single SD card file
void LoadAllAppTimesFromSD(void) {
    FIL file;

    char line[64];

    f_mount(&fs, "/", 1); // Mount the SD card
    if (f_open(&fil, "applications_time.txt", FA_READ) == FR_OK) {
        for (int i = 0; i < MAX_APPS; i++) {
            if (f_gets(line, sizeof(line), &fil)) {
                // Parse the saved cumulative time (in HH:MM:SS format)
                uint64_t hours, minutes, seconds;
                sscanf(line, "App%*d: %llu:%llu:%llu", &hours, &minutes, &seconds);
                appCumulativeSeconds[i] = (hours * 3600) + (minutes * 60) + seconds;
            }
        }
        f_close(&file);
    }
    f_mount(NULL, "/", 1); // Unmount the SD card
}

// Save all application times to a single SD card file
void SaveAllAppTimesToSD(void) {

    UINT bytesWritten;
    f_mount(&fs, "/", 1); // Mount the SD card
    if (f_open(&fil, "applications_time.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
        for (int i = 0; i < MAX_APPS; i++) {
            // Update cumulative time for the application
            appCumulativeSeconds[i] += appElapsedSeconds[i];
            appElapsedSeconds[i] = 0; // Reset elapsed seconds for the next session

            // Convert to HH:MM:SS format
            uint64_t totalSeconds = appCumulativeSeconds[i];
            uint64_t hours = totalSeconds / 3600;
            totalSeconds %= 3600;
            uint64_t minutes = totalSeconds / 60;
            uint64_t seconds = totalSeconds % 60;

            char timeString[64];
            sprintf(timeString, "App%d: %02llu:%02llu:%02llu\n", i + 1, hours, minutes, seconds);
            f_write(&fil, timeString, strlen(timeString), &bytesWritten);
        }
        f_close(&fil);
    }
    f_mount(NULL, "/", 1); // Unmount the SD card
}


*/



