EXE_NAME=p1
VAL_REPORT=valgrind_report.txt

all: clean $(EXE_NAME)

p1:
	gcc -Wall -o p1 $(EXE_NAME).c linked_list.o allocationsList.o

clean:
	rm -f $(EXE_NAME)

valgrind_check: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(VAL_REPORT) ./$(EXE_NAME)
	cat $(VAL_REPORT)
