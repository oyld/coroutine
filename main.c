#include "coroutine.h"
#include <stdio.h>

struct args {
	int n;
};

static void
foo(struct schedule * S, void *ud) {
	struct args * arg = ud;
	int start = arg->n;
	int i;
	for (i=0;i<10000;i++) {
		printf("coroutine %d : %d\n",coroutine_running(S) , start + i);
		coroutine_yield(S);
	}
}

static void
test(struct schedule *S) {
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };
	struct args arg3 = { 1000 };
	struct args arg4 = { 10000 };

	int co1 = coroutine_new(S, foo, &arg1);
	int co2 = coroutine_new(S, foo, &arg2);
	int co3 = coroutine_new(S, foo, &arg3);
	int co4 = coroutine_new(S, foo, &arg4);
	printf("main start\n");
	while (coroutine_status(S,co1) && coroutine_status(S,co2) &&
			coroutine_status(S,co3) && coroutine_status(S,co4)) {
		coroutine_resume(S,co1);
		coroutine_resume(S,co2);
		coroutine_resume(S,co3);
		coroutine_resume(S,co4);
	}
	printf("main end\n");
}

int 
main() {
	struct schedule * S = coroutine_open();
	test(S);
	coroutine_close(S);
	
	return 0;
}

