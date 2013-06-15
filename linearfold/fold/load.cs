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
                if (string.IsNullOrEmpty(index))
                {
                  // throw new ArgumentNullException("index");
                }
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
            List<string> words = System.IO.File.ReadAllText(@"C:\Users\Owner\Desktop\mars2.txt").Replace('\n', ' ').Replace('\t', ' ').Split(' ').ToList();
            for (int index = 0; index < words.Count;index++ )
            {
                if (string.IsNullOrEmpty(words[index]) || string.IsNullOrWhiteSpace(words[index]))
                {
                    words.RemoveAt(index);
                }
            }
            words.Add("");
            for (int i = 0; i < words.Count; i++)
            {
                string firstWord = words[i];
                string secondWord;
                string thirdWord;

                if (i + 2 < words.Count)
                {
                    secondWord = words[i + 1];
                    thirdWord = words[i + 2];
                }
                else
                {
                    secondWord = "";
                    thirdWord = "";
                }
                if (i + 1 < words.Count)
                {
                    secondWord = words[i + 1];
                }
                else
                {
                    secondWord = "";
                }

                if (!data.ContainsKey(firstWord)) // if the root is missing
                {
                    var bottom = new SortedSet<string>();
                    var middle = new SortedDictionary<string, SortedSet<string>>();
                    bottom.Add(thirdWord);
                    middle.Add(secondWord, bottom);

                    data.Add(firstWord, middle);
                }
                else
                {
                    if (!data[firstWord].ContainsKey(secondWord)) //if the branch is missing
                    {
                        var bottom = new SortedSet<string>();
                        bottom.Add(thirdWord);

                        data[firstWord].Add(secondWord, bottom);
                    }
                    else
                    {
                        if (!data[firstWord][secondWord].Contains(thirdWord)) //if the leaf is missing
                        {
                            data[firstWord][secondWord].Add(thirdWord);
                        }
                    }
                }
            }
        }
    }
}