cmake_minimum_required(VERSION 3.7.2)

project(dhrystone C)

DeclareCAmkESComponent(Client SOURCES components/Client/src/client.c)
DeclareCAmkESComponent(Timer SOURCES components/Timer/src/timer.c)

DeclareCAmkESRootserver(dhrystone.camkes)
