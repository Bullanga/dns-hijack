





all: dns_RR_t.o 
	gcc -o dns dns.c dns_RR_t.o


dns_RR_t.o: dns_RR_t.c
	gcc -c dns_RR_t.c



clean:
	rm -rf dns_RR_t.o dns
