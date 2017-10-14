#ifndef DATA_H_
#define DATA_H_

#define ARR_SIZE 5

struct mystruct{ 
	int a;
	int b;
	int c;
};

union data {
	int dig;
	char arr[ARR_SIZE];
	struct mystruct my_struct;
};

typedef enum {
	DIGIT  = 1,
	ARRAY  = 2,
	STRUCT = 3,
} msg_type_t;

typedef struct{
	msg_type_t ms_type;
	union data ms_body;
}message_t;


#endif
