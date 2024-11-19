hw2: Init.o List.o main.o run.o Scheduler.o Thread.o TestCase1.o TestCase2.o TestCase3.o
	gcc -o hw2 Init.o List.o main.o run.o Scheduler.o Thread.o TestCase1.o TestCase2.o TestCase3.o -pthread

Init.o: Init.h Thread.h list.h Init.c
	gcc -c -o Init.o Init.c -pthread

List.o: list.h list.c
	gcc -c -o List.o list.c -pthread

main.o: Init.h Scheduler.h Thread.h list.h run.h main.c
	gcc -c -o main.o main.c -pthread

run.o: run.h list.h run.c
	gcc -c -o run.o run.c -pthread

Scheduler.o: Scheduler.h list.h run.h Scheduler.c
	gcc -c -o Scheduler.o Scheduler.c -pthread

Thread.o: Thread.h Init.h Scheduler.h list.h run.h Thread.c
	gcc -c -o Thread.o Thread.c -pthread

TestCase1.o: TestCase1.h TestCase1.c
	gcc -c -o TestCase1.o TestCase1.c -pthread

TestCase2.o: TestCase2.h TestCase2.c
	gcc -c -o TestCase2.o TestCase2.c -pthread

TestCase3.o: TestCase3.h TestCase3.c
	gcc -c -o TestCase3.o TestCase3.c -pthread
