/* copyright 1993  - Symantec Corporation */
/* wrapper for the two different MFC header files*/
#if __SC__ || __RCC__
#pragma once
#endif

#if defined(_WIN32)
#include <32-bit\afx.h>
#else
#include <16-bit\afx.h>
#endif


