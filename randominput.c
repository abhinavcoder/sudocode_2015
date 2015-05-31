#include <stdio.h>
#include <time.h>
#include <stdlib.h>
 
int main()
{	
	srand(time(NULL));
	setvbuf(stdout, NULL, _IONBF, 0);
	char reply[20];
	int x, y, enemies, i;
        float travel_army, travel_spy ;
	scanf("%d %d %d %f %f",&x, &y, &enemies, &travel_spy, &travel_army);
	printf("ASSIGN SPY\n");
	scanf("%s", reply);
	puts("SNAPSHOT\n");
	scanf("%s", reply);
	while(1){
		printf("MOVE SPY\n %d %d\n",rand()%600,rand()%800);
		scanf("%s", reply);
		if(reply[0]=='S'){
			printf("ASSIGN SPY\n");
			scanf("%s", reply);
		}
		puts("SNAPSHOT\n");
		scanf("%s", reply);
		printf("MOVE SPY\n %d %d\n",rand()%600,rand()%800);
		scanf("%s", reply);
		if(reply[0]=='S'){
			printf("ASSIGN SPY\n");
			scanf("%s", reply);
		}
		puts("SNAPSHOT\n");
		scanf("%s", reply);
		printf("MOVE ARMY\n %d %d\n",rand()%600,rand()%800);
		scanf("%s", reply);
		puts("SNAPSHOT\n");
		scanf("%s", reply);
	}
	printf("ABANDON SPY\n");
	scanf("%s", reply);
	puts("SNAPSHOT\n");
	scanf("%s", reply);
	return 0;
}
