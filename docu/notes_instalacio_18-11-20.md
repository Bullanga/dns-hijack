# Notes de la instalació feta el 18.11.2020 al servidor develop
És una instalacio que es fa de cares la reunió amb dunia kato del 21 de novembre de 2020. El que farem es montar una vpn
amb el despatx usant el server com a servidor de VPN i montarem tota la infraestructura sense el captive portal i només
amb moodle funcionant.

## Questions que detectem
 Hi ha 4 parts diferenciades en aquest progecte:
- **Servidor base:** Seteig del servidor. Instalació de dependencies com python o docker i el netplan (ip estàtica i
    coses que fa el ./instal.sh de servidor_base) http://github.com/aucoop/servidor_base
- **Dockers:** Els dockers amb els continguts
- **Inite:** Captive portal i portal de benvinguda https://github.com/aucoop/inite
- **Dns-hijack:** Dns spoofing. https://github.com/bullanga/dns-hijack

##Passos per a una instal·lacio minimalista com la d'avui
1. Fer la configuració de xarxa amb ip estàtica pel servidor
2. Fer la instal·lació de docker i els containers necessaris. 
    2.1 Instalar docker
    2.2 Preparar els contenidors de moodle i mariadb  --> AQUI HEM FALLAT. EL MOODLE CAU NO SE PQ
    2.Fer el docker-compose up.
3. Fer la instalació d'inite sense el captive portal. Simplement servir l'apache
4. Instalar el dns-hijack, setejant el servei de systemd i compilant sense inite.
5. Instalar openvpn server al servidor d'aucoop. Al de producció.
    5.1 Posar com a dns el servidor de develop durant la instal·lació
6. Fer la configuració del router:
    6.1. Obrir els ports per a obrir la VPN al firewall


