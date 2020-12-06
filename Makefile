help:
	@echo "Choose one of the following commands:"
	@echo " - start            - to start running the program"
	@echo " - compile          - to compile all the files"
	@echo " - help             - list all make commands"
	@echo ""

start:
	clear & gcc -o main main.c && ./main

compile:
	clear & gcc -g main.c

default:
	help
