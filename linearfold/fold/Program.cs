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
            
            var aKeys = trees.Data.Keys;

            foreach (string a in aKeys) // each of the roots
            {
                var childrenOfA = trees[a];
                string[] childrenOfAKeys = childrenOfA.Keys.ToArray<string>();
                int childrenOfASize = childrenOfAKeys.Length;

                for (int currentB = 0; currentB < childrenOfASize; currentB++) // for each child of a (b)
                {
                    for (int currentD = currentB; currentD < childrenOfASize; currentD++) //for each unused child of a (d)
                    {
                        string b = childrenOfAKeys[currentB];
                        string d = childrenOfAKeys[currentD];
                        var childrenOfB = trees[b];
                        var childrenOfD = trees[d];
                        var childrenOfBKeys = childrenOfB.Keys;
                        var childrenOfDKeys = childrenOfD.Keys;
                        var allEValues = childrenOfBKeys.Intersect(childrenOfDKeys);

                        foreach (string e in allEValues) // for each e value (child of b and d)
                        {
                            var allGValues = childrenOfA[d];

                            foreach (string g in allGValues) // for each g value (child of ad)
                            {
                                var childrenOfBE = childrenOfB[e];
                                var childrenOfG = trees[g];
                                var childrenOfGKeys = childrenOfG.Keys;
                                var allHValues = childrenOfGKeys.Intersect(childrenOfBE);

                                foreach (string h in allHValues) //for each h value (child of be and g)
                                {
                                    var allCValues = childrenOfA[b];

                                    foreach (string c in allCValues) // for each c value (child of ab)
                                    {
                                        var childrenOfC = trees[c];
                                        var childrenOfCKeys = childrenOfC.Keys;
                                        var childrenOfDE = childrenOfD[e];
                                        var AllFValues = childrenOfCKeys.Intersect(childrenOfDE);

                                        foreach (string f in AllFValues) //for each f value (child of c and de)
                                        {
                                            var childrenOfCF = childrenOfC[f];
                                            var childrenOfGH = childrenOfG[h];
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