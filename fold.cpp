#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#include <map>

//  cout << (float) i /321135634 << '\r' << '\b'; // progress overwite
//  percentage thingy

using namespace std;

typedef vector <unsigned int> intVector;
typedef pair <unsigned int,intVector> intVectorPair;
typedef vector <intVectorPair> top;
typedef vector <top> tops;

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

void populateChildren(top & root,unsigned int x,vector <unsigned int> children)
{
  children = root[x].second;
}

void populateChildrenForE(top & root,vector <unsigned int> & children)
{
	for(unsigned int i = 0;i<root.size();i++)
	{
		children.push_back (root[i].first);
	}
}

unsigned int findBranch (top & root,unsigned int & x)// findBranch (rootB,frame[4]);
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

void outPutAll(vector<unsigned int> frame, map <unsigned int,string> & reverseDictionary)
{
  // check to see if it is junk first
  //  if (!(frame[3] == frame[1] && frame[6] == frame[2] && frame[1] == frame[3] && frame[7] == frame[5] && frame[6] == frame[2]))
  //{
    for (unsigned int i=0;i<frame.size();i++)
      {
        cout << reverseDictionary[frame[i]] << endl;
      }
    //  }
}

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



unsigned int maximum (top root)
{
  unsigned int grandchildren = 0;
  unsigned int i = 0;
  
  while (i < root.size())
    {
      grandchildren += root[i].second.size();
      i++;
    }
  if (grandchildren == 1) // is this necessary? Test without this.
    {
      return 1;
    }
  else
    {
      return ((grandchildren * (grandchildren+1))/2);
    }
}

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
  for (unsigned int temp =0;temp<trees.size();temp++)
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
    }
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
      //      cout <<"current: " <<current <<endl;

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
                  //   outPutAll (frame,reverseDictionary);

          ///////////preparing to get
          ///////////E/////////////////////////////////////////////////////////////
          vector <unsigned int> children;
           unsigned int iterE = 0;
           bool gotNextE = true;
          top rootB = trees[frame[1]];
          top rootD = trees[frame[3]];
        
          populateChildrenForE(rootB,children); 
             
             while (gotNextE)
              {
        
                getNext (frame,children,rootD,gotNextE,iterE,4); 
                     
                   if (gotNextE)
                  {

                    unsigned int e = findBranch (rootB,frame[4]);
                    children.clear();
         
                    populateChildren (rootB,e,children);
                     
                  }
               }
        }
    }
  return 0;
}
