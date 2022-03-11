
#include <CppLinuxSerial/SerialPort.hpp>
#include <stdio.h>
#include <string>
#include <time.h>
#include <iostream>
#include <ctime>
#include <unistd.h>

using namespace mn::CppLinuxSerial;

char _device_port[12];
char* get_device_name(void)
{
	// Compare the output of this program to running "ps aux" on the
	// command line.
	FILE *in = popen("sudo ls /dev/ttyACM*", "r");
	if(in == NULL)
	{
		perror("popen:");
		exit(EXIT_FAILURE);
	}

	// Write the output of the command to the console.
	while(fgets(_device_port, sizeof(_device_port), in) != NULL)
		printf("%s", _device_port);


	printf("\n\n");
	
	int pcloseRetVal = pclose(in); // wait for process to finish.
	
	return _device_port;
}

int main() {

	// Create serial port object and open serial port at 57600 buad, 8 data bits, no parity bit, and one stop bit (8n1)
    const char *device_name= get_device_name();
    // std::cout<<"Found a Device : "<<device_name<<std::endl;
    // std::string dev= "/dev/ttyACM1";
    // std::cout<<dev.length()<<std::endl;
    SerialPort serialPort("/dev/ttyACM1", BaudRate::B_115200, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
	// Use SerialPort serialPort("/dev/ttyACM0", 13000); instead if you want to provide a custom baud rate
	serialPort.SetTimeout(-1); // Block when reading until any data is received
	serialPort.Open();
	
	bool running=true;
    //Continuously recieve data 
    while(running){
    // Read some data back (will block until at least 1 byte is received due to the SetTimeout(-1) call above)

        std::string readData;
        serialPort.Read(readData);
        time_t curr_time;
	    curr_time = time(NULL);
	    char *tm = ctime(&curr_time);
	    std::cout << "Check In Time : " << tm;
        std::cout<<"Data Recieved from Barcode Scanner"<<readData<<std::endl;
        usleep(10000000/8);
        std::cout<<std::endl;
    	readData.clear();
    }
    
	// Close the serial port
	serialPort.Close();
}
