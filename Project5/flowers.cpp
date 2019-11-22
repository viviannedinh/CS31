#include "utilities.h"
#include <iostream>
#include <cstring>
using namespace std;

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);

const char WORDFILENAME[] = "/Users/vdinh/Documents/wordfile.txt";

int main()
{
    // fill w array with words
    // count how many elements in array
    char w[9000][7];
    int n = getWords(w, 9000, WORDFILENAME);
    if (n < 1)
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 0;
    }
    
    // get user input for number of rounds
    int numRounds;
    cout << "How many rounds do you want to play? ";
    cin >> numRounds;
    cout << endl;
    cin.ignore(10000, '\n');
    
    // output error message if invalid number of rounds inputted
    if (numRounds < 1)
    {
        cout << "The number of rounds must be positive." << endl;
        return 0;
    }
    
    int min = 0;
    int max = 0;
    double total = 0;
    
    for (int i = 1; i <= numRounds; i++)
    {
        cout << "Round " << i << endl;
        
        // obtain random int for position of mystery word
        int wordPos = randInt(0, n-1);
        cout << "The mystery word is " << strlen(w[wordPos]) << " letters long." << endl;
        int score = playOneRound(w, n, wordPos);
        
        // set precision
        cout.setf(ios::fixed);
        cout.precision(2);
        
        //calculate average
        total += score;
        double average = total/i;
        
        // output tries it took
        if (score == 1)
            cout << "You got it in 1 try." << endl;
        else
            cout << "You got it in " << score << " tries." << endl;
        
        // calculate minimum and maximum
        if (i == 1)
        {
            min = score;
            max = score;
        }
        else
        {
            if (score > max)
                max = score;
            if (score < min)
                min = score;
        }
        
        // output average, minimum, and maximum
        cout << "Average: " << average << ", minimum: " << min << ", maximum: " << max << endl << endl;
    }
}

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    if (nWords < 1 || wordnum < 0 || wordnum >= nWords)
        return -1;
    
    int score = 0;
    
    while (true)
    {
        // get trial word from user
        char trial[100];
        cout << "Trial word: ";
        cin.getline(trial, 100);
        
        // check if trial word is in array of cstrings
        bool inArray = false;
        for (int k = 0; k < nWords; k++)
            if (!strcmp(trial, words[k]))
                inArray = true;
        
        // check if all characters in cstring are lower case
        bool isLower = true;
        for (int j = 0; j < strlen(trial); j++)
            if (isupper(trial[j]))
                isLower = false;
        
        // print error message if trial word is invalid
        if (strlen(trial) > 6 || strlen(trial) < 4 || isLower == false)
            cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
        else if (!inArray)
            cout << "I don't know that word." << endl;
        else
        {
            score++;
            int flowers = 0;
            int bees = 0;
            
            char copy[10];
            strcpy(copy, words[wordnum]);
            
            // compare trial word with mystery word
            // count bees and flowers
            for (int l = 0; l < strlen(trial); l++)
            {
                if (trial[l] == copy[l])
                {
                    flowers++;
                    copy[l] = ' ';
                }
                else
                    for (int m = 0; m < strlen(trial); m++)
                        if (trial[l] == copy[m])
                        {
                            bees++;
                            copy[m] = ' ';
                            break;
                            
                        }
            }
            
            // output flowers and bees if trial word is wrong
            // return score if trial word is correct
            if (strlen(words[wordnum]) == flowers)
                return score;
            else
                cout << "Flowers: " << flowers << ", Bees: " << bees << endl;
        }
    }
}
