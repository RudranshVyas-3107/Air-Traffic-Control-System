#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <sys/ipc.h>

#include <sys/shm.h>
#include <pthread.h>
#include <sys/msg.h>

#include <stdbool.h>

#define MAXLEN 512

pthread_mutex_t runways[11] = {PTHREAD_MUTEX_DEFAULT};
key_t key;
int msgid;

// struct msqid_ds {
//     struct ipc_perm msg_perm;    /* Ownership and permissions */
//     time_t msg_stime;            /* Time of last msgsnd(2) */
//     time_t msg_rtime;            /* Time of last msgrcv(2) */
//     time_t msg_ctime;            /* Time of last change */
//     unsigned long msg_qnum;      /* Number of messages currently on queue */
//     unsigned long msg_qbytes;    /* Maximum number of bytes allowed on queue */
//     pid_t msg_lspid;             /* PID of last msgsnd(2) */
//     pid_t msg_lrpid;             /* PID of last msgrcv(2) */
// };

struct planeInfoDeparted
{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
};
struct planeInfoDepart
{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
    // int arriveOrDepart;
};
struct thread{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
    int airPortNumber;
    int numberOfRunways;
    int arr[4];
    // int arriveOrDepart;

};

void *depart(void *bal)
{
   // printf("threaddepart\n");
    //struct thread *myTTD;
    struct thread *myTTD = (struct thread*)bal;
    //struct thread *ths;
    //int *val_p = (int *)param;
    int n = sizeof(myTTD->arr) / 4;
    int small = myTTD->arr[0];
    int index = 0;
    //printf("runways %d\n",myTTD->numberOfRunways);
    //printf("threaddepart %d\n",myTTD->airportDearture);

    
    
    

    //printf("val_p[n-7]: %d",val_p[1]);

    struct planeInfoDeparted deep;
    for (int i = 0; i < myTTD->numberOfRunways; i++)
    {

        if (myTTD->arr[i] > myTTD->totalPlaneWeight)
        {
            if(small<myTTD->totalPlaneWeight){
                small=myTTD->arr[i];
                index = i;
            }
            else if(small>myTTD->arr[i]){
                small=myTTD->arr[i];
                index = i;
            }
            
            
        }
    }

    // deep.airPortArrival = val_p[n - 4];
    // deep.airportDearture = val_p[n - 3];
    // deep.mtype = val_p[n - 3] + 10;
    // deep.numberOfPassangers = val_p[n - 1];
    // deep.planeId = val_p[n - 5];
    // deep.totalPlaneWeight = val_p[n - 7];
    // deep.typeOfPlane = val_p[n - 2];

    deep.airPortArrival=myTTD->airPortArrival;
    deep.airportDearture=myTTD->airportDearture;
    deep.numberOfPassangers=myTTD->numberOfPassangers;
    deep.planeId=myTTD->planeId;
    deep.totalPlaneWeight=myTTD->totalPlaneWeight;
    deep.typeOfPlane=myTTD->typeOfPlane;

   // printf("departthread %d\n",myTTD->airPortArrival);
    //printf("plane arrival %d\n",deep.airPortArrival);
    //struct msqid_ds msq_stat;

    
    

    if (small >= myTTD->totalPlaneWeight)
    {
        
            deep.mtype=myTTD->airportDearture+20;
            

            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has completed boarding/loading and taken off from Runway No. %d of Airport No. %d\n", deep.planeId, index + 1, myTTD->airPortNumber);
           

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                 printf("error in sending message \n");
                exit(1);
            }
            // printf("plane arrival %d\n",deep.airPortArrival);
            pthread_mutex_unlock(&runways[index]);
           // printf("afterlock\n");
            pthread_exit(0);
            //printf("after end\n");
        }
        else
        {
           //printf("plane arrival2 %d\n",deep.airPortArrival); 
            deep.mtype=myTTD->airportDearture+20;
            index=myTTD->numberOfRunways+1;
            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has completed boarding/loading. %d of Airport No. %d \n", deep.planeId, index + 1, myTTD->airPortNumber);

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                 printf("error in sending message \n");
                exit(1);
            }
    //         if (msgrcv(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), deep.mtype, 0) != -1)
    // {
        
    //     // printf("error in receiving message\n");
    //     // exit(1);
    //     printf("success\n");
        
    // }
            //printf("deep mtype : %ld\n",deep.mtype);
            //printf("plane arrival3 %d\n",deep.airPortArrival);

    //         if (msgctl(msgid, IPC_STAT, &msq_stat) == -1) {
    //     perror("msgctl");
    //     exit(EXIT_FAILURE);
    // }

    // Print the number of messages in the queue
    //printf("Number of messages in the queue: %ld\n", msq_stat->msg_qnum);

            pthread_mutex_unlock(&runways[index]);

            pthread_exit(0);
        }
    }
    

void* Arrive(void*param)
{
    //printf("threadarrive\n");
    struct thread *myTTD;
    myTTD = (struct thread*)param;
    struct thread *ths;
    //int *val_p = (int *)param;
    int n = sizeof(myTTD->arr) / 4;
    int small = myTTD->arr[0];
    int index = 0;

    
    
    

    //printf("val_p[n-7]: %d",val_p[1]);

    struct planeInfoDeparted deep;
   for (int i = 0; i < myTTD->numberOfRunways; i++)
    {

        if (myTTD->arr[i] > myTTD->totalPlaneWeight)
        {
            if(small<myTTD->totalPlaneWeight){
                small=myTTD->arr[i];
                index = i;
            }
            else if(small>myTTD->arr[i]){
                small=myTTD->arr[i];
                index = i;
            }
            
            
        }
    }

    // deep.airPortArrival = val_p[n - 4];
    // deep.airportDearture = val_p[n - 3];
    // deep.mtype = val_p[n - 3] + 10;
    // deep.numberOfPassangers = val_p[n - 1];
    // deep.planeId = val_p[n - 5];
    // deep.totalPlaneWeight = val_p[n - 7];
    // deep.typeOfPlane = val_p[n - 2];

    deep.airPortArrival=myTTD->airPortArrival;
    deep.airportDearture=myTTD->airportDearture;
    deep.numberOfPassangers=myTTD->numberOfPassangers;
    deep.planeId=myTTD->planeId;
    deep.totalPlaneWeight=myTTD->totalPlaneWeight;
    deep.typeOfPlane=myTTD->typeOfPlane;

    if (small >= myTTD->totalPlaneWeight)
    {
        
            deep.mtype=myTTD->airPortArrival+40;

            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has completed deboarding/deloading and landed off from Runway No. %d of Airport No. %d\n", deep.planeId, index + 1, myTTD->airPortNumber);

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }
            pthread_mutex_unlock(&runways[index]);
            pthread_exit(0);
        }
        else
        {
            deep.mtype=myTTD->airportDearture+40;
            index=myTTD->numberOfRunways+1;
            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has landed on Runway. %d of Airport No. %d and has completed deboarding/unloading.\n", deep.planeId, index + 1, myTTD->airPortNumber);

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }

            pthread_mutex_unlock(&runways[index]);

            pthread_exit(0);
        }
    }





// void *arrive(void *param)
// {
//     int *val_p = (int *)param;
//     int n = sizeof(val_p) / 4;
//     int small = val_p[0];
//     int index = 0;
//     for (int i = 0; i < n - 3; i++)
//     {

//         if (val_p[i] > val_p[n - 7] && small > val_p[i])
//         {
//             small = val_p[i];
//             index = i;
//         }
//     }
//     if (small > val_p[n - 7])
//     {

//             pthread_mutex_lock(&runways[index]);
//         sleep(3);
//         printf("Plane %d has landed on Runway. %d of Airport No. %d and has completed deboarding/unloading.", val_p[n - 5], val_p[index + 1], val_p[n - 6]);

//         if (msgsnd(msgid, (void *)&deep, MAXLEN, 0) == -1)
//         {
//             // printf("error in sending message %s",buf);
//             exit(1);
//         }

//         pthread_mutex_unlock(&runways[index]);

//         pthread_exit(0);
//     }
//     else
//     {
//         sleep(5);

//         if (msgsnd(msgid, (void *)&deep, MAXLEN, 0) == -1)
//         {
//             // printf("error in sending message %s",buf);
//             exit(1);
//         }

//         printf("plane landed boarding/loading from special runway");
//         pthread_exit(0);
//     }
// }

int main()
{
    int airPortNumber;
    int numberOfRunways;

    printf("Enter Airport Number : \n");
    scanf("%d", &airPortNumber);

    printf("Enter number of Runways : \n");
    scanf("%d", &numberOfRunways);

    // argc=numberOfRunways;

    int size=0;

    // int *nums = (int *)malloc((argc - 1) * sizeof(int));
    // int i = 1;
    // for (i = 1; i < argc; i++)
    // {
    //     nums[i - 1] = atoi(argv[i]);
    //     size++;
    // }

    int a[numberOfRunways];
    printf("Enter loadCapacity of Runways (give as a space separated list as a single line): \n");
    for (int i = 0; i < numberOfRunways; i++)
    {
        scanf("%d", &a[i]);
    }

    key_t key;
    int msgid;
    struct planeInfoDepart planeRecDepart;

    key = ftok("plane.c", 'A');
    if (key == -1)
    {
        printf("error in creating unique key\n");
        exit(1);
    }

    msgid = msgget(key, 0644 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("error in creating message queue\n");
        exit(1);
    }

    pthread_t pla[10];
    pthread_t t;
    
    pthread_attr_t attr;
    struct thread *bal=malloc(sizeof(struct thread));
pthread_attr_init(&attr);

    int x = 0;

    int l = 0;

    //  if (msgrcv(msgid, (void *)&planeRecDepart, sizeof(struct planeInfoDepart), airPortNumber + 10, 0) == -1)
    //     {
    //         printf("error in receiving message\n");
    //         exit(1);
    //     }
    //     printf("%d\n",planeRecDepart.planeId);
   // printf("hello\n");

    for (int s = 0; s < numberOfRunways; s++)
        {
            bal->arr[s] = a[s];
        }
        


    while (1)
    {
        l++;
        //printf("hello%d\n",l);

        
        

         //bal = (struct thread*)malloc(sizeof *bal);

        
        
        


        //int argv[numberOfRunways+7];
        
        //printf("gautam%d\n",bal->arr[0]);
        

        if (msgrcv(msgid, (void *)&planeRecDepart, sizeof(struct planeInfoDepart), airPortNumber+10, 0) == -1)
        {
            printf("error in receiving message\n");
            exit(1);
        }
        if(planeRecDepart.typeOfPlane==3){break;}
        bal->airPortArrival=planeRecDepart.airPortArrival;
        bal->airportDearture=planeRecDepart.airportDearture;
        bal->mtype=planeRecDepart.mtype;
        bal->numberOfPassangers=planeRecDepart.numberOfPassangers;
        bal->totalPlaneWeight=planeRecDepart.totalPlaneWeight;
        bal->planeId=planeRecDepart.planeId;
        bal->typeOfPlane=planeRecDepart.typeOfPlane;
        bal->airPortNumber=airPortNumber;
        bal->numberOfRunways=numberOfRunways;
        //printf("gautam : %d\n",planeRecDepart.airportDearture);
        if(planeRecDepart.airportDearture==airPortNumber){
            pthread_create(&pla[x], NULL, depart, (void *)bal);
        x++;

        }
        else{
             pthread_create(&pla[x], NULL, Arrive, (void *)bal);
              x++;
        }
        // if (msgrcv(msgid, (void *)&planeRecDepart, sizeof(struct planeInfoDepart), airPortNumber+30, 0) == -1)
        // {
        //     printf("error in receiving message\n");
        //     exit(1);
        // }
        // bal->airPortArrival=planeRecDepart.airPortArrival;
        // bal->airportDearture=planeRecDepart.airportDearture;
        // bal->mtype=planeRecDepart.mtype;
        // bal->numberOfPassangers=planeRecDepart.numberOfPassangers;
        // bal->totalPlaneWeight=planeRecDepart.totalPlaneWeight;
        // bal->planeId=planeRecDepart.planeId;
        // bal->typeOfPlane=planeRecDepart.typeOfPlane;
        // bal->airPortNumber=airPortNumber;
        // bal->numberOfRunways=numberOfRunways;
        // if(planeRecDepart.mtype>30&&planeRecDepart.mtype<40){
        //     pthread_create(&pla[x], NULL, Arrive, (void *)bal);
        // x++;

        // }
        

        // printf("hello\n");
        
        // printf("%d\n", planeRecDepart.planeId);

       
        

        // argv[numberOfRunways] = planeRecDepart.totalPlaneWeight;
        // argv[numberOfRunways + 1] = airPortNumber;
        // argv[numberOfRunways + 2] = planeRecDepart.planeId;
        // argv[numberOfRunways + 3] = planeRecDepart.airPortArrival;
        // argv[numberOfRunways + 4] = planeRecDepart.airportDearture;
        // argv[numberOfRunways + 5] = planeRecDepart.typeOfPlane;
        // argv[numberOfRunways + 6] = planeRecDepart.numberOfPassangers;

       // printf("%d\n",argv[numberOfRunways+6]);

        // if (planeRecDepart.airportDearture == airPortNumber)
        // {

        

        //}

        // else
        // {
        // pthread_create(&pla[x++], NULL, arrive, (void *)argv);
        //}

        // if (planeRecDepart.arriveOrDepart == 0)
        // {

        //     pthread_create(&pla[x++], NULL, depart, (void *)argv);
        // }

        // else
        // {
        //     pthread_create(&pla[x++], NULL, arrive, (void *)argv);
        // }
    }

    for (int m = 0; m < x; m++)
    {
        pthread_join(pla[m], NULL);
    }
}