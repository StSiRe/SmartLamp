# Lamp
Проект умной лампы/хаба умного дома    
Лампа умеет рисовать красивые анимации, помогает пробуждаться рано утром а также может выступать в виде хаба для умного дома     
Пример работы лампы лежит в папке renders(фото и презентация проекта)

TODO: 

MQTT Support
ZigBee Module support
LoRa Modules support

Make additional support for flash drives(like sd cards)

Sync between 2(or more lamp`s)

Full BlueTooth Audio 

Problems:

На данный момент обнаружена серьезная проблема - производительность ESP32V1C2 16mb весьма низкая для выполнения сразу двух-трех задач
А именно, при попытке:
(1) Играть аудио через BlueTooth отправляя данные на i2s кодек
(2) Отрисовывать любую нестатичную анимацию
(3) Отдавать данные для веб-страницы
Платформа крашится и продолжает работать только после перезагрузки

По тестам, нагрузка на ядра в процессе выполнения объявленных выше действий колеблется в районе 97-100 процентов процессорного времени, соответственно следует подумать о переходе на другие процессоры(пока рассматриваю AllWinner A13 - стоит 1 доллар и имеет Cortex A8 1Ghz ядро c полноценной памятью и прочими фишками, либо же F1C100s 32mb ram ddr и одно производительное ядро)

В любом случае развитие проекта требует перехода на более продвинутое железо

Но так как существует концепция HooliLamp Lite <-> HooliLamp <-> HooliLamp Max
То вполне возможно, что обрезанную версию лампы можно выпустить на esp32 с небольшими доделками/интеграшками


