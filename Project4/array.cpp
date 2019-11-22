#include <iostream>
using namespace std;

// prototype functions
int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int main()
{
    //testing functions
    string people[5] = { "donald", "lindsey", "fiona", "rudy", "mick" };
    int j = appendToAll(people, 5, "!!!");  // returns 5
    cout << j << endl;

    string officeholders[5] = { "donald", "lindsey", "mike", "adam", "nancy" };
    int z = lookup(officeholders, 5, "adam");  // returns 3
    cout << z << endl;
    
    string persons[6] = { "donald", "lindsey", "marie", "rudy", "fiona", "adam" };
    int k = positionOfMax(persons, 6);   // returns 3
    cout << k << endl;
    
    string politician[5] = { "mike", "donald", "lindsey", "nancy", "adam" };
    int m = rotateLeft(politician, 5, 1);  // returns 1
    cout << m << endl;
    
    string d[9] = {"rudy", "adam", "mike", "mike", "fiona", "fiona", "fiona", "mike", "mike"};
    int p = countRuns(d, 9);  //  returns 5
    cout << p << endl;
    
    string coworkers[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
    int q = flip(coworkers, 5);  // returns 5
    cout << q << endl;
    
    string folks[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
    string group[6] = { "adam", "", "fiona", "donald", "mike", "rudy" };
    int r = differ(folks, 7, group, 6);  //  returns 3
    int s = differ(folks, 2, group, 3);  //  returns 2
    cout << r << " " << s << endl;
    
    string names[10] = { "gordon", "marie", "nancy", "mick", "adam", "lindsey" };
    string names1[10] = { "marie", "nancy", "mick" };
    int t = subsequence(names, 6, names1, 3);  // returns 1
    string names2[10] = { "gordon", "mick" };
    int u = subsequence(names, 5, names2, 2);  // returns -1
    cout << t << " " << u << endl;
    
    string name4[10] = { "gordon", "marie", "nancy", "mick", "adam", "lindsey" };
    string set1[10] = { "donald", "adam", "mick", "marie" };
    int v = lookupAny(name4, 6, set1, 4);  // returns 1 (a1 has "marie" there)
    string set2[10] = { "rudy", "fiona" };
    int w = lookupAny(name4, 6, set2, 2);  // returns -1 (a1 has none)
    cout << v << " " << w << endl;
    
    string persona[6] = { "donald", "lindsey", "marie", "rudy", "fiona", "adam" };
    int x = separate(persona, 6, "gordon");  //  returns 3
    string persons2[4] = { "marie", "nancy", "lindsey", "mike" };
    int y = separate(persons2, 4, "mike");  //  returns 2
    cout << x << " " << y << endl;
    
    cout << "All tests succeeded" << endl;
}

int appendToAll(string a[], int n, string value)
{
    // invalid argument
    if (n < 0)
        return -1;
    
    // append value onto each element of the array
    for (int i = 0; i < n; i++)
        a[i] += value;
    
    return n;
}

int lookup(const string a[], int n, string target)
{
    // invalid argument
    if (n < 0)
        return -1;
    
    // compare target with elements of the array
    for (int i = 0; i < n; i++)
        if (a[i] == target)
            return i;
    
    // if no string matches target string
    return -1;
}

int positionOfMax(const string a[], int n)
{
    // invalid arguement
    if (n <= 0)
        return -1;
    
    // current max position and value
    int maxPos = n-1;
    string max = a[n-1];
    
    // compare elements for maximum
    for (int i = n-1; i >= 0; i--)
        if (a[i] >= max)
        {
            maxPos = i;
            max = a[i];
        }

    return maxPos;
}

int rotateLeft(string a[], int n, int pos)
{
    // invalid argument
    if (n < 0)
        return -1;
    
    // temp string to kept elements that is eliminated
    string copy = a[pos];
    
    // shift elements to the left
    for (int i = pos; i < n - 1; i++)
        a[i] = a[i+1];
    
    // put eliminated string at the end of the array
    a[n-1] = copy;
    
    return pos;
}

int countRuns(const string a[], int n)
{
    // invalid argument
    if (n < 0)
        return -1;
    
    int runs = 0;
    
    // increment runs for each new element
    for (int i = 0; i < n; i++)
    {
        string item = a[i];
        runs++;
        for (int j = i + 1; a[i] == a[j]; j++)
            i++;
    }
    
    return runs;
}

int flip(string a[], int n)
{
    // invalid argument
    if (n < 0)
        return -1;
    
    // switch elements starting from outside to inside
    for (int i = 0, j = n - 1; i != n / 2; i++, j--)
    {
        string temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
    
    return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    // invalid argument
    if (n1 < 0 || n2 < 0)
        return -1;
    
    // get minimum n for limit of comparison
    int n;
    if (n1 <= n2)
        n = n1;
    else
        n = n2;
    
    // compare each element until elements differ
    for (int i = 0; i < n; i++)
        if (a1[i] != a2[i])
            return i;
    
    return n;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    // invalid argument
    if (n1 < 0 || n2 < 0)
        return -1;
    
    //
    for (int i = 0; i < n1; i++)
    {
        bool same = true;
        // check for contiguous sequence
        for (int j = 0, k = i; j < n2 && same == true; j++, k++)
            if (a1[k] != a2[j])
                same = false;
        
        // if a1 does contain  a2 as a contiguous  sequence
        if (same)
            return i;
    }
    
    // if a1 does not contain a2 as a contiguous subsequence
    return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    // invalid argument
    if (n1 < 0 || n2 < 0)
        return -1;
    
    // compare each element of first array with each element of second
    for (int  i = 0; i < n1; i++)
        for (int  j = 0; j < n2; j++)
            if (a1[i] == a2[j])
                return i;
    
    // if no elements are the same
    return -1;
}

int separate(string a[], int n, string separator)
{
    // invalid argument
    if (n < 0)
        return -1;
    
    // sorting the elements in ascending order
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                string temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            
            }
    
    // find where separator fits in the array and return position
    for (int i = 0; i < n; i++)
        if (a[i] >= separator)
            return i;
    
    // if the separator is greater than all elements
    return n;
}
