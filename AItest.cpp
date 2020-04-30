DWORD pLocalPlayer = { 0x509B74 };
DWORD playerObjectAddress = 0;

// Base address of "client.dll" (int or DWORD)
int ClientDLL = (int)GetModuleHandleA("client.dll");

// Find actual EntityBase address
EntityBase = *(int*)(ClientDLL + 0x9D3C6C); 

// Read the memory address of the EntityBase+0x23C4 to find the ID of the entity in the crosshair
int in_cross = *(int*)(EntityBase + 0x23C4); 
 
// ReadProcessMemory reads memory from a given process
ReadProcessMemory(handleToGame, (LPCVOID)addressToRead, &variableToStoreReadInformation, sizeof(variableToStoreReadInformation), NULL);

// WriteProcessMemory writes memory to a given process
WriteProcessMemory(handleToGame, (LPVOID)addressToWriteTo, &variableContainingValueToWrite, sizeof(variableContainingValueToWrite), NULL);

