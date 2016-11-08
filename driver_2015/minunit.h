 /* file: minunit.h */
#include <stdio.h>
 #define mu_assert(message, test) do { if (!(test)) return message; } while (0)

#define mu_run_test(test) do {char *message = test(); printf("test:%50s - %3s\n",#test,message?"NOK":"OK");tests_run++; \
                                if (message) return message; } while (0)


// #define mu_run_test(test) do {char *message = test(); tests_run++; \
//                                if (message) return message; } while (0)
// #define mu_run_test_named(test) do {char *message = test(); printf("test:%s - %s\n",#test,message?"NOK":"OK");tests_run++; \
//                                if (message) return message; } while (0)

 extern int tests_run;
