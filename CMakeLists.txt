cmake_minimum_required(VERSION 3.23)
project(EGFWD C)

set(CMAKE_C_STANDARD 99)

add_executable(App Application/app.c Card/card.c Terminal Terminal/terminal.c Server Server/server.c)
add_executable(Card Card/card.c)
add_executable(Server Server/server.c Terminal Terminal/terminal.c)
add_executable(Terminal Terminal/terminal.c)
add_executable(Application_Tests Application/Tests.c Card/card.c Terminal Terminal/terminal.c Server Server/server.c)