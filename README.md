# What is dns-hijack?

dns-hijack is a indispensible part of the **[INITE project](https://github.com/xXIty/init "INITE github page")**, which is a captive portal that uses the DNS hijacking technic. The dns-hijack is a dummy and stupid pseudo DNS  with the ability to apply some logics in the domains resolution.

# Installation

```sh
git clone https://github.com/dhap0/dns-hijack.git
cd dns-hijack
make
sudo ./dns


```

## How it works?

The configuration file is in [include/config.h](../master/include/config.h)
 path. There are some important control variables:

```c
/*
*	`use_inite` can have only two differents values:
*
* 		0	: The web portal INITE is NOT in use
* 		1	: The web portal INITE is in use
*/
const unsigned int use_inite = 1;

/*
*	`public_domains` is a list of domains (should be ended with '.') that the dns will respond the ip indicated in 
* the `public_ip` variable.
*/
const char *public_domains[] = { "duniakato.dks.", };
/*
*	`private_domains` is a list of domains (should be ended with '.') that the dns will respond the ip indicated in 
* the `private_ip` variable. The `private_domain` variable only have sense if `use_inite` equals 1 and the values 
* should correspond with the domain names you would prevent user access without registering in the INITE portal first.
*/
const char *private_domains[] = { "wikipedia.dks.", };

const char public_ip[16]= "127.0.0.1"  ;
const char private_ip[16]= "0.0.0.0"  ;

const char comment[] = "More info: https://github.com/dhap0/dns-hijack";

const unsigned int max_forks =  0;

const char dhcp_ip_range[2][16] = {"192.168.1.2", "192.168.1.100"};

```
