cmake_minimum_required(VERSION 3.7.2)

project(dhrystone C)

DeclareCAmkESComponent(Client SOURCES
    components/Client/src/client.c
    components/Client/src/dhry_1.c
    components/Client/src/dhry_2.c
    )
DeclareCAmkESComponent(Timer SOURCES components/Timer/src/timer.c)

DeclareCAmkESRootserver(dhrystone.camkes)
