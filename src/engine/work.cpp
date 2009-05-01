#include "work.hpp"
#include "procedure.hpp"

#include <jngl.hpp>

void Work::QuitEvent()
{
    GetProcedure().Quit();
}

Work::~Work()
{
}
