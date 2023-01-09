
#include<stdio.h>
#include<pthread.h>

// Number of philosophers
#define N 5

// Helper macros to get the left and right fork indices
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N

// Philosopher states
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// State of each philosopher
int state[N];

// Mutex and condition variables for the monitor
pthread_mutex_t mutex;
pthread_cond_t cond[N];

// Function to test if a philosopher can start eating
void test(int i)
{
    // Check if the philosopher is hungry and has both forks available
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        // Set the philosopher's state to eating
        state[i] = EATING;

        // Signal the condition variable to wake up the philosopher
        pthread_cond_signal(&cond[i]);
    }
}

// Function for a philosopher to pick up forks
void pickup_forks(int i)
{
    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Set the philosopher's state to hungry
    state[i] = HUNGRY;
    printf("Philosopher %d is hungry\n", i);

    // Try to acquire the forks
    test(i);

    // If the philosopher can't acquire the forks, wait on the condition variable
    while (state[i] == HUNGRY)
        pthread_cond_wait(&cond[i], &mutex);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
}

// Function for a philosopher to return forks
void return_forks(int i)
{
    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Set the philosopher's state to thinking
    state[i] = THINKING;
    printf("Philosopher %d is thinking\n", i);

    // Return the forks and check if neighbors can start eating
    test(LEFT);
    test(RIGHT);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
}

// Function for the philosopher thread
void *philosopher(void *arg)
{
    // Get the philosopher index
    int i = *((int*)arg);

    // Number of meals eaten by the philosopher
    int num_meals = 0;

    // Repeat until the philosopher has eaten 3 meals
    while (num_meals < 3)
    {
        // Pick up the forks and eat
        pickup_forks(i);
        printf("Philosopher %d is eating\n", i);
        return_forks(i);

        // Increment the number of meals eaten
        num_meals++;
    }

    // Print a message when the philosopher has finished dining
    printf("Philosopher %d has finished dining\n", i);
}

int main()
{
    // Initialize the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < N; i++)
        pthread_cond_init(&cond[i], NULL);

    // Array of philosopher threads
    pthread_t threads[N];

    // Array of philosopher indices
    int ids[N];

    // Create the philosopher threads
    for (int i = 0; i < N; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, (void*)&ids[i]);
    }

    // Wait for the philosopher threads to finish
    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    // Destroy the mutex and condition variables
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < N; i++)
        pthread_cond_destroy(&cond[i]);

    // Print a message when all philosophers have finished dining
    printf("All philosophers have finished dining\n");

    return 0;
}
