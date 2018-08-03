#include <cstdio>   // printf()
#include <cstdlib>  // EXIT_SUCCESS, EXIT_FAILURE

#include "test_parsers.h"       // did_pass_test_no_ocpi_app_parsers()
#include "test_app_defaults.h"  // did_pass_test_ocpi_app_default_values()
#include "test_app_mins.h"      // did_pass_test_ocpi_app_min_values()
#include "test_app_maxes.h"     // did_pass_test_ocpi_app_max_values()
#include "test_app_delays.h"    // did_pass_test_ocpi_app_AD9361_...() functions

int main(int argc, char **argv)
{
  bool swonly = false;
  if (argc > 1)
  {
    std::string arg_swonly(argv[1]);
    if(arg_swonly == "swonly")
    {
      swonly = true;
    }
    else
    {
      printf("invalid argument: %s (must either be 'swonly' or nothing')", argv[1]);
      goto failed;
    }
  }
  if (argc > 2)
  {
    printf("too many arguments, (must either have no arguments or swing argument 'swonly'')");
    goto failed;
  }

  if(!did_pass_test_no_ocpi_app_parsers())                        { goto failed; }

  if(not swonly)
  {
    if(!did_pass_test_ocpi_app_default_values())                    { goto failed; }
    if(!did_pass_test_ocpi_app_min_values())                        { goto failed; }
    if(!did_pass_test_ocpi_app_max_values())                        { goto failed; }
    if(!did_pass_test_ocpi_app_AD9361_DATA_CLK_Delay_enforcement()) { goto failed; }
    if(!did_pass_test_ocpi_app_AD9361_Rx_Data_Delay_enforcement())  { goto failed; }
  }
 
  printf("PASSED\n");
  return EXIT_SUCCESS;

  failed:
  printf("FAILED\n");
  return EXIT_FAILURE;
}

