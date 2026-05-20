#pragma once

#include "ntstuff.h"
#include <cstdarg>
#include <intrin.h>

// divisor values
#define SERIAL_115200_BAUD 1
#define SERIAL_57600_BAUD  2
#define SERIAL_38400_BAUD  3

extern void InitializeSerial(UINT16 divisor);
extern void SetBaud(UINT16 divisor);

extern UINT8 ReadSerial();
extern void WriteSerial(const UINT8* data, SIZE_T size);
extern void DebugPrint(PCSTR msg, ...);
extern void VDebugPrint(PCSTR msg, va_list args);

extern void DebugLog(PCSTR file, int line, PCSTR msg, ...);

#ifdef _DEBUG
#define DBG2(file, line, ...) DebugLog(file, line, __VA_ARGS__)
#define DBG(...)			  DBG2(__FILE__, __LINE__, __VA_ARGS__)
#else
#define DBG(...)
#endif
