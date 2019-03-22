#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#include <stdio.h>
#include <unistd.h>


/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


int issafe(){
	int work[NUMBER_OF_RESOURCES];
	int finished[NUMBER_OF_CUSTOMERS];
	int i,j;
	for(i=0;i<NUMBER_OF_RESOURCES;++i)
		work[i] = available[i];
	for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
		finished[i] = 0;
	int flag_find = 0;
	int index = -1;
	while(1){
		index = -1;
		for(i=0;i<NUMBER_OF_CUSTOMERS;++i){
			if(finished[i]==1)
				continue;
			int flag = 0;
			for(j=0;j<NUMBER_OF_RESOURCES;++j){
				if(need[i][j]>work[j]){
					flag = 1;
					break;
				}
			}
			if(flag==0){
				index = i;
				break;
			}
		}
		if(index==-1){
			for(i=0;i<NUMBER_OF_CUSTOMERS;++i){
				if(finished[i]==0)
					return 0;
			}
			return 1;
		}
		else{
			for(i=0;i<NUMBER_OF_RESOURCES;++i){
				work[i] = work[i] + allocation[index][i];
			}
			finished[index] = 1;
		}
	}
}

int request(int consumer, int req_resource[NUMBER_OF_RESOURCES]){
	int i;
	int finish = 1;
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		if(req_resource[i]!=need[consumer][i])
			finish = 0;
	}
	if(finish == 1){
		for(i=0;i<NUMBER_OF_RESOURCES;++i){
			need[consumer][i] = 0;
			available[i] += allocation[consumer][i];
			allocation[consumer][i] = 0;
		}
		return 1;
	}
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		if(req_resource[i]>need[consumer][i]){
			printf("Request is more than need\n");
			return 0;
		}
	}
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		if(req_resource[i]>available[i]){
			printf("Request is more than available\n");
			return 0;
		}
	}
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		allocation[consumer][i] = allocation[consumer][i] + req_resource[i];
		need[consumer][i] = need[consumer][i] - req_resource[i];
		available[i] = available[i] - req_resource[i];
	}
	if(issafe()==1)
		return 1;
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		allocation[consumer][i] = allocation[consumer][i] - req_resource[i];
		need[consumer][i] = need[consumer][i] + req_resource[i];
		available[i] = available[i] + req_resource[i];
	}
	printf("Not safe\n");
	return 0;
}

int release(int consumer, int rel_resource[NUMBER_OF_RESOURCES]){
	int i;
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		if(rel_resource[i]>allocation[consumer][i]){
			printf("Release is more than allocation\n");
			return 0;
		}
	}
	for(i=0;i<NUMBER_OF_RESOURCES;++i){
		allocation[consumer][i] -= rel_resource[i];
		need[consumer][i] += rel_resource[i];
		available[i] += rel_resource[i];
	}
	return 1;
}

void init_matrix(){
    FILE *in = fopen("matrix.txt","r");
    if(in==NULL){
        printf("fileopen wrong\n");
        return;
    }
    int i,j;
    for(i=0;i<NUMBER_OF_CUSTOMERS;++i){
    	fscanf(in,"%d %d %d %d\n",&maximum[i][0],&maximum[i][1],&maximum[i][2],&maximum[i][3]);
    }
    fclose(in);
    for(i=0;i<NUMBER_OF_CUSTOMERS;++i){
    	for(j=0;j<NUMBER_OF_RESOURCES;++j){
    		allocation[i][j] = 0;
    		need[i][j] = maximum[i][j];
    	}
    }
/*    while(!feof(in)){
    	int i,j
        fscanf(in);
        // add the task to the scheduler's list of tasks
    }*/
}

void banker(){
	char s;
	int resource_tmp[4];
	int b;
	while(1){
		s = getchar();
		if(s=='*'){
			getchar();
			printf("available:\n");
			int i;
			printf("%d %d %d %d\n",available[0],available[1],available[2],available[3]);
			printf("maximum:\n");
			for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
				printf("%d %d %d %d\n",maximum[i][0],maximum[i][1],maximum[i][2],maximum[i][3]);
			printf("allocation:\n");
			for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
				printf("%d %d %d %d\n",allocation[i][0],allocation[i][1],allocation[i][2],allocation[i][3]);
			printf("need:\n");
			for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
				printf("%d %d %d %d\n",need[i][0],need[i][1],need[i][2],need[i][3]);
		}
		else if(s=='R'){
			s = getchar();
			scanf("%d %d %d %d %d",&b, &resource_tmp[0],&resource_tmp[1],&resource_tmp[2],&resource_tmp[3]);
			getchar();
			if(s=='Q'){
				if(request(b,resource_tmp)==0){
					printf("request Not successfully\n");
				}
				else{
					printf("Done\n");
				}
			}
			if(s=='L'){
				if(release(b,resource_tmp)==0){
					printf("release Not successfully\n");
				}
				else{
					printf("Done\n");
				}
			}
		}
	}
	return;
}

int main(int argc,  char *argv[]){
	// get command line arguments argv[1], argv[2], argv[3]
	int i;
	for(i=1;i<=NUMBER_OF_RESOURCES;++i)
		available[i-1] = atoi(argv[i]);
	init_matrix();
	banker();
	//exit
	return 0;
}