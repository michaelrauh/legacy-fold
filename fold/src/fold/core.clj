(ns fold.core
  (:gen-class))
(require '[clojure.string :as str])

(defn read-to-list
  "Take in filename and return list of words"
  [filename]
  (str/split (slurp filename) #" "))

(defn parse
  "Take a list of words and return a foldable object"
  [words]
  (pr words)
  )



(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (parse (read-to-list "resources/text.txt")))
