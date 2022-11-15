EXE_NAME=p2
VAL_REPORT=valgrind_report.txt

all: clean $(EXE_NAME)

p2:
	gcc -Wall -o p2 $(EXE_NAME).c linked_list.o

clean:
	rm -f $(EXE_NAME)

valgrind_check: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(VAL_REPORT) ./$(EXE_NAME)
	cat $(VAL_REPORT)
