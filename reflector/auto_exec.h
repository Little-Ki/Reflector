#pragma once

#define STRCAT_IMPL(a, b) a##b
#define STRCAT(a, b) STRCAT_IMPL(a, b)
#define EXEC_NAME STRCAT(__exec__, __LINE__)
#define AUTO_EXEC struct EXEC_NAME { EXEC_NAME(); } EXEC_NAME; EXEC_NAME::EXEC_NAME()
