#pragma once

#include <iostream>

#define INNO_ERROR   std::cout << "\033[1m\033[31m"  // bold red
#define INNO_WARNING std::cout << "\033[1m\033[33m"  // bold yellow
#define INNO_INFO    std::cout << "\033[1m\033[32m"  // bold green
#define INNO_INFOL   std::cout << "\033[32m"         // green
#define INNO_DEBUG   std::cout << "\033[1m\033[36m"  // bold cyan
#define INNO_REND    "\033[0m" << std::endl

#define INNO_TITLE   std::cout << "\033[1m\033[35m"  // bold magenta
#define INNO_MSG     std::cout << "\033[1m\033[37m"  // bold white