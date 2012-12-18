#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#include <map>
#include <algorithm>
#include <set>

//  cout << (float) i /321135634 << '\r' << '\b'; // progress overwite
//  percentage thingy

using namespace std;

typedef vector <unsigned int> intVector;
typedef pair <unsigned int,intVector> intVectorPair;
typedef vector <intVectorPair> top;
typedef vector <top> tops;

// declare a new vector and pass it in for intersect. Pass in children vectors, and a new size int. Size will be the size of the intersect. Intersect will be the intersect vector. Don't trust the size of this vector. It has junk. Use size instead.
void getIntersection (vector <unsigned int> & x, vector <unsigned int> & y, unsigned int & size, vector <unsigned int> & intersect)
{
    
    sort (x.begin(),x.end());
    sort (y.begin(),y.end());
    
    if (x.size() > y.size())
    {
        intersect.resize (x.size());
    }
    else
    {
        intersect.resize (y.size());
    }
    
    vector <unsigned int>::iterator it;
    it = set_intersection (x.begin(),x.end(),y.begin(),y.end(),intersect.begin());
    size = (unsigned int) (it-intersect.begin()); // gives the size of intersection before you get junk
    
}

// function returns true if a branch object does not contain a given leaf
bool doesNotContainLeaf (intVector & container, unsigned int & x)
{
    
    if (container.empty())
    {
        return true;
    }
    unsigned int i=0;
    
    while (i<container.size()-1 && container[i] != x)
    {
        i++;
    }
    return (container[i] != x);
    
}

// procedure that places branch members from root into vector for easy retrieval. Only used for E so far.
void getChildren(top & root,vector <unsigned int> & children)
{
    for (unsigned int i = 0;i<root.size();i++)
    {
        children.push_back (root[i].first);
    }
}

// function returns the index of a branch for the root
unsigned int findBranch (top & root,unsigned int & x)
{
    unsigned int i = 0;
    
    while (root[i].first != x && i != root.size())
    {
        i++;
    }
    if (i == root.size())
    {
        cout << "over the limit in findBranch. Object not found/////////////////////////////////////////////////";
    }
    return i;
}

// procedure acts upon main and sweep coordinates directly, in order to find the next frame configuration
void iterateCoordinates (top & root,pair <unsigned int, unsigned int> & mainCoordinates,pair <unsigned int, unsigned int> & sweepCoordinates)
{
    if (sweepCoordinates.first == root.size() -1 && sweepCoordinates.second == root[root.size() -1].second.size() -1)
    {
        // then sweep is at end. Iterate main once and set sweep to main.
        
        // determine if main.second is at end
        if (mainCoordinates.second == root[mainCoordinates.first].second.size() -1)
        {
            mainCoordinates.second = 0;
            mainCoordinates.first++;
        }
        else //main.second is not at end
        {
            mainCoordinates.second++;
        }
        sweepCoordinates = mainCoordinates;
    }
    else
    {
        //determine if sweep.second is at end
        if (sweepCoordinates.second == root[sweepCoordinates.first].second.size()-1)
        {
            // iterate first and reset second
            sweepCoordinates.first++;
            sweepCoordinates.second = 0;
        }
        else // sweep.second is not at end
        {
            // iterate second, leave first alone
            sweepCoordinates.second++;
        }
    }
}

// procedure that creates a string to int dictionary entry for every unique word in the file
void loadDictionary(tops & trees,map <string,unsigned int> & dictionary)
{
    string word;
    typedef pair <string,unsigned int> stringInt; // this is a dictionary entry
    fstream vocab; vocab.open ("input.txt",fstream::in); // open the stream
    while (!vocab.eof())
    {
        vocab >> word;
        
        if (dictionary.count (word) == 0)
        {
            dictionary.insert (stringInt (word,dictionary.size()));
        }
    }
}

// procedure populates a reverse dictionary, that takes an int and gives back a string, for human readable output
void reverseEntries (map <string,unsigned int> & dictionary, map <unsigned int,string> & reverseDictionary)
{
    typedef pair <unsigned int,string> intString;
    //this creates the reverse dictionary
    map <string,unsigned int>::iterator i;
    
    for (i=dictionary.begin();i!=dictionary.end();i++)
    {
        unsigned int num = i-> second;
        string word = i->first;
        
        reverseDictionary.insert (intString (num,word));
    }
}

// function returns true if a root does not have a value x as a branch (child)
bool doesNotContain (top & root,unsigned int x)
{
    unsigned int i=0;
    if (root.size() ==0)
    {
        return true;
    }
    else
    {
        while (i < root.size()-1 && root[i].first != x)
        {
            i++;
        }
        
        return (root[i].first != x);
    }
}

// applies filtering and outputs to stdout sloppily formatted data
void outPutAll(vector<unsigned int> frame, map <unsigned int,string> & reverseDictionary)
{
    // check to see if it is junk first
    multiset <int> words;
    bool repeat = false;
    
    for (unsigned int i=0;i<frame.size();i++)
    {
        words.insert(frame[i]);
    }
    
    unsigned int iter =0;
    while (iter<frame.size() && (!repeat))
    {
        if (words.count (frame[iter]) > 1)
        {
            repeat = true;
        }
        iter++;
    }
    
    if (!repeat)
    {
        cout << "////////////////"<<endl;
        
        cout << reverseDictionary[frame[0]] << " " << reverseDictionary[frame[1]] << " " << reverseDictionary[frame[2]] << endl;
        cout << reverseDictionary[frame[3]] << " " << reverseDictionary[frame[4]] << " " << reverseDictionary[frame[5]] << endl;
        cout << reverseDictionary[frame[6]] << " " << reverseDictionary[frame[7]] << " " << reverseDictionary[frame[8]] << endl;
    }
}


// function returns the number of runs on a given root. Saves lots of computing with math!
unsigned int maximum (top root)
{
    unsigned int grandchildren = 0;
    unsigned int i = 0;
    
    while (i < root.size())
    {
        grandchildren += root[i].second.size();
        i++;
    }
    return ((grandchildren * (grandchildren+1))/2);
}

// procedure places into the frame new values, based upon main and sweep coordinates gotten from iterateCoordinates
void getNextFrame (unsigned int & current,top & root,vector <unsigned int> & frame,pair <unsigned int, unsigned int> & mainCoordinates,pair <unsigned int, unsigned int> & sweepCoordinates,bool & ranBefore)
{
    // This bool prevents missing the first one.
    if (ranBefore)
    {
        iterateCoordinates (root,mainCoordinates,sweepCoordinates);
    }
    
    frame [0] = current; // a
    frame [1] = root [mainCoordinates.first].first; // b
    frame [2] = root [mainCoordinates.first].second [mainCoordinates.second]; // c
    frame [3] = root [sweepCoordinates.first].first; // d
    frame [6] = root [sweepCoordinates.first].second [sweepCoordinates.second]; // g
    
    // for next time
    ranBefore = true;
    
}



// prepares for the search phase. Includes creation of necessary data structures from stream, and population of dictionaries.
void load (tops & trees,  map <unsigned int,string> &  reverseDictionary, map <string,unsigned int>  &dictionary)
{
    fstream in; in.open ("input.txt",fstream::in); // open the stream
    string word,oneBack,twoBack; // incoming strings
   
    
    loadDictionary (trees,dictionary);
    cout << "dictionary loaded"<<endl;
    
    trees.resize(dictionary.size());
    for (unsigned int temp = 0;temp<dictionary.size();temp++)
    {
        top dummy;
        trees[temp] = dummy;
    }
    
    in >> twoBack;
    in >> oneBack;
    
    // twoBack is the first word, and is therefore only a root.
    // oneBack is a branch on twoBack.
    intVectorPair temp;
    temp.first = dictionary[oneBack];
    temp.second.clear();
    trees[dictionary[twoBack]].push_back(temp);
    
    // now to construct the data structure
    while (!in.eof())
    {
        in >> word;
        // place the new word onto the correct root
        if (doesNotContain (trees[dictionary[oneBack]],dictionary[word]))
        {
            intVectorPair branch;
            branch.first = dictionary[word];
            branch.second.clear();
            trees[dictionary[oneBack]].push_back (branch);
        }
        
        // place the new word onto the correct branch
        
        unsigned int i = findBranch (trees[dictionary[twoBack]],dictionary[oneBack]);
        
        if (doesNotContainLeaf (trees[dictionary[twoBack]][i].second,dictionary[word]))
        {
            trees[dictionary[twoBack]][i].second.push_back(dictionary[word]);
        }
        
        // update the values (loop constant)
        twoBack = oneBack;
        oneBack = word;
    }
    
    // the final branch that was put on needs to be deleted.
    // this branch was put onto the final root that is talked about, and
    // it is the last entry.
    trees[dictionary[word]].pop_back(); // word is what used to be oneBack
    
    // now that trees is loaded, make the reverse dictionary
    reverseEntries (dictionary,reverseDictionary);
}

void getSquare (unsigned int current, vector <unsigned int> & frame, map <unsigned int, string> &reverseDictionary, tops & trees)
{
    //   cout << "current: "<< current<<endl; // rough idea of progress. Very jumpy.
    pair <unsigned int, unsigned int> mainCoordinates;
    pair <unsigned int,unsigned int> sweepCoordinates;
    
    mainCoordinates.first = 0;
    mainCoordinates.second = 0;
    sweepCoordinates = mainCoordinates;
    
    unsigned int max = maximum (trees[current]);
    bool ranBefore = false;
    
    for (unsigned int pos = 0;pos < max; pos++)
    {
        getNextFrame (current,trees[current],frame,mainCoordinates,sweepCoordinates,ranBefore);/////////////////////////////////////////////
        
        ///////////preparing to get
        ///////////E/////////////////////////////////////////////////////////////
        vector <unsigned int> childrenOfB;
        vector <unsigned int> childrenOfD;
        
        getChildren(trees[frame[1]],childrenOfB); // gets the children of root B.
        getChildren (trees[frame[3]],childrenOfD); // gets the children of root D.
        
        // variables to be filled by intersect:
        vector <unsigned int> intersectForE;
        unsigned int sizeOfIntersectForE;
        
        getIntersection (childrenOfB,childrenOfD,sizeOfIntersectForE,intersectForE);
        
        for (unsigned int iterE = 0; iterE < sizeOfIntersectForE; iterE++)
        {
            
            frame[4] = intersectForE[iterE];
            //we need the BE children
            unsigned int e = findBranch (trees[frame[1]],frame[4]);
            vector <unsigned int> childrenOfBE (trees[frame[1]][e].second);
            
            // we also need the children of G
            vector <unsigned int> childrenOfG;
            getChildren (trees[frame[6]],childrenOfG);
            
            // variables to be filled by intersect:
            vector <unsigned int> intersectForH;
            unsigned int sizeOfIntersectForH;
            
            getIntersection (childrenOfBE, childrenOfG,sizeOfIntersectForH,intersectForH);
            
            for (unsigned int iterH = 0; iterH < sizeOfIntersectForH;iterH++)
            {
                frame[7] = intersectForH[iterH]; // assign H, then go on to get possible F values
                
                ////////////preparing for
                ////////////F//////////////////////////////
                
                // we need the de children.
                unsigned int de = findBranch (trees[frame[3]],frame[4]);
                vector <unsigned int> childrenOfDE (trees[frame[3]][de].second);
                
                //now we need the children of C
                vector <unsigned int> childrenOfC;
                getChildren (trees[frame[2]],childrenOfC);
                
                //Now here are the target variables:
                vector <unsigned int> intersectForF;
                unsigned int sizeOfIntersectForF;
                
                getIntersection (childrenOfDE,childrenOfC,sizeOfIntersectForF,intersectForF);
                
                for (unsigned int iterF = 0; iterF<sizeOfIntersectForF; iterF++)
                {
                    frame[5] = intersectForF[iterF]; // assign F, then go on to get possible I values
                    
                    ////////////preparing for//////////
                    ////////////I  //////////////////////////////////////
                    
                    // we need the cf children
                    unsigned int cf = findBranch (trees[frame[2]],frame[5]);
                    vector <unsigned int> childrenOfCF (trees[frame[2]][cf].second); // fancy copy constructor. C++ is incredible.
                    
                    // we need the gh children
                    unsigned int gh = findBranch (trees[frame[6]],frame[7]);
                    vector <unsigned int> childrenOfGH (trees[frame[6]][gh].second);
                    
                    // Here are the data the we get from getIntersection
                    vector <unsigned int> intersectForI;
                    unsigned int sizeOfIntersectForI;
                    
                    getIntersection (childrenOfCF, childrenOfGH, sizeOfIntersectForI, intersectForI);
                    
                    for (unsigned int iteri = 0;iteri < sizeOfIntersectForI;iteri++)
                    {
                        frame[8] = intersectForI[iteri];
                        
                        outPutAll (frame,reverseDictionary);
                    }
                }
            }
        }
    }
    
}

int main()
{
    vector <unsigned int> frame (9,1234578); // here is an easy to recognize value, to indicate that a number was missed.
    map <unsigned int,string> reverseDictionary;
    map <string,unsigned int>  dictionary;
    tops trees;
    
    load (trees,reverseDictionary,dictionary);

    
    cout << "begin search phase"<<endl;
    //cout << "number of iterations: " << trees.size()<<endl;
   // for  (unsigned int current = 0;current < trees.size();current++)
    //{
    unsigned int current = dictionary["the"];
        getSquare (current,frame,reverseDictionary,trees);
    //}
    cout << "search complete\n";
    return 0;
}