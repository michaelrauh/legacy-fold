using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace fold
{
    class Trees
    {
        private SortedDictionary<string, SortedDictionary<string, SortedSet<string>>> data = new SortedDictionary<string, SortedDictionary<string, SortedSet<string>>>();

        public SortedDictionary<string, SortedDictionary<string, SortedSet<string>>> Data
        {
            get { return data; }
        }
        public SortedDictionary<string, SortedSet<string>> this[string index]
        {
            get 
            {
               /* if (string.IsNullOrEmpty(index))
                {
                    throw new ArgumentNullException("index"); // need to develop way to get rid of empties.As it is, it manifests as a blank in output
                }*/
                if (data.ContainsKey(index))
                {
                return data[index]; 
                }
                else
                {
                    throw new ArgumentException("tree does not contain an entry for " + index, "index");
                }
            }
        }

        public SortedSet<string> this[string index,string index2]
        {
            get 
            {
                if (data.ContainsKey(index))
                {
                    var x = data[index];
                    if (x.ContainsKey(index2))
                    {
                        return x[index2];
                    }
                    else
                    {
                        throw new ArgumentException("tree does not contain an entry for " + index2, "index2");
                    }
                }
                else
                {
                    throw new ArgumentException("tree does not contain an entry for " + index, "index");
                }
            }
        }
        public void load()
        {
            string[] words = System.IO.File.ReadAllText(@"C:\Users\Owner\Desktop\mars.txt").Split(' ');
            for (int i = 0; i < words.Length; i++)
            {
                if (!data.ContainsKey(words[i])) // if the top is missing
                {
                    SortedSet<string> bottom = new SortedSet<string>();
                    if (i + 2 < words.Length) // if boundary conditions fail a dummy is added to preserve structure
                        bottom.Add(words[i + 2]);

                    SortedDictionary<string, SortedSet<string>> middle = new SortedDictionary<string, SortedSet<string>>();
                    if (i + 1 < words.Length)
                        middle.Add(words[i + 1], bottom);

                    data.Add(words[i], middle);
                }
                else
                {
                    if (!data[words[i]].ContainsKey(words[i + 1])) //if the branch is missing
                    {
                        SortedSet<string> bottom = new SortedSet<string>();
                        if (i + 2 < words.Length)
                            bottom.Add(words[i + 2]);

                        data[words[i]].Add(words[i + 1], bottom);
                    }
                    else
                    {
                        if (!data[words[i]][words[i + 1]].Contains(words[i + 2])) //if the leaf is missing
                        {
                            if (i+2 < words.Length)
                            data[words[i]][words[i + 1]].Add(words[i + 2]);
                        }
                    }
                }
            }
        }
    }
}