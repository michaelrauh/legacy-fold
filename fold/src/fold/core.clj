(ns fold.core
  (:gen-class))
(require '[clojure.string :as str])

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
 (pr(str/split (slurp "resources/text.txt") #" "))) 
