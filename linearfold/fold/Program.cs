using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace fold
{
    class Program
    {
        static void Main(string[] args)
        {
            var timer = new System.Diagnostics.Stopwatch();
            timer.Start();
            var trees = new Trees();
            trees.load();

            SortedDictionary<string, SortedDictionary<string, SortedSet<string>>>.KeyCollection aKeys = trees.Data.Keys;

            foreach (string a in aKeys) // each of the roots
            {
                SortedDictionary<string, SortedSet<string>>.KeyCollection bdKeys = trees.Data[a].Keys;
                string[] childrenOfA = bdKeys.ToArray<string>();

                for (int currentB = 0; currentB < bdKeys.Count; currentB++) // for each child of a (b)
                {
                    for (int currentD = currentB; currentD < bdKeys.Count; currentD++) //for each unused child of a (d)
                    {
                        string b = childrenOfA[currentB];
                        string d = childrenOfA[currentD];
                        SortedDictionary<string, SortedSet<string>> childrenOfB = trees.Data[b];
                        SortedDictionary<string, SortedSet<string>> childrenOfD = trees.Data[d];

                        SortedDictionary<string, SortedSet<string>>.KeyCollection childrenOfBKeys = childrenOfB.Keys;
                        SortedDictionary<string, SortedSet<string>>.KeyCollection childrenOfDKeys = childrenOfD.Keys;

                        var allEValues = childrenOfBKeys.Intersect(childrenOfDKeys);

                        foreach (string e in allEValues) // for each e value (child of b and d)
                        {
                            SortedSet<string> allGValues = trees.Data[a][d];

                            foreach (string g in allGValues) // for each g value (child of ad)
                            {
                                SortedSet <string> childrenOfBE = trees.Data[b][e];

                                SortedDictionary<string, SortedSet<string>> childrenOfG = trees.Data[g];
                                SortedDictionary<string, SortedSet<string>>.KeyCollection childrenOfGKeys = childrenOfG.Keys;

                                var allHValues = childrenOfGKeys.Intersect(childrenOfBE);

                                foreach (string h in allHValues) //for each h value (child of be and g)
                                {
                                    SortedSet<string> allCValues = trees.Data[a][b];

                                    foreach (string c in allCValues) // for each c value (child of ab)
                                    {
                                        SortedDictionary<string, SortedSet<string>> childrenOfC = trees.Data[c];
                                        SortedDictionary<string, SortedSet<string>>.KeyCollection childrenOfCKeys = childrenOfC.Keys;

                                        SortedSet<string> childrenOfDE = trees.Data[d][e];

                                        var AllFValues = childrenOfCKeys.Intersect(childrenOfDE);

                                        foreach (string f in AllFValues) //for each f value (child of c and de)
                                        {
                                            SortedSet<string> childrenOfCF = trees.Data[c][f];
                                            SortedSet<string> childrenOfGH = trees.Data[g][h];

                                            var allIValues = childrenOfCF.Intersect(childrenOfGH);

                                            foreach (string i in allIValues)
                                            {
                                                string[] results = new[] {a,b,c,d,e,f,g,h,i};

                                                var duplicates = results.GroupBy(s => s).Where(x => x.Count() > 1).Select(x => x.Key);
                                                if (duplicates.Count()==0)
                                                {
                                                    foreach (string str in results)
                                                    {
                                                        System.Console.WriteLine(str);
                                                    }
                                                    System.Console.WriteLine("////////////");
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
            timer.Stop();
            System.Diagnostics.Debug.WriteLine("Elapsed Time {0}", timer.Elapsed);
            Console.Read();
        }
    }
}