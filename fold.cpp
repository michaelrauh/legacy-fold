#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#include <map>
#include <algorithm>
#include <set>


//populatechildrenforE is about to expand to just get children. Get grandchildren may be useful. It would have a call to find branch

//  cout << (float) i /321135634 << '\r' << '\b'; // progress overwite
//  percentage thingy

using namespace std;

typedef vector <unsigned int> intVector;
typedef pair <unsigned int,intVector> intVectorPair;
typedef vector <intVectorPair> top;
typedef vector <top> tops;

// declare a new vector and pass it in for intersect. Pass in children vectors, and a new size int. Size will will be the size of the intersect. Intersect will be the intersect vector. Don't trust the size of this vector. It has junk. Use size instead.
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
	for(unsigned int i = 0;i<root.size();i++)
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

// procedure that gets the next value that passes a check against the perpindicular. framepos is the position in the frame of the data point being gotten. gotNext is a status that will be false if the root is exhausted. If it is successful, it will insert into the frame a word that "works" It takes a vector of children of one root, and a plain root from the other side to check against. This seems a bit lopsided though. Why not just take all of the children, run a set intersection (like for I) and do a for each over the set intersection? Seems simpler to me.
void getNext(vector <unsigned int> & frame, vector<unsigned int> children, top & root,bool & gotNext,unsigned int & i, unsigned int framePos)
{
  while  (i < children.size() && doesNotContain (root,children[i]))
    {
      i++;
    }
  if (!(i < children.size()))
    {
      gotNext = false;
    }
  else if (i < children.size() && doesNotContain (root,children[i]))
    {
      gotNext = false;
    }
  else 
    {
      gotNext = true;
      frame [framePos] = children[i];
    }
  i++;
}


// function returns int - the number of runs on a given root. Saves lots of computing with math!
unsigned int maximum (top root)
{
  unsigned int grandchildren = 0;
  unsigned int i = 0;
  
  while (i < root.size())
    {
      grandchildren += root[i].second.size();
      i++;
    }
 // if (grandchildren == 1) // is this necessary? Test without this.
   // {
     // return 1;
    //}
  //else
    //{
      return ((grandchildren * (grandchildren+1))/2);
    //}
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
 void load (tops & trees,  map <unsigned int,string> &  reverseDictionary)
{
  fstream in; in.open ("input.txt",fstream::in); // open the stream
  string word,oneBack,twoBack; // incoming strings
  map <string,unsigned int>  dictionary; // keeps the numbers of the strings
  
  loadDictionary (trees,dictionary);
  cout << "dictionary loaded"<<endl;
  // can use fill constructor?
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

int main()
{
  vector <unsigned int> frame (9,1234578);
  map <unsigned int,string> reverseDictionary;
  tops trees;
  
  load (trees,reverseDictionary);
  ////////////////////////////////////////////////////////////////
    
    //This outputs a representation of the main data structure.
  /* for (unsigned int temp =0;temp<trees.size();temp++)
    {
      top tempRoot = trees[temp];
      
      unsigned int iter1,iter2;
      cout << "root: " <<reverseDictionary[temp]<<endl;
      for (iter1=0;iter1<tempRoot.size();iter1++)
        {
          cout <<"\t" << "branch: " <<reverseDictionary[tempRoot[iter1].first] <<endl;
          for (iter2=0;iter2<tempRoot[iter1].second.size();iter2++)
            {
              cout<<"\t\t" <<"leaf: "<<reverseDictionary[tempRoot[iter1].second[iter2]]<<endl;
            }
        }
    }*/
    
  ////////////////////////////////////////////////////////////////////
    
   cout << "begin search phase"<<endl;
   cout << "number of iterations: " << trees.size()<<endl;
  for  (unsigned int current = 0;current < trees.size();current++)
    { 
      //   cout << "current: "<< current<<endl;
      pair <unsigned int, unsigned int> mainCoordinates;
      pair <unsigned int,unsigned int> sweepCoordinates;
      
      mainCoordinates.first = 0;
      mainCoordinates.second = 0;
      sweepCoordinates = mainCoordinates;
      
      // cout<< "////////////////////////////"<<endl;
      //  cout <<"current: " <<current <<endl;
      
      top root = trees[current];
      unsigned int max = maximum (root);
      // cout <<" max " <<max<<endl;
      bool ranBefore = false;
      // cout << "max: " <<max<<endl;
      //  cout << "root: "<<reverseDictionary [current]<<endl;
      for (unsigned int pos = 0;pos < max; pos++)
        { 
          getNextFrame (current,root,frame,mainCoordinates,sweepCoordinates,ranBefore);/////////////////////////////////////////////
          
          
          //  cout << "main: (" << mainCoordinates.first <<"," <<mainCoordinates.second <<")" << "  ";
          //cout << "sweep: (" <<sweepCoordinates.first << "," <<sweepCoordinates.second <<")" << endl;
                   
          ///////////preparing to get
          ///////////E/////////////////////////////////////////////////////////////
          vector <unsigned int> children;
          unsigned int iterE = 0;
          bool gotNextE = true;
          top rootB = trees[frame[1]];
          top rootD = trees[frame[3]];
          
          getChildren(rootB,children); // gets the children of root B. That is, possible values for e from the perspective of B.
          
          while (gotNextE)
            {
              
              getNext (frame,children,rootD,gotNextE,iterE,4); // edits frame at position 4, by iterating iterE over children, and checking to see if they are children of rootD. If they are, gotNextE is true. It will continue to search until children is exhausted or until gotNextE is true though. - design note : What is to prevent just finding the intersection of these sets of children, and then iterating over that intersection? All that really matters is what is in the frame. Once the frame is gotten, you go to the next round. You just have to make sure you don't go past where you are trying to go. This can be avoided by essentially doing a for each. 
              
              if (gotNextE)
                {
                  
                  unsigned int e = findBranch (rootB,frame[4]);
                  vector <unsigned int> childrenOfBE (rootB[e].second);
                  
                  top rootG = trees[frame[6]];
                  bool gotNextH=true;
                  unsigned int iterH =0;
                  
                  while (gotNextH)
                    {
                      getNext (frame,childrenOfBE,rootG,gotNextH,iterH,7);
                      if (gotNextH)
                        {
                          ////////////preparing for
                          ////////////F//////////////////////////////

                          // we need the de children.
                          unsigned int de = findBranch (rootD,frame[4]);
                          vector <unsigned int> childrenOfDE (rootD[de].second);
                            
                          //now we need the children of C
                          top rootC = trees[frame[2]];
                            vector <unsigned int> childrenOfC;
                            getChildren (rootC,childrenOfC);
                            
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
                                  unsigned int cf = findBranch (rootC,frame[5]);
                                  vector <unsigned int> childrenOfCF (rootC[cf].second); // fancy copy constructor. C++ is incredible.

                                  // we need the gh children
                                  unsigned int gh = findBranch (rootG,frame[7]);
                                  vector <unsigned int> childrenOfGH (rootG[gh].second);
                                    
                                    // Here are the data the we get from getIntersection
                                    vector <unsigned int> intersectForI;
                                    unsigned int sizeOfIntersectForI;
                                    
                                    getIntersection (childrenOfCF, childrenOfGH, sizeOfIntersectForI, intersectForI);
                                  
                                    for (unsigned int iteri = 0;iteri < sizeOfIntersectForI;iteri++) // for each item in the intersection
                                    {
                                      frame[8] = intersectForI[iteri]; // assign item to frame space 8. This is a bit of hard coding that is necessary
                                     
                                      outPutAll (frame,reverseDictionary);
                                    } 
                            }
                        }
                    }
                }
            }
        }
    }
  cout << "search complete\n";
  return 0;
}
