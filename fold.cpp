#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#include <map>


//pass root to end

// does load actually work?
// it seems that end is returning false when it needs to return true
// make sure getNextFrame works by monitoring the coordinates

// get rid of the push backs in load. Complicated but make a complete
// dictionary, then make the vector start that size, filled in with roots.

using namespace std;

typedef vector <unsigned int> intVector;
typedef pair <unsigned int,intVector> intVectorPair;
typedef vector <intVectorPair> top;
typedef vector <top> tops;

bool doesNotContain (top root,unsigned int x)
{
  unsigned int i=0;

  while (i<root.size())
    {
      if (root[i].first == x)
        {
          return false;
        }
      i++;
    }
  return true;
}

void outPutAll(vector<unsigned int> frame)
{
  // check to see if it is junk first
  //  if (!(frame[3] == frame[1] && frame[6] == frame[2] && frame[1] == frame[3] && frame[7] == frame[5] && frame[6] == frame[2]))
   {
      for (unsigned int i=0;i<frame.size();i++)
        {
           cout << frame[i] <<endl; // this is not a "pretty print" it
          // outputs it for processing later
        }
       }
}

void getNext(vector <unsigned int> & frame, vector<unsigned int> children, top root,bool & gotNext,unsigned int & i, unsigned int framePos)
{
  // Now that we have the frame, we need to get the next valid e value
  // for that frame. If there is no valid e value, then gotNextE is
  // false

  if (i >= children.size())
    {
      gotNext = false;
    }
  else
    {
      while  (i < root.size() && doesNotContain (root,children[i]))
        {
          i++;
        }
      
      if (doesNotContain (root,children[i]))
        {
          gotNext = false;
        }
      else 
        {
          gotNext = true;
          frame [framePos] = root[i].first;
        }
    }
}


bool end (unsigned int current,tops & trees,pair <unsigned int,unsigned int> mainCoordinates,pair <unsigned int,unsigned int> sweepCoordinates)
{

  // if there are more problems it is the result of the -1 in the
  // code. 0-1 can have bad consequences for this. I think it is
  // prevented by the # of granchildren work though.

	//return true; //Just checked: It reaches the end of the program in 19 iterations when this is always true.
	//return false; //this does not seem to effect the behavoir of the program.

  // end indicates that trees[current] is used up. That is, the sweep
  // is at the max values, and main is one behind

  // first, create an object that is trees[current]
  top root = trees[current];


  // first check to see if there is "room" for main and sweep. We need
  // 2 grandchildren.

  // this is if the root has no branches
  if (root.size() == 0)
    {
      return true;
    }
  else if (root.size() == 1) 
    {
      // all of the grandchildren must be on this branch, so the
      // branch needs at least 2 children.
      if (!(root[0].second.size() < 1)) // if this is false if moves
                                        // on to the other considerations
        {
          return true;
        }
    }
  else // at least two 
    {
      unsigned int number = 0;
      unsigned int i = 0;

      while (number < 2 && i < root.size())
        {
          number += root[i].second.size();
          i++;
        }

        if (number < 2) 
        {
          return true;
        }
    }
  // now check to see if sweep is at end. 
  if (!(sweepCoordinates.first == root.size() - 1 && sweepCoordinates.second == root[sweepCoordinates.first].second.size() - 1) )
    {
      // this means that it is not at the end, so we can return false
      // immediately
      return false;
    }
  else // sweep is at end. check main 
    {
      // if not on the same root
      if (mainCoordinates.first != sweepCoordinates.first)
                                                          
        {
          return (
                  // sweep is on next root, main is on last leaf, and sweep's branch
                  // has one child
                  (mainCoordinates.first == sweepCoordinates.first - 1)
                  && (mainCoordinates.second == root[mainCoordinates.first].second.size() - 1)
                  && (root[sweepCoordinates.first].second.size() == 1)
                  );
        }
      else // on the same branch
        {
          // check one grandchild back
          return (mainCoordinates.second == sweepCoordinates.second - 1);
        }
    }
}

void getNextFrame (unsigned int current,vector <unsigned int> & frame,tops & trees,pair <unsigned int, unsigned int> & mainCoordinates,pair <unsigned int, unsigned int> & sweepCoordinates)
{
  //  cout <<"here"<<endl;
  // Most important thing is that we know that it is not at the end at
  // this point. 
  // Determine if main needs to advance (sweep at end)
  top root = trees[current];
  
  if (sweepCoordinates.first == root.size() -1 && sweepCoordinates.second == root[root.size() -1].second.size() -1)
    {
      // then sweep is at end. Iterate main once and set sweep to the
      //   main value
      //   sweepCoordinates.first = 0;
      // sweepCoordinates.second = 0;
      
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

  // now to load the frame in based upon the new positions
  
  // abc
  //def
  //ghi
  // frame = a,b,c,d,e,f,g,h,i
  
  // main goes across top (abc)
  // sweep goes down side (adg)

  // If these end up being the correct type, I am assuming they are
  // the correct value

  frame [0] = current; // a
  frame [1] = root [mainCoordinates.first].first; // b
  frame [2] = root [mainCoordinates.first].second [mainCoordinates.second]; // c
  frame [3] = root [sweepCoordinates.first].first; // d
  frame [6] = root [sweepCoordinates.first].second [sweepCoordinates.second]; // g

  //  cout << "there"<<endl;
}
 


void load (tops & trees)
{
  typedef pair <string,unsigned int> stringInt; // this is a dictionary entry
  fstream in; in.open ("test2.txt",fstream::in); // open the stream
  string one,two,three,oneNew,twoNew,threeNew; // incoming strings
  unsigned int oneInt,twoInt,threeInt; // ints for the strings
  map <string,unsigned int>  dictionary; // keeps the numbers of the strings

  while (!in.eof())
    {
    
      in >> one;
      in >> two;
      in >> three;
    
      
      //check to see if the inputs are defined. If they are not, give
      //them a dictionary entry, and a place in the trees vector

      if (dictionary.count (one) == 0)
        {
          top root;
          dictionary.insert (stringInt (one,dictionary.size()));
          // this is putting an empty root as a placeholder.
          trees.push_back (root);                     
        }
      if (dictionary.count (two) == 0)
        {
          top root;
          dictionary.insert (stringInt (two,dictionary.size()));
          // this is putting an empty root as a placeholder.
          trees.push_back (root);                     
        }
      if (dictionary.count (three) == 0)
        {
          top root;
          dictionary.insert (stringInt (three,dictionary.size()));
          // this is putting an empty root as a placeholder.
          trees.push_back (root);                     
        }
     

      // Now that everything is definitely defined, and has a place,
      // put each thing into its place. The root (one) must be in
      // place as it is defined. Now put two below it.
     
     
      // if two is not already defined below one, define it
      // this requires searching the vector of pairs by int value
      if (doesNotContain(trees[dictionary[one]], (dictionary[two])))
        {
       
          intVectorPair branch;
          branch.first = dictionary[two];
          trees[dictionary[one]].push_back (branch);
        }
    
      // Here is a loop that'll run only a couple of times to find
      // the right branch, which must exist
      unsigned int guess;
      threeInt = dictionary[three];
      twoInt = dictionary[two];
      oneInt = dictionary[one];
      unsigned int i = 0;
      
      guess = trees[dictionary[one]][i].first;


      while (twoInt != guess)
        {
          i++;
          guess = trees[dictionary[one]][i].first;
        }
      // cout << "here" << endl;
      //This line is causing a bad_alloc
      //   cout<<"dictionary [one] " << dictionary [one]<<endl;
      // cout << "i " << i<<endl;
      // cout<< " trees[dictionary[one]].size() " << trees[dictionary[one]].size()<<endl;
      //  cout<<"trees.size " << trees.size()<<endl<<endl;

      trees[dictionary[one]][i].second.push_back(threeInt);
      //  cout << "there" <<endl;
    }

  // now output the dictionary for later use
  map <string,unsigned int>::iterator it;
  it = dictionary.begin();
  while (it != dictionary.end())
    {
      //     cout <<it->first<<endl;
      // cout << it->second<<endl;
      it++;
    }
}

int main()
{
  vector <unsigned int> frame (9,0);
  tops trees;
  // cout << "before load"<<endl;
  load (trees);
  // cout << "made it out of load"<<endl;

  // for each of the roots
  for  (unsigned int current = 0;current < trees.size();current++)
  {
    // cout << "trees.size: " << trees.size() << "current : " << current << endl;
    // cout << endl<< "current:  " << current<<endl;
    // These will keep track of how far along main and sweep are.
    // Start at (0,0)
    pair <unsigned int, unsigned int> mainCoordinates;
    pair <unsigned int,unsigned int> sweepCoordinates;
    mainCoordinates.first = 0;
    mainCoordinates.second = 0;
    sweepCoordinates = mainCoordinates;

    //this will determine if the root is used up
    //    cout << "before getNextFrame" <<endl;
    while (!end(current,trees,mainCoordinates,sweepCoordinates))
      {
        //   cout << "Main : (" <<  mainCoordinates.first << "," <<
        //   mainCoordinates.second << ") " << "sweep: (" <<
        //   sweepCoordinates.first << "," << sweepCoordinates.second <<
        //   ")" << endl;

         //  cout << trees[current][sweepCoordinates.first].second[sweepCoordinates.second]<<endl;
        
        top root = trees[current];
        //   cout << root[root.size()-1].second[root[root.size()-1].second.size()-1]<<endl;
        //    cout << "# of branches: " << root.size()<<endl;
        //    cout << "# of leaves: " << root[root.size()-1].second.size()<<endl;

        // This will place the correct ints into frame
        getNextFrame (current,frame,trees,mainCoordinates,sweepCoordinates);

        
        //    cout << "after getNextFrame" <<endl;
        // now that there is a frame to work with, the last 4 need to
        // be filled in
       
       unsigned int iterE=0; // keeps track of past e values tried
        unsigned int i=0; // just used to populate children
        
        // populate vector of children of b
        top rootB = trees[frame[1]];
        top rootD = trees[frame[3]];
        
        vector <unsigned int> children;

        // cout << rootB.size() <<endl;
        //  cout <<"before" << endl;
        while (i<rootB.size())
          {
            // cout <<" i : " << i << "size: " << rootB.size()<<endl;
            children.push_back(rootB[i].first);
            i++;
          }
        //     cout <<"after" <<endl;
        bool gotNextE = true; // just to enter the loop the first time
        while (gotNextE)
          {
            //    cout << "next" <<endl;
            // this will get the next e value (pos == 4)
            getNext(frame,children,rootD,gotNextE,iterE,4); 
            //  cout << "here?" << endl;
            //  cout << gotNextE <<endl;
            if (gotNextE) // there is a good e value
              {
                      cout << "one" <<endl;
                // Here is the stuff getting ready to call getNextF.
                // first, the two important roots must be found
                top rootD = trees[frame[3]];
                top rootC = trees[frame[2]];
                cout << "two"<<endl;
                // Now all of the children of de need to be put into
                // the children vector.
                children.clear();
                i=0;
                unsigned int iterF=0;
                cout << "three" <<endl;
                // first it is necessary to find the correct child of
                // rootD (the e value)
                unsigned int k = 0;
                //    cout << "before" <<endl;
                while (rootD[k].first != frame[4])
                  {
                    k++;
                  }
                cout <<"four" <<endl;
                while (i<rootD[k].second.size())
                  {
                    children.push_back(rootD[k].second[i]);
                    i++;
                  }
                cout << "five" <<endl;
                //    cout << "after" <<endl;
                bool gotNextF = true;
                while (gotNextF)
                  {
                    cout <<"six" <<endl;
                    getNext(frame,children,rootC,gotNextF,iterF,5);
                    cout << "seven" <<endl;
                    
                    if (gotNextF) // there is a good f value
                      {
                        // now we have to find h, the child of be and
                        // g
                        cout <<"eight" <<endl;
                        top rootB = trees[frame[1]];
                        top rootG = trees[frame[6]];
                        cout << "9" <<endl;
                        children.clear();
                        i=0;
                        unsigned int iterH=0;
                        cout << "10" <<endl;
                        // first it is necessary to find the correct child of
                        // rootB (the e value)
                        unsigned int k = 0;
                        while (rootB[k].first != frame[4])
                          {
                            k++;
                          }
                        cout << "11" <<endl;
                        // load the vector of children of be
                        while (i<rootD[k].second.size())
                          {
                            children.push_back(rootD[k].second[i]);
                            i++;
                          }
                        cout << "12" <<endl;
                        bool gotNextH = true;
                        while (gotNextH)
                          {
                            getNext (frame,children,rootG,gotNextH,iterH,7);
                            cout << "13" <<endl;
                            if (gotNextH) // there is a good H value
                              {
                                // we can just reuse rootG and rootC
                                // here
								  top rootG = trees[frame[6]];
								  top rootC = trees[frame[2]];
                                children.clear();
                                i=0;
                               unsigned int iterI=0;
                               cout << "14" <<endl;
                                // first it is necessary to find the correct child of
                                // rootC (the f value)
                               unsigned int k = 0;
                               //  cout <<k<<endl;
                               cout <<"f " << frame[5] <<endl;
                                while (rootC[k].first != frame[5])
                                  {
                                    cout <<"f " << frame[5] <<endl;
                                    cout << "guess " << rootC[k].first <<endl;
                                    // cout <<rootC.size()<<endl;
                                    //  cout << rootC.size()-1<<endl << k;
                                    k++;
                                  }
                                cout << "15" <<endl;
                                
                                // load the vector of children of cf
                                while (i<rootC[k].second.size())
                                  {
                                    children.push_back(rootC[k].second[i]);
                                    i++;
                                  }

                                cout << "16" <<endl;
                                bool gotNextI = true;
                                while (gotNextI)
                                  {
                                    getNext (frame,children,rootG,gotNextI,iterI,8);

                                    if (gotNextI) // there is a good
                                                  // value for i
                                      {
                                        cout << "center" <<endl;
                                        outPutAll(frame);
                                        
                                      }
                                  }
                              }
                          }
                      }
                  }
              }
          }
      }
  }
  // notice no iteration on this end. All iteration is internal.
  // (there is also a for loop) (by internal I mean the getnext
  // functions iterate)
  return 0;

}
