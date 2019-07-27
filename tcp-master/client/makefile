ALL:client.out

client.out:client_link.c client_main.c client_socket.c client_ui.c press.c
	gcc client_link.c client_main.c client_socket.c client_ui.c press.c -lsqlite3 -lpthread -o client.out -w
