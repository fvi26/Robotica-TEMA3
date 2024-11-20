

# Cerinte :

Jocul începe prin afișarea unui mesaj de bun venit pe LCD. Apăsarea unui buton declanșează startul jocului.
Pentru inițierea jocului, implementarea butonului de start este flexibilă, oferind următoarele opțiuni:

Pornirea jocului la apăsarea oricărui buton – Jocul se poate declanșa prin apăsarea oricărui buton conectat la sistem.
Un buton specific pentru start – Un buton anume, clar identificat pe breadboard, poate fi desemnat pentru pornirea jocului.
Adăugarea unui buton dedicat – Se poate introduce un al șaptelea buton exclusiv pentru inițierea jocului.
Desfășurarea Rundelor
Fiecare jucător dispune de trei butoane, fiecare asociat unui LED de o culoare distinctă, și un LED RGB suplimentar.
La începutul fiecărei runde, doar unul dintre jucători este activ.

LED-ul RGB al jucătorului activ se aprinde într-o culoare corespunzătoare unuia dintre butoanele sale. Jucătorul trebuie să apese rapid butonul care corespunde culorii afișate de LED-ul RGB pentru a acumula puncte. Cu cât reacția este mai rapidă, cu atât punctajul obținut este mai mare.

La sfârșitul fiecărei runde, LCD-ul afișează scorurile actualizate ale ambilor jucători.
Pe toată durata jocului, LCD-ul arată în timp real punctajele fiecărui jucător.

Timpul și Finalizarea Jocului
Pe parcursul jocului, servomotorul se rotește pentru a indica progresul. O rotație completă a servomotorului marchează încheierea jocului, iar viteza rotației poate fi ajustată în funcție de preferințe.

La finalul jocului, LCD-ul afișează numele câștigătorului împreună cu scorul final. După câteva secunde, ecranul revine la afișarea mesajului de start, pregătit pentru o nouă sesiune.

# Descriere :
În această temă, fiecare echipă va crea un joc de reflex competitiv pentru doi jucători, în care ambii participanți vor concura pentru a obține cel mai mare punctaj, testându-și viteza de reacție. Proiectul se va realiza în echipe de câte două persoane.

Fiecare jucător va avea butoane și LED-uri proprii, iar jocul se va desfășura în mai multe runde. Scopul fiecărui jucător este să apese cât mai rapid butonul care corespunde culorii afișate pe LED-ul RGB al echipei sale. Punctajul fiecărui jucător va fi afișat pe un ecran LCD și se va actualiza pe parcursul jocului. La finalul jocului, jucătorul cu cel mai mare punctaj este declarat câștigător.

# Componente: 

6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)
2x LED RGB (1 pentru fiecare jucător)
6x butoane (3 pentru fiecare jucător)
1x LCD
1x servomotor
2x Breadboard
Fire de legatura
2x Arduino Uno

# Schema Montaj : 


# Imagine Montaj: 
![WhatsApp Image 2024-11-20 at 10 58 31](https://github.com/user-attachments/assets/8eff720d-9590-449c-a115-b6fdf595814a)
![WhatsApp Image 2024-11-20 at 10 58 30](https://github.com/user-attachments/assets/5bf87557-c85d-4d27-ba47-0411cd5a3770)
![1](https://github.com/user-attachments/assets/8132c5fc-ed1b-4b05-85f9-2f37b3e7b8d3)


# Videoclip Montaj: 
https://youtube.com/shorts/aiYvDzG09GI?si=SE-FnjBaswFRixji

