#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define PIN 17

int main(int argc, char* argv[])
{
	gpioInitialise();
	gpioSetMode(PIN, PI_OUTPUT);
	gpioWrite(PIN, 1);//LED ON
	getchar();//Waits for Enter key
	gpioWrite(PIN, 0); //LED OF
	getchar();//Waits for Enter key
	// gpioTerminate();
	// return 0;
	
	
	if(gpioInitialise() < 0) {
	fprintf(stderr, "Error: pigpio initialization failed. Did you run with sudo?\n");
	return 1;	
		}
	
	gpioWrite(PIN, 0);
	
	double min_temp = 68.0;
	double max_temp = 76.0;
	
	double total_temp = 0.0;
	int reading_count = 0;
	
	FILE* log_file = fopen("temperature_log.txt", "w");
	if(log_file == NULL){
		fprintf(stderr, "Error: cold not open temperature_log.txt for writing.\n");
		gpioTerminate();
		return 1;
		
		}
		
	fprintf(log_file, " Temperature(F)\tSystem Status\tWarning LED\n");
	fprintf(log_file, " --------------------------------------------\n");
	
	int choice = 0;
	while(1) {
		printf("\n=== Smart Classroom Comfort Monitor ===\n");
		printf("Current Accepetable Range: %.1f F - %.1f F\n", min_temp, max_temp);
		printf("1. Enter classroom temperature reading\n");
		printf("2. Change acceptable temperature range\n");
		printf("3. View average temperatre and exit\n");
		printf("Select an option (1-3): ");
		
		if (scanf("%d", &choice) != 1){
			printf("Invalid input. Please enter a number (1-3).\n");
			while(getchar() != '\n');
			continue;
			}
		if(choice == 1){
			double temp;
			printf("Enter classroom temperature in Fahrenheit: ");
			if(scanf("%lf", &temp) != 1){
				printf("Invalid temperature input.\n");
				while(getchar() != '\n');
				continue;
				}
		
		total_temp += temp;
		reading_count++;
		
		if(temp >= min_temp && temp <= max_temp){
			gpioWrite(PIN, 0); //LED OFF
			printf("\nTemperature: %.1f F | System Status: NORMAL | Warning LED: OFF\n", temp);
			fprintf(log_file, "%.1f F\t\tWARNING\t\tOFF\n", temp);
			}	
			else{
				gpioWrite(PIN, 1); //LED ON
				printf("\nTmeperature: %.1f F | System Status: WARNING | Waning LED: ON\n", temp);
				fprintf(log_file, "%.1f F\t\tWARNING\t\tON\n", temp);	
				}
				fflush(log_file);
			}
			else if(choice == 2){
				double new_min, new_max=0;
				printf("Enter new minimum acceptable temperature (F): ");
				if(scanf("%lf", &new_min) != 1){
					printf("Invalid input.\n");
					while(getchar() != '\n');
					continue;
					}
				
				if(new_min <= new_max){
					min_temp = new_min;
					max_temp = new_max;
					printf("Acceptable range updated to %.1f F - %.1f F.\n", min_temp, max_temp);
					fprintf(log_file, "[Range Updated: %.1f F - %.1f F.]\n", min_temp, max_temp);
					fflush(log_file);
				}
				else{
						printf("Error: Minimum temperature cannot be greater than maximum temperature.\n");
					}
				}else{
			if(choice == 3){
				break;
				}
				else{
					printf("Invalid option. Please choose 1, 2, or 3.\n");
					}
		
			
			}
	}		
		
			if(reading_count > 0){
				double average = total_temp / reading_count;
				printf("\nTotal Readings: %d\n",reading_count);
				printf("Average Recorded Temperature: %.2f F\n", average);
				fprintf(log_file, "------------------------------------------------\n");
				fprintf(log_file, "Average Recorded Temperature: %.2f F\n", average);
				}
				else{
					printf("\nNo temperature readings were recorded.\n");
					
					}
				
				fclose(log_file);
				gpioWrite(PIN, 0);
				gpioTerminate();
				printf("Data saved to temperature_log.txt. Exiting...\n");
		
		
		
		
	
	
	return 0;


}

