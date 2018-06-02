/*
Create an empty queue of stacks. 
    Create/add a stack containing {w1} to the queue. 
    While the queue is not empty: 
    Dequeue the partial-ladder stack from the front of the queue. 
    For each valid English word that is a "neighbor" (differs by 1 letter) 
of the word on top of the stack: 
        If that neighbor word has not already been used in a ladder before:
            If the neighbor word is w2: 
            Hooray! we have found a solution (and it is the stack you are 
            working on in the queue). 
            Else:
                Create a copy of the current partialladder stack. 
                Put the neighbor word on top of the copy stack. 
                Add the copy stack to the end of the queue.
*/
#include <iostream>
#include <map>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

void read(map<string, bool> &voc);
void getWords(string &, string &, const map<string, bool> &);
stack<string> wordLadder(string &, string &, map<string, bool> &);
int main()
{
    //Read file and build vocabulary
    map<string,bool> voc;
    read(voc);
    cout << endl;
    string word1, word2;
    while(true)
    {
        //User part
        getWords(word1, word2, voc);
        //find wordladder
        stack<string> rslt=wordLadder(word1, word2, voc);
        //print
        if(rslt.top()!="Bad")
        {
            cout << "A ladder from " << word2 << " to " << word1 << endl
                 << word2 << ' ';
            while(!rslt.empty())
            {
                cout<<rslt.top()<<' ';
                rslt.pop();
            }
            cout << endl;
        }
        else
            cout << "There is no word ladder for them.";
        cout << endl;
    }
    
    getchar();
    return 0;
}

void read(map<string,bool> &voc)
{
    //read the file
    string fileName;
    ifstream dic;
    cout << "Dictionary file name?  ";
	cin >> fileName;
    dic.open(fileName);
    while (!dic)
    {
        cout << "Unable to open that file. Try again,please." << endl
             << "Dictionary file name?  ";
        cin >> fileName;
        dic.open(fileName);
    }
    getchar();
    //store it in the map, voc
    string temp;
	while (getline(dic, temp))
		voc.insert(pair<string,bool>(temp,false));//false imply not been used yet
}

void getWords(string& word1, string& word2,const map<string,bool> &voc)
{
    bool isValid = false;
    while(!isValid)
    {
        isValid = true;
        cout << "Word #1 (or Enter to quit) ";
    	getline(cin,word1);
    	if (word1.empty()) exit (-1);
	    cout << "Word #2 (or Enter to quit) ";
	    getline(cin,word2);
	    if (word2.empty()) exit (-1);
        //to lower case
        transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
        transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
        //Invalide inputs
        if(word1.length()!=word2.length())
        {
            isValid = false;
            cout << "The two words msut have the same length." << endl
                 << endl;
            continue;
        }
        if(word1==word2)
        {
            isValid = false;
            cout << "The two words msut be different." << endl
                 << endl;
            continue;
        }
        if(voc.find(word1)==voc.end()||voc.find(word2)==voc.end())
        {
            isValid = false;
            cout << "The two words msut be found in the dictionary." << endl
                 << endl;
            continue;
        }
    }
}
vector<string> generateNext(const string word, map<string, bool> &voc)
{
    vector<string> nextWords;
    int len = word.size();
    string wordCopy;
    for (int i = 0; i < len; ++i )
    {
        for (char ch = 'a'; ch <= 'z'; ch ++)
        {
            wordCopy = word;
            wordCopy.replace(i, 1, string(1,ch));
            if(voc.find(wordCopy)!=voc.end() && voc[wordCopy]==false)
            {
                nextWords.push_back(wordCopy);
                voc[wordCopy] = true;
            }
        }
    }
    return nextWords;
}
stack<string> wordLadder(string &word1, string &word2, map<string, bool> &voc)
{
    //To initialize the queue
    stack<string> temp;
    temp.push(word1);
    voc[word1] = true;
    queue<stack<string>> q;
    q.push(temp);

    vector<string> nextWords;
    while(!q.empty())
    {
        temp = q.front();
        q.pop();
        //To find all neighbors
        nextWords = generateNext(temp.top(), voc);
        for (int i = 0; i < nextWords.size() ; ++i )
        {
            if(nextWords[i]==word2)
                return temp;
            else
            {
                temp.push(nextWords[i]);
                q.push(temp);
                temp.pop();
            }
        }
    }
    //if we can't find a ladder
    stack<string> Bad;
    Bad.push("Bad");
    return Bad;
}