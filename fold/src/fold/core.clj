(ns fold.core
  (:gen-class))
(require '[clojure.string :as str])

(defn indexed
  "Returns a lazy sequence of [index, item] pairs, where items come
  from 's' and indexes count up from zero. Taken from library

  (indexed '(a b c d))  =>  ([0 a] [1 b] [2 c] [3 d])"
  [s]
  (map vector (range) s))

(defn positions
  "Returns a lazy sequence containing the positions at which pred
   is true for items in coll. Taken from library"
  [pred coll]
  (for [[idx elt] (indexed coll) :when (pred elt)] idx))


(defn read-to-list
  "Take in filename and return list of words"
  [filename]
  (str/split (slurp filename) #" "))

(defn parse
  "Take a list of words and return a foldable object. For now, print the words"
  [words]
  (pr words))

(def x (read-to-list "resources/text.txt"))

(positions (hash-set "is") x)

(defn x-positions
  "Currying the collection name out of the positions function"
  [word]
  (positions (hash-set word) x))

;Locations of the words
(def y (vec(map x-positions (distinct x))))

;words mapped to their locations
(zipmap x y)

(defn xnth
  "Curry x out of nth"
  [n]
  (nth x n))


(defn -main
  "Calls parse on a hard-coded file for now"
  [& args]
  (parse (read-to-list "resources/text.txt")))
