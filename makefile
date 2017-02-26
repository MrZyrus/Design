ALBAIDA = A B
CHRISTOFIDES = 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
DEGREE = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
GRID = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
RANDOM = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19

main:	Design.cpp
	g++ -std=c++11 -o Design Design.cpp

solve:	main
	for i in $(ALBAIDA); do ./Design instanciasPRPP/ALBAIDA/ALBAIDA$${i}NoRPP; done || bin/true
	for i in $(CHRISTOFIDES); do ./Design instanciasPRPP/CHRISTOFIDES/P$${i}NoRPP; done || bin/true
	for i in $(DEGREE); do ./Design instanciasPRPP/DEGREE/D$${i}NoRPP; done || bin/true
	for i in $(GRID); do ./Design instanciasPRPP/GRID/G$${i}NoRPP; done || bin/true
	for i in $(RANDOM); do ./Design instanciasPRPP/RANDOM/R$${i}NoRPP; done || bin/true