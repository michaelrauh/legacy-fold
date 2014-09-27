(ns fold.core
  (:gen-class))
(require '[clojure.string :as str])
(require '[clojure.math.combinatorics :as combo])

(combo/combinations [1 2 3] 2)

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

(def words (read-to-list "resources/text.txt"))

(defn words-nth
  "Curry words out of nth"
  [n]
  (nth words n))

(defn words-positions
  "Curry the collection name out of the positions function"
  [word]
  (positions (hash-set word) words))

(defn next-word
  [word]
  (distinct (map words-nth (map inc(positions (hash-set word ) words)))))

(def second-words(vec (map next-word (butlast (distinct words)))))

(zipmap words second-words)

(def roots (zipmap words second-words))

(defn -main
  "Currently printing data structure"
  [& args]
  (pr roots))
