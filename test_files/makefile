all:
	listen

listen:
	clang test_udplisten.c ../src/sverresnetwork.c -o test_listen -lpthread -Wall

broadcast:
	clang test_udpbroadcast.c ../src/sverresnetwork.c -o test_broadcast -lpthread -Wall
