#include "System.h"
namespace Noelle
{
	TCHAR System::ms_sLogBuffer[LOG_BUFFER_SIZE];
	DWORD System::ms_dwMainThreadID = 0;
}