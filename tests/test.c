#include "munit/munit.h"
//#include "wrapp.h"
#include "guardaIP.h"
#include "dnslib.h"
#include "config.h"
//#include "variables.h"

//la gracia d'aquest fitxer es anar posant els tests de cada paquet i veure si els compleix

//##########################################
//      AQUI COMENCEN ELS TESTS DE guardaIP
//##########################################

/* Ordre en el que es troben les coses del test:
* 1. Funcions que duen a terme els tests
* 2. Array de tests
* 3. Suite de tests de guardaIP */

MunitResult test_parse_request_domains(const MunitParameter params[], void* user_data_or_fixture){
//		char *data = user_data_or_fixture;
		char param[] = "\006google\002es\000\001\000";
		char domain[256];
		unsigned int type;
		type = parse_requested_domain(domain, param);
		
		munit_assert_string_equal(domain,"google.es");
		munit_assert_uint16(type,==,TYPE_A);
		
    return MUNIT_OK;
}

MunitResult test_guardaIP_init_guardaIP(const MunitParameter params[], void* user_data_or_fixture){
    const char* ip_test_ini = dhcp_ip_range[0];
    const char* ip_test_final = dhcp_ip_range[1];
    //printf("IP inicial: %s IP final: %s", ip_test_ini, ip_test_final);
    if (!init_guardaIP(ip_test_ini,ip_test_final)) return MUNIT_FAIL;
    int length = r_get_length();
    munit_assert_int(length, ==, 199);
    return MUNIT_OK;
}

MunitResult test_guardaIP_r_clear(const MunitParameter params[], void* user_data_or_fixture){
    int* registre = r_get_registry();
    int length = r_get_length();
    const char* ip_test_ini = dhcp_ip_range[0];
    const char* ip_test_final = dhcp_ip_range[1];
    init_guardaIP(ip_test_ini, ip_test_final);
    int i;
    for (i = 0; i < length; i++){
	munit_assert_int(registre[i], ==, 0);
    }
    return MUNIT_OK;
}
MunitResult test_guardaIP_r_add(const MunitParameter params[], void* user_data_or_fixture){
    const char* ip_test_ini = dhcp_ip_range[0];
    const char* ip_test_final = dhcp_ip_range[1];
    init_guardaIP(ip_test_ini, ip_test_final);
    int* registre = r_get_registry();
    char a[]= "192.168.1.4";
    //printf("La llargada dels registres és de %d\n", r_get_length());
    if (r_add(a)==-1) return MUNIT_FAIL;
    munit_assert(r_add(a) );
    munit_assert(r_add(a) && registre[3]);
    return MUNIT_OK;
}

MunitResult test_guardaIP_r_add_forçant_error(const MunitParameter params[], void* user_data_or_fixture){
    const char* ip_test_ini = dhcp_ip_range[0];
    const char* ip_test_final = dhcp_ip_range[1];
    init_guardaIP(ip_test_ini, ip_test_final);
    int* registre = r_get_registry();
    char a[]= "192.199.1.4";
    //printf("La llargada dels registres és de %d\n", r_get_length());
    munit_assert_int(-1, ==, r_add(a));
}

MunitResult test_guardaIP_r_findValue(const MunitParameter params[], void* user_data_or_fixture){
    const char* ip_test_ini = dhcp_ip_range[0];
    const char* ip_test_final = dhcp_ip_range[1];
    init_guardaIP(ip_test_ini, ip_test_final);
    char a[]= "192.168.1.4";
    r_add(a);
    munit_assert(r_findValue(a));
    return MUNIT_OK;
}

//AQUI L'ARRAY DE TESTS DE guardaIP
static MunitTest tests_guardaIP[] = {
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
  { (char*) "/test_guardaIP_r_add", test_guardaIP_r_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/test_guardaIP_r_findValue", test_guardaIP_r_findValue, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL } //Sanseakabó
};

static MunitTest tests_dnslib[] = {
	{ "/test_parse_request_domains", test_parse_request_domains, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};

//AQUI HI HA LA SUITE DEL TEST DE guardaIP
static const MunitSuite suite_guardaIP = {
  "/guardaIP", /* name */
  tests_guardaIP, /* tests */
  NULL, /* suites */
  1, /* iterations */
  MUNIT_SUITE_OPTION_NONE /* options */
};

//AQUI HI HA LA SUITE DEL TEST DE guardaIP
static const MunitSuite suite_dnslib = {
  "/dnslib", /* name */
  tests_dnslib, /* tests */
  NULL, /* suites */
  1, /* iterations */
  MUNIT_SUITE_OPTION_NONE /* options */
};

//       AQUI ACABEN ELS TESTS DE guardaIP
//#################################################

// ...aqui van tots els altres tests (funcions, array de tests i suite de cada una de les funcionalitats)...

//Main dels tests perquè executi les suites 
int main(int argc, char* const* argv){
    munit_suite_main(&suite_guardaIP, NULL, argc, argv);
		munit_suite_main(&suite_dnslib, NULL, argc, argv);
}

