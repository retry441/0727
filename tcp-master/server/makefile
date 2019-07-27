ALL:server.out

server.out:server_handle.c server_link.c server_main.c server_socket.c server_sqlite.c server_ui.c
	gcc server_handle.c server_link.c server_main.c server_socket.c server_sqlite.c server_ui.c -lpthread -lsqlite3 -o server.out -w
