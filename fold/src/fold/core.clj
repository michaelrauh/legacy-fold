(ns fold.core
  (:gen-class))
(require '[clojure.string :as str])

(defn indexed
  "Returns a lazy sequence of [index, item] pairs, where items come
  from 's' and indexes count up from zero.

  (indexed '(a b c d))  =>  ([0 a] [1 b] [2 c] [3 d])"
  [s]
  (map vector (range) s))

(defn positions
  "Returns a lazy sequence containing the positions at which pred
   is true for items in coll."
  [pred coll]
  (for [[idx elt] (indexed coll) :when (pred elt)] idx))


(defn read-to-list
  "Take in filename and return list of words"
  [filename]
  (str/split (slurp filename) #" "))

(defn parse
  "Take a list of words and return a foldable object"
  [words]
  (pr words))

(def x (read-to-list "resources/text.txt"))

(positions (hash-set "is") x)

(defn x-positions
  "currying the collection name out of the positions function"
  [word]
  (positions (hash-set word) x))

(x-positions "is")

(map x-positions (distinct x))

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (parse (read-to-list "resources/text.txt")))
