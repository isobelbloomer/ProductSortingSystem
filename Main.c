/*
Program: Full program
Task 1: Read in the data from all files Sort each file of production data by product weight using merge sort
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 225
#define MAXSIZE 11

//Structure templates
struct date
{
    int day;
    int hour;
    int minute;
};

typedef struct
{
    int lineCode;
    int batchCode;
    struct date BDT;
    int productID;
    char productName[50];
    float weight;
    float price;
    char targetEngineCode[20];
    int binNumber;
} product;

//circular queue implementation for task 2
typedef struct
{
    product elements[MAXSIZE];
    int head, tail, queue_size;
} circular_queue;

//TASK 1
int read_file(const char *filename, product Item[]) ;
//TASK 1
void merge(product arr[], int left, int mid, int right) ;
//TASK 1
void merge_sort(product arr[], int left, int right) ;
//TASK 1
void print_production_line(product arr[]) ;

//TASK 2
//Function to initialise empty queue
void init_queue(circular_queue *q) ;

//TASK 2
// function to check if queue is empty
int is_empty(circular_queue *q) ;

//TASK 2
//function to add an item to the queue
void enqueue(circular_queue *q, product value);

//TASK 2
//function to return an item from the queue
product dequeue(circular_queue *q);

//TASK 2
//merge 4 production lines into one dispatch list using circular queues
void merge_queues_to_dispatch_list(product line1[], int size1, product line2[], int size2, product line3[], int size3, product line4[], int size4, product merged[], int *mergedSize) ;

//TASK 2
void print_production_line2(product arr[], int count);

//TASK 3
//Function to find earliest occurence of a product with a particular weight
int binary_search(product arr[], int left, int right, float weight);

//Task 4
//Function to summarise the number of products in the delivery
void summarise_delivery(product dispatch_list[], int count);


int main()
{
    //TASK 1
    //array pointer for each file
    const char *files[4] = {"line1.txt", "line2.txt", "line3.txt", "line4.txt"};

    //One array for each production line
    product production_line1[MAXSIZE], production_line2[MAXSIZE], production_line3[MAXSIZE], production_line4[MAXSIZE];

    int count1, count2, count3, count4;

    //Read data from each file into its array
    count1 = read_file(files[0], production_line1);
    count2 = read_file(files[1], production_line2);
    count3 = read_file(files[2], production_line3);
    count4 = read_file(files[3], production_line4);

    //Sort each array by weight
    merge_sort(production_line1, 0, count1 - 1);
    merge_sort(production_line2, 0, count2 - 1);
    merge_sort(production_line3, 0, count3 - 1);
    merge_sort(production_line4, 0, count4 - 1);

    // Print results for each production Line
    printf("\n--- Sorted Production Line 1 ---\n");
    print_production_line(production_line1);

    printf("\n--- Sorted Production Line 2 ---\n");
    print_production_line(production_line2);

    printf("\n--- Sorted Production Line 3 ---\n");
    print_production_line(production_line3);

    printf("\n--- Sorted Production Line 4 ---\n");
    print_production_line(production_line4);

    //TASK 2
    // making an array to store all items from all production lines
    product dispatch_list[48] ;
    int dispatch_count;

    // merge queues to the dispatch list
    merge_queues_to_dispatch_list(production_line1, count1, production_line2, count2, production_line3, count3, production_line4, count4, dispatch_list, &dispatch_count);


    printf("\n--- Final Dispatch List (Sorted by weight) ---\n");
    print_production_line2(dispatch_list, dispatch_count);

    //TASK 3
    // ask user to enter the weight of the item they want to find
    float entered_weight;
    printf("\nPlease enter the weight of the product:");
    scanf("%f", &entered_weight); 

    // initialise an integer for the position in the array where the earliest occurence of the entered weight is
    int first_occurence;

    //binary search to find first occurence of product with particular weight
    first_occurence = binary_search(dispatch_list, 0, 47, entered_weight);

    // print details of items first occurence
    if(first_occurence >= 0)
    {
        printf("\n\n|Product ID |Product Weight");
        printf("\n|%d       |%f         ", dispatch_list[first_occurence].productID, dispatch_list[first_occurence].weight);
    } //end if
    else
    {
        printf("\n\nNo product with weight %f found.\n", entered_weight);
    } //end else

    //TASK 4
    //call function to summarise delivery
    summarise_delivery(dispatch_list, dispatch_count);

    return 0;
} // end main

//TASK 1
//function to read the files
int read_file(const char *filename, product item[])
{
    // file pointer
    FILE *file = fopen(filename, "r");

    //if = NULL
    if(!file)
    {
        perror("\nError opening file\n");
        //end program
        return 0;
    } //end if

    char line[MAX_LINE_LENGTH] ;
    int count = 0 ;

    
    //read each line into array
    while(fgets(line, sizeof(line), file) && count < MAXSIZE)
    {
        product issue; 
        
        // parse line using sscanf
        sscanf(line, "%d,%d,%d,%d,%d,%d,%49[^,],%f,%f,%19[^,],%d", &issue.lineCode, &issue.batchCode, &issue.BDT.day, &issue.BDT.hour, &issue.BDT.minute, &issue.productID, &issue.productName[50], &issue.weight, &issue.price, &issue.targetEngineCode[50], &issue.binNumber);
        item[count++] = issue ;
    } //end while

    //Close file
    fclose(file);
    return count;
  
} //end function

//TASK 1
// function to sort arr of items
//recursively divides array into smaller arrays until it reaches base case then merges back in sorted order
void merge_sort(product arr[], int left, int right)
{
    if(left < right)
    {
        //initialising middle of array
        int mid = left + (right - left) / 2 ;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right) ;
    } // end if
} //end function

//TASK 1
//function to merge arrays
// merges two sorted subarrays
void merge(product arr[], int left, int mid, int right)
{
    int i, j, k;
    int leftLength = mid - left + 1; //Length of left side of the array
    int rightLength = right - mid ; //Length of right side of the array

    // temp arrays
    product leftArr[leftLength], rightArr[rightLength] ;

    //copying data to temp arrays
    for(i = 0; i < leftLength; i++)
    {
        leftArr[i] = arr[left + i] ;
    } //end for
    for(j = 0; j < rightLength; j++)
    {
        rightArr[j]  = arr[mid + 1 + j] ;
    } //end for
    
    //Initialise indexes for arrays
    i = 0;
    j = 0;

    //Initialise index for merged array
    k = left;

    // Merge the temp arrays back
    while(i < leftLength && j < rightLength)
    {
        // if left weught is then put it into arr
        if(leftArr[i].weight < rightArr[j].weight)
        {
            arr[k] = leftArr[i] ;
            i++ ;
        }
        // Otherwise put the element from the right into arr
        else
        {
            arr[k] = rightArr[j] ;
            j++ ;
        }
        //move onto next part of array
        k++ ;
    } //end while

    //Copy remaining elements from the right temp array into arr
    while(i < leftLength)
    {
        arr[k] = leftArr[i] ;
        i++ ;
        k++ ;
    } //end while

    // Copy the remaining elements from the left temp array into arr
    while(j < rightLength)
    {
        arr[k] = rightArr[j] ;
        j++ ;
        k++ ;
    } //end while
} //end function

//TASK 1
//Function to print production lines
void print_production_line(product arr[])
{
    printf("\n|Product id| Product weight\n");
    for(int i = 0; i < 12; i++)
    {
        printf("|%d      |%.2f \n", arr[i].productID, arr[i].weight);
    } //end for 

} //end function

//TASK 2
// function to merge the lists into one dispatch list
void merge_queues_to_dispatch_list(product line1[], int size1, product line2[], int size2, product line3[], int size3, product line4[], int size4, product merged[], int *merged_size)
{
    //Initialise circular queues for each production line

    circular_queue q1, q2, q3, q4;

    init_queue(&q1); 
    init_queue(&q2);
    init_queue(&q3);
    init_queue(&q4);

    // Load each production line into its queue
    for(int i = 0; i < size1; i++)
    {
        enqueue(&q1, line1[i]);
    }
    for(int i = 0; i < size2; i++)
    {
        enqueue(&q2, line2[i]);
    }
    for(int i = 0; i < size3; i++)
    {
        enqueue(&q3, line3[i]);
    }
    for(int i = 0; i < size4; i++)
    {
        enqueue(&q4, line4[i]);
    }

    int index = 0;

    while(!is_empty(&q1) || !is_empty(&q2) || !is_empty(&q3) || !is_empty(&q4))
    {
        product *min = NULL;

        if(!is_empty(&q1))
        {
            min = &q1.elements[q1.head] ;
        }
        if(!is_empty(&q2) && (!min || q2.elements[q2.head].weight < min -> weight))
        {
            min = &q2.elements[q2.head];
        }
        if(!is_empty(&q3) && (!min || q3.elements[q3.head].weight < min -> weight))
        {
            min = &q3.elements[q3.head];
        }
        if(!is_empty(&q4) && (!min || q4.elements[q4.head].weight < min -> weight))
        {
            min = &q4.elements[q4.head];
        }

        if(min)
        {
            //Copy min item into merged list
            merged[index++] = *min;

            //Remove from the correct queue
            if(min == &q1.elements[q1.head])
            {
                dequeue(&q1);
            }
            else if(min == &q2.elements[q2.head])
            {
                dequeue(&q2);
            }
            else if(min == &q3.elements[q3.head])
            {
                dequeue(&q3);
            }
            else if(min == &q4.elements[q4.head])
            {
                dequeue(&q4);
            }
        } //end if

        *merged_size = index;
    } //end while
    
} //end function

//TASK 2
//Function to initialise empty queue
void init_queue(circular_queue *q)
{
    q -> head = -1;
    q -> tail = -1;
    q -> queue_size = 0;
} //end function

//TASK 2
//Function to check if queue is empty
int is_empty(circular_queue *q)
{
    return(q -> queue_size == 0);
} //end function

//TASK 2
//Function to add a production item to the queue
void enqueue(circular_queue *q, product value)
{
    if(q -> queue_size == MAXSIZE)
    {
        return; // queue full
    }

    if(is_empty(q))
    {
        q -> head = 0;
        q -> tail = 0;
    }
    else
    {
        q -> tail = (q -> tail + 1) % MAXSIZE;
    }

    q -> elements[q -> tail] = value;
    q -> queue_size++;
} // end function

//TASK 2
//Function to return a production item from the queue
product dequeue(circular_queue *q)
{
    product empty = {0};

    if(is_empty(q))
    {
        return empty;
    }

    product value = q -> elements[q -> head];

    if(q -> head == q -> tail)
    {
        q -> head = -1;
        q -> tail = -1;
    }
    else
    {
        q -> head = (q -> head + 1) % MAXSIZE;
    }

    q -> queue_size-- ;
    return value;
} //end function

//TASK 2
void print_production_line2(product arr[], int count)
{
    printf("\n|Product id| Product weight\n");
    for(int i = 0; i < count; i++)
    {
        printf("|%-10d   |%-13.2f \n", arr[i].productID, arr[i].weight);
    }
} //end function

//TASK 3
//Function to find earliest occurence of a product with a particular weight
//recursively cuts the search range in half each time
int binary_search(product arr[], int left, int right, float weight)
{
    int mid;

    //checks middle element of curent search range
    if(right >= left)
    {
        mid = left + (right - left) / 2;

        //if value is found it returns the index
        if(arr[mid].weight == weight)
        {
            return mid;
        }
        //if its less it searches the left half
        else if(arr[mid].weight > weight)
        {
            //search to left
            return binary_search(arr, left, mid - 1, weight);
        }
        //if its greater then the middle element it searches the right half
        else
        {
            //search to right
            return binary_search(arr, mid + 1, right, weight);
        }
    }

    // if weight isnt found 
    return -1;
} // end function


//TASK 4
//Function to summarise the number of products in the delivery
void summarise_delivery(product dispatch_list[], int count)
{
    printf("\n--- Delivery Summary Report ---\n");
    printf("Total number of products in delivery: %d\n\n", count);
} //end function