#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>



pthread_mutex_t mutex;

pthread_cond_t cond0;
pthread_cond_t cond1;
pthread_cond_t cond2;
pthread_cond_t cond3;
pthread_cond_t cond4;

int data=0;
int count = 0;
int num;
int num2;
int buf;
int time1 =1;
int correct = 1; // 1 false, 0 correct

int passed_vehicle = 5;
int waiting_vehicle[15] ={0};
int sequence[15] ={1,0,0,};

int time_last[4] = {-1,-1,-1,-1};



void *increase(void *arg){
	while(1){
		sleep(1);
		pthread_mutex_lock(&mutex);

		waiting_vehicle[count]=sequence[count];
		
		while(1)
		{
			
			num2 = rand()%15;
			if(waiting_vehicle[num2]!=0)
			{	
				buf = waiting_vehicle[num2];
				break;
			}
		}	

	//	printf("buf : %d\n", buf);

		


		if(buf==1)
			pthread_cond_signal(&cond1);
		else if(buf==2)
			pthread_cond_signal(&cond2);
		else if(buf==3)
			pthread_cond_signal(&cond3);
		else if(buf==4)
			pthread_cond_signal(&cond4);
		else
			break;	
		

		for(int r=0;r<4;r++)
			if(time_last[r]!=-1)
				time_last[r]=time_last[r]-1;
		



		printf("tick : %d\n",time1);
		printf("===========\n");
		printf("Passed Vehicle\n");
		printf("Car");

		for(int r=0;r<4;r++)
			if(time_last[r]==0)
			{
				printf("%d ",r);
				time_last[r]=-1;
			}

		printf("\n");
		printf("Waiting Vehicle\n");
		printf("Car");
		for(int r=0;r<num;r++)
			if(waiting_vehicle[r]!=0)
				printf("%d ",waiting_vehicle[r]);

		printf("\n");

		count++;
		time1++;
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void *printdata(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond1,&mutex);
		
		int sum =0;

		for(int g=0;g<4;g++)
		{
			if(g==3)
				;
			else
				sum+=time_last[g];
		}

		if(sum==-3)
		{
			time_last[buf] =2;
			waiting_vehicle[num2]=0;
		}
		else
			break;

		pthread_mutex_unlock(&mutex);
	}
}

void *crossroad_p2(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond2,&mutex);
		
		//time_last 0 1 2 3 = 1 2 3 4
		int sum =0;

		for(int g=0;g<4;g++)
		{
			if(g==4)
				;
			else
				sum +=time_last[g];
		}

		if(sum==-3) // go
		{
			time_last[buf]=2;
			waiting_vehicle[num2]=0;
		}
		else        // no go
			break;

		pthread_mutex_unlock(&mutex);
	}

}

void *crossroad_p3(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond3,&mutex);

		int sum =0;
		
		for(int g=0;g<4;g++)
		{
			if(g==1)
				;
			else
				sum+=time_last[g];

		}

		if(sum==-3)
		{
			time_last[buf] = 2;
			waiting_vehicle[num2] = 0;
		}
		else
			break;


		pthread_mutex_unlock(&mutex);
	}

}

void *crossroad_p4(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond4,&mutex);

		int sum =0;
		for(int g=0;g<4;g++)
		{
			if(g==2)
				;
			else
				sum+=time_last[g];
		}
		if(sum==-3)
		{
			time_last[buf] =2;
			waiting_vehicle[num2] =0;
		}
		else
			break;

		pthread_mutex_unlock(&mutex);
	}
}



int main()
{
	printf("Total number of vehicle : ");
	scanf("%d",&num);
	srand(time(NULL));

	for(int i=0;i<num;i++)
		sequence[i] = (rand()%4)+1;
	for(int i=0;i<num;i++)
		printf("%d ",sequence[i]);
	printf("\n");

	pthread_t thread1,thread2,thread3,thread4,thread5;

	pthread_mutex_init(&mutex,NULL);
	
	pthread_cond_init(&cond0,NULL);
	pthread_cond_init(&cond1,NULL);
	pthread_cond_init(&cond2,NULL);
	pthread_cond_init(&cond3,NULL);
	pthread_cond_init(&cond4,NULL);

	pthread_create(&thread1,NULL,increase,NULL);
	pthread_create(&thread2,NULL,printdata,NULL);
	pthread_create(&thread3,NULL,crossroad_p2,NULL);
	pthread_create(&thread4,NULL,crossroad_p3,NULL);
	pthread_create(&thread5,NULL,crossroad_p4,NULL);
	
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);

	return 0;
}
