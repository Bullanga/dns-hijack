#include <stdio.h>
#include <string.h>
#include "munit/munit.h"
#include "wrapp.h"
#include "tests_guardaIP.h"

//la gracia d'aquest fitxer es anar posant els tests de cada paquet i veure si els compleix

//##########################################
//      AQUI COMENCEN ELS TESTS DE guardaIP
//##########################################

/* Ordre en el que es troben les coses del test:
* 1. Funcions que duen a terme els tests
* 2. Array de tests
* 3. Suite de tests de guardaIP */

MunitResult test_guardaIP_init_guardaIP(const MunitParameter params[], void* user_data_or_fixture){
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    //Aquest rang és de 262 IPs
    if (!init_guardaIP(ip_test_ini,ip_test_final)) return MUNIT_FAIL;
    munit_assert_int(r_lenght, ==, 263);
    return MUNIT_OK;
}

MunitResult test_guardaIP_r_clear(const MunitParameter params[], void* user_data_or_fixture){
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    init_guardaIP(ip_test_ini, ip_test_final);
    int i;
    for (i = 0; i < r_length; i++){
	munit_assert_int(registry[i], ==, 0);
    }
    return MUNIT_OK;
}
MunitResult test_guardaIP_r_add(const MunitParameter params[], void* user_data_or_fixture){
    const char* ip_test_ini = "192.168.2.3";
    const char* ip_test_final = "192.168.3.10";
    init_guardaIP(ip_test_ini, ip_test_final);
    char a[]= "192.168.2.4";
    munit_assert_int(r_add(a) )
    munit_assert(r_add(a) && registry[160]);
    return MUNIT_OK
}
MunitResult test_guardaIP_r_findValue(const MunitParameter params[], void* user_data_or_fixture){
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    init_guardaIP(ip_test_ini, ip_test_final);
    char a[]= "192.168.2.4";
    r_add(a);
    munit_assert(r_findValue(a));
    return MUNIT_OK;
}

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


//       AQUI ACABEN ELS TESTS DE guardaIP
//#################################################

// ...aqui van tots els altres tests (funcions, array de tests i suite de cada una de les funcionalitats)...

//Main dels tests perquè executi les suites 
int main(int argc, const char* argv[]){
    munit_suite_main(&suite_guardaIP, NULL, argc, argv);
}

