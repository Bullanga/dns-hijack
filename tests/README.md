# Coses bàsiques i importants del framework:
- Té macros que molen molt perque hi ha molta info. Exemple: ```munit_assert_int(foo, ==, bar);``` Com aquestes n'hi ha un munt (https://nemequ.github.io/munit/#assertions).
- Te funcions per generar numeros aleatoris: https://nemequ.github.io/munit/#prng
- Per gestionar els test cases veure l'exemple que hi ha fet a la carpeta tests o mirar aquesta docu: https://nemequ.github.io/munit/#prng
- Cada cosa que volem testejar ha d'estar en una funció diferent de l'estil ```MunitResult my_test(const MunitParameter params[], void* user_data_or_fixture);```
- Aquests tests separats, per creat un _test case_ s'han de posar en un array de tests d'aquest típus:
```C
MunitTest tests[] = {
//Aqui a sota hi ha un test (una funció).  
{
    "/my-test", /* nom del test */
    my_test, /* test (nom de la funció) */
    NULL, /* setup (funció que s'executarà abans del test. Veure docu per saber més) */
    NULL, /* tear_down (funció que s'executarà després del test, veure docu per saber més)*/
    MUNIT_TEST_OPTION_NONE, /* options */
    NULL /* parameters */
  },
  /* Mark the end of the array with an entry where the test
   * function is NULL */
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
```
- Un cop tenim el nostre array de tests el posem a una _suite_:

```C
static const MunitSuite suite = {
  "/my-tests", /* nom de la suite */
  tests, /* array de tests */
  NULL, /* suites imbrincades. Si es vol saber més mirar docu */
  1, /* iterations */
  MUNIT_SUITE_OPTION_NONE /* No serveix de res, esta per versions futures*/
};
```
- Es pot cridar a les suites amb una funció com la següent: 
```C
int
munit_suite_main(const MunitSuite* suite,
                 void* user_data,
                 int argc,
                 const char* argv[]); 
```
- Es poden parametritzar els tests, és a dir, fer un test molts cops amb paràmetres diferents. Hi ha docu aquí: https://nemequ.github.io/munit/#parameterized-tests
- 

**Carpeta tests/test.c es on es faran els tests. ALLÀ HI HA UN EXEMPLE DE TOT AIXÒ**

Doncs, per cada *test_case* hi ha d'haver un array de tests i una _suite_

**L'únic fitxer que cal tocar es el test.c. Per executar els tests cal fer ```make tests```*test.c. Per executar els tests cal fer ```make tests```**
