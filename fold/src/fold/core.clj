(ns fold.core
  (:gen-class))
(require '[clojure.string :as str])
(require '[clojure.math.combinatorics :as combo])

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
  (str/split (slurp filename) #"\W+"))

(def words (read-to-list "resources/text.txt"))

(defn words-nth
  "Curry words out of nth"
  [n]
  (nth words n))

(defn next-word
  [word]
  (distinct (map words-nth (map inc(positions (hash-set word ) words)))))

(def second-words (vec (map next-word (distinct (butlast words)))))

(zipmap words second-words)

(def roots (zipmap words second-words))

(defn leaves
  "Give all unique unordered cartesian products of mapped values given root"
  [word]
  (distinct (map sort (combo/cartesian-product (vec (roots word)) (vec (roots word))))))

(leaves "Here")

(defn -main
  "Currently printing data structure"
  [& args]
  (pr roots))
