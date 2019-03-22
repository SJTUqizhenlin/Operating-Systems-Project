#include <stdio.h>
#include <unistd.h>
#include <string.h>

int maxlength = 1000;
int current_length = 0;
int Memory_size = 0;

typedef struct single_process{
	char name[20];
	int start;
	int end;
}process;





int insert(process * Process, int index, char name[20], int need_size){
	int i = current_length;
	char tmp[20];
	for(;i>index;--i){
		Process[i].start = Process[i-1].start;
		Process[i].end = Process[i - 1].end;
		strncpy(Process[i].name, Process[i - 1].name, 19);
	}
	if (index == 0)
		Process[index].start = 0;
	else
		Process[index].start = Process[index - 1].end;
	Process[index].end = Process[index].start + need_size;
	strncpy(Process[index].name, name, 19);
	current_length += 1;
	return 0;
}
int request(process* Process, char name[20], int need_size, char allocate_type){
	printf("request for memory\n");

	int* space = (int*)malloc(sizeof(int)*maxlength);
	space[0] = 0;
	int i;
	int tmp = 0;
	for(i=0;i<current_length;++i){
		space[i] = Process[i].start - tmp;
		tmp = Process[i].end;
	}
	space[current_length] = Memory_size - tmp;
	int index = -1;
	if(allocate_type=='F'){
		for(i=0;i<= current_length;++i){
			if(need_size <= space[i]){
				index = i;
				insert(Process, i, name, need_size);
				return 0;
			}
		}
		printf("Have no space!\n");
		return 1;
	}
	else if(allocate_type=='B'){
		int least = 1000000;
		int res = -1;
		for(i=0;i<= current_length;++i){
			if(need_size <= space[i]){
				if( least > space[i]){
					least = space[i];
					res = i;
				}
			}
		}
		if (res != -1)
			insert(Process, res, name, need_size);
		else {
			printf("Have no space!\n");
			return 1;
		}
		return 0;
	}
	else{
		int most = -1;
		int res = -1;
		for (i = 0; i <= current_length; ++i) {
			if (need_size <= space[i]) {
				if (most < space[i]) {
					most = space[i];
					res = i;
				}
			}
		}
		if (res != -1)
			insert(Process, res, name, need_size);
		else {
			printf("Have no space!\n");
			return 1;
		}
		return 0;
	}
	free(space);
	return 0;
}

int Remove(process* Process, int index) {
	int i = 0;
	for (i = index; i < current_length - 1; ++i) {
		Process[i].start = Process[i + 1].start;
		Process[i].end = Process[i + 1].end;
		strncpy(Process[i].name, Process[i + 1].name, 19);
	}
	return 0;
}

int release(process* Process, char name[20]){
	int i = 0;
	for (i = 0; i < current_length; ++i) {
		if (strcmp(Process[i].name, name) == 0) {
			Remove(Process, i);
			current_length -= 1;
			return 0;
		}
	}
	printf("Not found it!\n");
	return 1;
}

int compact(process* Process){
	int i = 0;
	int tmp = 0;
	for (i = 0; i < current_length; ++i) {
		int t = Process[i].start - tmp;
		Process[i].start = tmp;
		tmp = Process[i].end;
		Process[i].end = Process[i].end - t;
	}
	printf("Compact done!\n");
}

int state(process* Process){
	int i=0;
	for(i=0;i<current_length;++i){
		printf("Addresses [%d:%d] Process %s\n", Process[i].start, Process[i].end, Process[i].name);
	}
	return 0;
}

void allocate(process *Process){
	char str[10] = "allocator>";
	char s;
	while(1){
		printf("%s", str);
		s = getchar();
		if(s=='R'){
			char name[20];
			s = getchar();
			getchar();
			if(s=='Q'){
				int need_size;
				char allocate_type;
				scanf("%s %d %c", name, &need_size, &allocate_type);
				getchar();
				request(Process, name, need_size, allocate_type);
			}
			else{
				scanf("%s", name);
				getchar();
				release(Process, name);
			}
		}
		else if(s=='C'){
			getchar();
			compact(Process);
		}
		else{
			int i;
			for(i=0;i<4;++i)
				getchar();
			state(Process);
		}
	}
}

int main(int argc,  char *argv[]){
	// get command line arguments argv[1], argv[2], argv[3]
	process Process[1000];
	int i;
	Memory_size = atoi(argv[1]);
	allocate(Process);
	//exit
	return 0;
}
