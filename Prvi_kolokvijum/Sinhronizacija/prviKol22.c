/*Korišćenjem programskog jezika C kreirati Linux program u kome glavna nit kreira celobrojni niz od N elemenata i
popunjava ga slučajnim vrednostima iz opsega [0, 10]. Glavna nit pokreće dve dodatne niti koje su zadužene za
sortiranje niza. Prva dodatna nit sortira prvu polovinu niza (0-N/2 – 1) u rastući redosled a druga nit sortira
drugu polovinu niza (N/2 – N-1) u opadajući redosled. Kada obe dodatne niti završe sa sortiranjem, glavna nit
štampa sortirani niz. Za sinhronizaciju niti koristiti mutex-e i uslovne promenljive.*/

