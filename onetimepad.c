/*************************
 *
 * Project Name: One Time, One Time
 * Description: Write a One Time Pad encryption system
 * File names: onetimepad.c
 * Date: February 16, 2018
 * Authors: Niklas Byriel, Muntabir Choudhury
 *
 ***********************/

#include <stdio.h>

//the maximum read length of a message
#define MAX_LENGTH 2048


int bits(unsigned char character);
unsigned char rotate(unsigned char character, int count);
int readInput(unsigned char array[]);
void printArray(unsigned char array[],int size);
void tileKey(unsigned char key[],int messageLength,int keyLength);
void chain(unsigned char key[],int messageLength);
void encrypt(unsigned char message[], unsigned char key[], int messageLength);

int main(){
  unsigned char message[MAX_LENGTH]; //array storing the message
  unsigned char key[MAX_LENGTH]; //array storing the key
  int messageLength = readInput(message); //stores length of message
  int keyLength = readInput(key); //stores length of key initially
 
  tileKey(key,messageLength,keyLength);

  chain(key, messageLength);

  encrypt(message, key, messageLength);

  printArray(message,messageLength);
  
  return 0;
}

/*
 *Description: XORs every item in the message with the key to encrypt the message, changing the message array
 *
 *Parameters: message[] - the message array
 *            key[] - the key array
 *            messageLength - the length of the message
 */
void encrypt(unsigned char message[], unsigned char key[], int messageLength){

  //loops through all items in the array
  for (int i = 0; i < messageLength; i++){
    message[i] = message[i] ^ key[i];
  }
}
/*
 *Description: rotates each item in the key
 *
 *Parameters: key[] - the key array
 *            messageLength - the length of the message, now also length of key
 */
void chain(unsigned char key[],int messageLength){
  int sum=0;
  //runs through each item in the key
  for (int i = 0; i < messageLength; i++){
    //Checks if i is the first item in the array
    if (i == 0){
      sum+=key[messageLength-1];
      key[i] = rotate(key[i] ^ key[sum], bits(key[messageLength-1]));
    }
    else {
      key[i] = rotate(key[i] ^ key[sum], bits(key[i-1]));
    }
    sum += key[i];
    sum = sum % messageLength;
  }
}

/*
* Description: tiles the key to make it same size as message
*
* Parameters: key[] - array storing the key
*             messageLength - the length of the message
*             keyLength - the length of they key before tiling
*/
void tileKey(unsigned char key[],int messageLength,int keyLength){
  int i = 0;
  //Makes the key the same length as the message
  for (i = 0; i < messageLength; i++){
    key[i] = key[i % keyLength];
  }
}

/*
 * Description: counts the 1 bits in a character
 * 
 * Parameters: character - the character whose length is to be found
 *
 * Returns: number of 1 bits in the character
 */
int bits(unsigned char character){
  int count=0;
  int i;
  //loops through each bit in the character to test if it is 1
  for (i = 0; i < 8; i++, character = character >> 1){
    //checks if the last bit is equal to 1
    if((character | 1) == character)
      count++;
  }
  return count;
}
/*
 * Description: rotates a character by count bits
 *
 * Parameters: character - the original character
 *             count - the number of bits to be rotated by
 *
 * Returns: the rotated character
 */
unsigned char rotate(unsigned char character, int count){
  count = count % 7;
  char lastBits = character & (255 >> (8-count));
  return ((lastBits << (7-count)) | (character >> count));
}
/*
 * Description: reads input until EOF or delimiter
 *
 * Parameters: array[] - the array to put the input in
 *
 * Returns: the number of characters inputted
 */
int readInput(unsigned char array[]){
  char c;
  int count = 0;
  //Reads either the first MAX_LENGTH worth of characters inputted, or until an EOF or delimiter
  while ((c = getchar()) != 255 && c != EOF && count < MAX_LENGTH){
    array[count] = c;
    count++;
  }
  int n = count;
  //Fills the rest of the array with 0, if array is shorter than MAX_LENGTH
  while (n < MAX_LENGTH){
    array[n] = 0;
    n++;
  }
  //tests if array is longer than max length and if the last character is not the delimiter or EOF
  if (count >= MAX_LENGTH && c != 255 && c != EOF){
    //grabs the next character to clear input stream until 255 or EOF is reached
    while ((c = getchar()) != 255 && c != EOF){}
  }
  return count;
}

/*
 * Description: outputs an array
 * 
 * Parameters: array[] - the array to be printed
 *             size - the length of the array
 */
void printArray(unsigned char array[],int size){
  int count;
  //runs through the entire array
  for (count = 0; count < size; count++){
    putchar(array[count]);
  }
}
