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
            var trees = new Trees();
            trees.load();
            
            var aKeys = trees.Data.Keys;
            foreach (string a in aKeys) // each of the roots
            {
                var childrenOfA = trees[a];
                string[] childrenOfAKeys = childrenOfA.Keys.ToArray<string>();
                int childrenOfASize = childrenOfAKeys.Length;

                getSecond(trees, a, childrenOfA, childrenOfAKeys, childrenOfASize);
            }
        }

        private static void getSecond(Trees trees, string a, SortedDictionary<string, SortedSet<string>> childrenOfA, string[] childrenOfAKeys, int childrenOfASize)
        {
            for (int currentB = 0; currentB < childrenOfASize; currentB++) // for each child of a (b)
            {
                getThird(trees, a, childrenOfA, childrenOfAKeys, childrenOfASize, currentB);
            }
        }

        private static void getThird(Trees trees, string a, SortedDictionary<string, SortedSet<string>> childrenOfA, string[] childrenOfAKeys, int childrenOfASize, int currentB)
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

                getFourth(trees, a, childrenOfA, b, d, childrenOfB, childrenOfD, allEValues);
            }
        }

        private static void getFourth(Trees trees, string a, SortedDictionary<string, SortedSet<string>> childrenOfA, string b, string d, SortedDictionary<string, SortedSet<string>> childrenOfB, SortedDictionary<string, SortedSet<string>> childrenOfD, IEnumerable<string> allEValues)
        {
            foreach (string e in allEValues) // for each e value (child of b and d)
            {
                var allGValues = childrenOfA[d];

                getFifth(trees, a, childrenOfA, b, d, childrenOfB, childrenOfD, e, allGValues);
            }
        }

        private static void getFifth(Trees trees, string a, SortedDictionary<string, SortedSet<string>> childrenOfA, string b, string d, SortedDictionary<string, SortedSet<string>> childrenOfB, SortedDictionary<string, SortedSet<string>> childrenOfD, string e, SortedSet<string> allGValues)
        {
            foreach (string g in allGValues) // for each g value (child of ad)
            {
                var childrenOfBE = childrenOfB[e];
                var childrenOfG = trees[g];
                var childrenOfGKeys = childrenOfG.Keys;
                var allHValues = childrenOfGKeys.Intersect(childrenOfBE);
                getSixth(trees, a, childrenOfA, b, d, childrenOfD, e, g, childrenOfG, allHValues);
            }
        }

        private static void getSixth(Trees trees, string a, SortedDictionary<string, SortedSet<string>> childrenOfA, string b, string d, SortedDictionary<string, SortedSet<string>> childrenOfD, string e, string g, SortedDictionary<string, SortedSet<string>> childrenOfG, IEnumerable<string> allHValues)
        {
            foreach (string h in allHValues) //for each h value (child of be and g)
            {
                var allCValues = childrenOfA[b];

                getSeventh(trees, a, b, d, childrenOfD, e, g, childrenOfG, h, allCValues);
            }
        }

        private static void getSeventh(Trees trees, string a, string b, string d, SortedDictionary<string, SortedSet<string>> childrenOfD, string e, string g, SortedDictionary<string, SortedSet<string>> childrenOfG, string h, SortedSet<string> allCValues)
        {
            foreach (string c in allCValues) // for each c value (child of ab)
            {
                var childrenOfC = trees[c];
                var childrenOfCKeys = childrenOfC.Keys;
                var childrenOfDE = childrenOfD[e];
                var AllFValues = childrenOfCKeys.Intersect(childrenOfDE);

                System.Console.Out.Write( getEighth(a, b, d, e, g, childrenOfG, h, c, childrenOfC, AllFValues));
            }
        }

        private static string getEighth(string a, string b, string d, string e, string g, SortedDictionary<string, SortedSet<string>> childrenOfG, string h, string c, SortedDictionary<string, SortedSet<string>> childrenOfC, IEnumerable<string> AllFValues)
        {
            var allResults = new List<string>();
            foreach (string f in AllFValues) //for each f value (child of c and de)
            {
                var childrenOfCF = childrenOfC[f];
                var childrenOfGH = childrenOfG[h];
                var allIValues = childrenOfCF.Intersect(childrenOfGH);

                allResults.Add(getNinth(a, b, d, e, g, h, c, f, allIValues));
            }
            string allResultsString = string.Concat(allResults);
            return allResultsString;
        }

        private static string getNinth(string a, string b, string d, string e, string g, string h, string c, string f, IEnumerable<string> allIValues)
        {
            var allResults = new List<string>();
            foreach (string i in allIValues)
            {
                string[] results = new[] { a, " ", b, " ", c, "\n", d, " ", e, " ", f, "\n", g, " ", h, " ", i, "\n","\n" };
                string result = string.Concat(results);
                bool goodResult = (b != d && c != e && e != g && c != g && f != h);

                if (goodResult)
                {
                    allResults.Add(result);
                }
            }
            string allResultsString = string.Concat(allResults);
            return allResultsString;
        }
    }
}