#ifndef _ACTION_H_
#define _ACTION_H_

void clickPassenger();
void clickVip();
void clickClose();
void clickPause(const int number);
void clickReset(const int number);

DWORD WINAPI checkpoint0(PVOID pM);
DWORD WINAPI checkpoint1(PVOID pM);
DWORD WINAPI checkpoint2(PVOID pM);
DWORD WINAPI checkpoint3(PVOID pM);
DWORD WINAPI checkpoint4(PVOID pM);
DWORD WINAPI checkpoint5(PVOID pM);
DWORD WINAPI checkpoint6(PVOID pM);

#endif
