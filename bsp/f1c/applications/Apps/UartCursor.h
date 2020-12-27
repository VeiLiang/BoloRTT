#ifndef __UartCursor_h
#define __UartCursor_h
typedef struct
{
	unsigned int State;//1=pressed
	unsigned int Px;
	unsigned int Py;
}UartCursorDataStruct;
extern UartCursorDataStruct UartCursorData;
void UartCursor_Init(void);
#endif
