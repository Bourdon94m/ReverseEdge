#include "MISC.h"
#include "ESP/ESP.h"
#include "utilities/Math.h"
#include <iostream>


uintptr_t moduleBaseAddressMISC = (uintptr_t)GetModuleHandle("ac_client.exe");
uintptr_t localPlayerMISC = (uintptr_t)moduleBaseAddressMISC + 0x0018AC00;



//This method will make the player jump constantly 

void MISC::BunnyHop()
{

	std::cout << "Value : " << *(float*)(localPlayerMISC + 0x18) << std::endl;
	
	if (*(float*)(localPlayerMISC + 0x18) <= 0.0f)
	{
		*(float*)(localPlayerMISC + 0x18) = 1.4; // Writing memory with the new value
		std::cout << "New Bunny : " << *(float*)(localPlayerMISC + 0x18) << std::endl;
		std::cout << "Address of Bunny : " << &*(float*)(localPlayerMISC + 0x18) << std::endl;
	}
	

	Sleep(30); // more fluid and don't block the CPU

	
	
	
}
