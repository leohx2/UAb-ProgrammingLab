To run it locally without a make file, you can copy and past the following instruction in the base dir

gcc main.c functions/manage_catalog.c functions/manage_lists.c functions/menu.c functions/search.c functions/aux_functions.c functions/manage_interactions.c functions/favorite_lists.c functions/activity_report.c functions/recommendation.c login.c -Wall -Wunused-value -Wno-uninitialized -o Atividade2

Compiling from a UNIX to a windows 64 and 32 bits
x86_64-w64-mingw32-gcc main.c functions/manage_catalog.c functions/manage_lists.c functions/menu.c functions/search.c functions/aux_functions.c functions/manage_interactions.c functions/favorite_lists.c functions/activity_report.c functions/recommendation.c login.c -Wall -Wunused-value -Wno-uninitialized -o AF2_64.exe

i686-w64-mingw32-gcc main.c functions/manage_catalog.c functions/manage_lists.c functions/menu.c functions/search.c functions/aux_functions.c functions/manage_interactions.c functions/favorite_lists.c functions/activity_report.c functions/recommendation.c login.c -Wall -Wunused-value -Wno-uninitialized -o AF2_32.exe


Feel free to change the name from Atividade2 to whatever you want. 