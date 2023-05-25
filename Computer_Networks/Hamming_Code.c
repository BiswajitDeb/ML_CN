#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define bool int
#define MAX 100

//Binary int array to store the decimal number in binary form
int binaryArray[MAX];

int idx = 1;
int r = 0;

//function to check if the number is power of 2
bool is_power_2(int i)
{
	return (i != 0) && ((i & (i - 1)) == 0);
}

//function that generates power of 2
int power_of_2(int i)
{
    if(i == 0)
        return 1;

    int p = 1;
    for(int k = 0; k < i; ++k)
        p *= 2;

    return p;
}

//function that generates the i th power of 2

//Function to convert decimal to binary and storing it in binaryArray
void decimal_to_binary(int n)
{
    //A variable to store
    while(n != 0)
    {
        int remainder = n % 2;

        if(idx == 1)
        {
            ++r;
            ++idx;
            continue;
        }

        if(is_power_2(idx))
        {
            ++r;
            ++idx;
        }

        binaryArray[idx] = remainder;

        n = n / 2;
        ++idx;
    }
}

void generate_hamming_code(int noOfRow, int noOfCol, int numberStore[][noOfCol])
{
    for(int col = 0; col < noOfCol; ++col)
    {
        int targetIndex = power_of_2(col);

        int xor_value = 0;

        for(int row = 1; row <= noOfRow; ++row)
        {
            if(row == targetIndex)
                continue;

            else
            {
                if(numberStore[row][col])
                    xor_value ^= binaryArray[row];
            }
        }

        binaryArray[targetIndex] = xor_value;
    }
}

void print()
{
    for(int k = idx; k >= 1; --k)
    {
        if(binaryArray[k] == -1)
            continue;

        printf("%d ", binaryArray[k]);
    }
    printf("\n");
}

int main()
{
    //Initializing all values of the binaryArray to -1
    //***************************//
    for(int i = 0; i < MAX; ++i)
        binaryArray[i] = -1;
    //***************************//

    int n;

    char input;
    printf("Type of input:\n");
    printf("i)  'd' for decimal\n");
    printf("ii) 'b' for binary\n");

    scanf("%c", &input);

    switch(input)
    {
    default:
        printf("INVALID INPUT CHOICE!!! TERMINATING PROGRAM\n");
        exit(0);

    case 'd':
    case 'D':
        printf("Enter a message(DECIMAL NUMBER): ");
        scanf("%d", &n);

        //Function to convert decimal to binary and storing it in binaryArray
        decimal_to_binary(n);

        //As for the last bit the index will increase by 1,
        //so we are decreasing it by 1
        idx = idx - 1;

        break;

    case 'b':
    case 'B':
        printf("Enter size of input: ");
        scanf("%d", &n);

        printf("Enter bits in from of LSB -> MSB\n");
        while(n != 0)
        {
            if(idx == 1)
            {
                ++r;
                ++idx;
                continue;
            }

            if(is_power_2(idx))
            {
                ++r;
                ++idx;
            }

            printf("Enter %d bit value: ", idx - r);
            scanf("%d", &binaryArray[idx]);

            --n;
            ++idx;
        }

        idx = idx - 1;
        break;
    }

    printf("\nThe Input in binary form is: ");
    print();

    //Since we are skipping all the 2 power's indexes, counting
    //the number of -1 up to the index will give us the value of r
    //which will satisfy 2^r >= m + r + 1.
    //This is done while forming the binaryArray itself.
    printf("\nThe value of r: %d\n", r);

    //Creating a matrix that will store the binary values from 1 to idx
    int numberStore[idx + 1][r];

    //Initializing all the 2D elements to be 0
    //************************************
    for(int row = 0; row <= idx; ++row)
    {
        for(int col = 0; col < r; ++col)
            numberStore[row][col] = 0;
    }
    //************************************

    //Generating the binary values for all number from 1 to idx
    for(int row = 1; row <= idx; ++row)
    {
        int col = 0;
        int currentRowNumber = row;

        while(currentRowNumber != 0)
        {
            int remainder = currentRowNumber % 2;
            numberStore[row][col++] = remainder;
            currentRowNumber = currentRowNumber / 2;
        }
    }
    //***********************************************************

    //Generating the hamming code
    generate_hamming_code(idx, r, numberStore);
    printf("\n");

    //storing the hamming code in a separate array
    int hammingCode[idx];
    for(int k = idx; k >= 1; --k)
        hammingCode[k - 1] = binaryArray[k];

    printf("HAMMING CODE FOR THE ENTERED MESSAGE - SUCCESS!\n");

    printf("Hamming code: ");
    for(int j = idx - 1; j >= 0; --j)
        printf("%d ", hammingCode[j]);
    printf("\n");

    return 0;
}
