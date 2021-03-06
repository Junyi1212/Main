#include "Shm.h"

//===========================================================
//shmget,shmat,shmdt, shmctl
//index==1: consumer
//index==2: producer

void ShmCase(int index)
{
    printf("%s\n", __FUNCTION__);
    if((index != 1) && (index != 2))
    {
    	printf("Enter error param:%d\n", index);
	return;
    }

    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    int shmid;
    char buffer[BUFSIZ];

    srand((unsigned int)getpid());    

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    if (shmid == -1) 
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

/* We now make the shared memory accessible to the program. */
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) 
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %p\n", (int*)shared_memory);
    shared_stuff = (struct shared_use_st *)shared_memory;

    if(index == 1)//consumer
    {
	/* The next portion of the program assigns the shared_memory segment to shared_stuff,
	 which then prints out any text in written_by_you. The loop continues until end is found
	 in written_by_you. The call to sleep forces the consumer to sit in its critical section,
	 which makes the producer wait. */
	    shared_stuff->written_by_you = 0;
	    while(running) 
	    {
	        if (shared_stuff->written_by_you) 
		 {
	            printf("You wrote: %s", shared_stuff->some_text);
	            sleep( rand() % 4 ); /* make the other process wait for us ! */
	            shared_stuff->written_by_you = 0;
	            if (strncmp(shared_stuff->some_text, "end", 3) == 0) 
		     {
	                running = 0;
	            }
	        }
	    }
    }
    else if(index == 2)//producer
    {
	    while(running) 
	    {
	        while(shared_stuff->written_by_you == 1) 
		 {
	            sleep(1);            
	            printf("waiting for client...\n");
	        }
	        printf("Enter some text: ");
	        fgets(buffer, BUFSIZ, stdin);
	        
	        strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
	        shared_stuff->written_by_you = 1;

	        if (strncmp(buffer, "end", 3) == 0) 
		 {
	                running = 0;
	        }
	    }
    }

/* Lastly, the shared memory is detached and then deleted. */
    if (shmdt(shared_memory) == -1) 
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if(index == 1)//consumer
    {
	    if (shmctl(shmid, IPC_RMID, 0) == -1) 
	    {
	        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
	        exit(EXIT_FAILURE);
	    }
    }
        
    return;
}