#include "IO/VerbosityControl.h"

bool g_verbose_mode = false;

void set_verbose_mode(bool verbose)
{
    g_verbose_mode = verbose;
}

bool is_verbose_mode()
{
    return g_verbose_mode;
}
