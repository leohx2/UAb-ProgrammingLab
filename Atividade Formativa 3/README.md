# How to compile


### From the `unity_test` folder:

##### For Macbook:
```bash
gcc creating_and_moving.c ../aux.c ../draw_rectangles/*.c ../handle_commands/command_reader.c -o MAC_creating_and_moving_tests
gcc gravity_collision_commands.c ../aux.c ../draw_rectangles/*.c ../handle_commands/command_reader.c -o MAC_gravity_collision_commands_tests
```

##### For Windows:
```bash
x86_64-w64-mingw32-gcc creating_and_moving.c ../aux.c ../draw_rectangles/*.c ../handle_commands/command_reader.c -o WIN_creating_and_moving_tests.exe
x86_64-w64-mingw32-gcc gravity_collision_commands.c ../aux.c ../draw_rectangles/*.c ../handle_commands/command_reader.c -o WIN_gravity_collision_commands_tests.exe
```


###  From the main folder (for building the main executable):

##### For Macbook:
```bash
gcc main.c handle_commands/command_reader.c draw_rectangles/draw_rectangle.c draw_rectangles/save_rectangle_coordinates.c aux.c -o MAC_AF3
```
##### For Windows:
```bash
x86_64-w64-mingw32-gcc main.c handle_commands/command_reader.c draw_rectangles/draw_rectangle.c draw_rectangles/save_rectangle_coordinates.c aux.c -o WIN_AF3.exe
```