#include <stdio.h>
#include <string.h>
#include "munit/munit.h"
#include "wrapp.h"
#include "tests_guardaIP.h"

//la gracia d'aquest fitxer es anar posant els tests de cada paquet i veure si els compleix

//AQUI L'ARRAY DE TESTS DE guardaIP
static MunitTest guardaIP[] = {
  {
    /* The name is just a unique human-readable way to identify the
     * test. You can use it to run a specific test if you want, but
     * usually it's mostly decorative. */
    (char*) "/test_init_guardaIP",
    test_guardaIP_init_guardaIP,  //Funcio que implementa el test
    NULL,  //Setup. Funcio que s'executa al principi del test
    NULL,  //Tear-down. Funció que s'executa al final del test
    /* Finally, there is a bitmask for options you can pass here.  You
     * can provide either MUNIT_TEST_OPTION_NONE or 0 here to use the
     * defaults. */
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  //Aqui s'ensenyen els tests posats a l'array d'una manera més compacta
  { (char*) "/test_guardaIP_r_clear", test_guardaIP_r_clear, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/test_guardaIP_r_add", test_guardaIP_r_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, test_params },
  { (char*) "/test_guardaIP_r_findValue", test_guardaIP_r_findValue, NULL, NULL, MUNIT_TEST_OPTION_NONE, test_params },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL } //Sanseakabó
};

//AQUI HI HA LA SUITE DEL TEST DE guardaIP
static const MunitSuite suite_guardaIP = {
  "/test-guardaIP", /* name */
  guardaIP, /* tests */
  NULL, /* suites */
  1, /* iterations */
  MUNIT_SUITE_OPTION_NONE /* options */
};


int main(){
    munit_suite_main(&test_suite, NULL, argc, argv);
}

