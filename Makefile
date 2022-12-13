EXE_NAME=p3
VAL_REPORT=valgrind_report.txt
LINKED_LIST=linked_list.c


all: clean $(EXE_NAME)

p3:
	gcc -Wall -o $(EXE_NAME) $(EXE_NAME).c linked_list.o

clean:
	rm -f $(EXE_NAME)

valgrind_check: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(VAL_REPORT) ./$(EXE_NAME)
	cat $(VAL_REPORT)

list:
	gcc -c $(LINKED_LIST)
