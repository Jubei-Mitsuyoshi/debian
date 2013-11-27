/* 
   This is a file used by 002_libfile example.

   test code to test that libecasoundc can be compiled
 */
#include <stdio.h>
#include <ecasoundc.h>

int main()
{
  printf("hello world\n");
  eci_init();
  eci_command("engine-status");
  printf("%s\n", eci_last_string());
  eci_cleanup();
  return 0;
}
