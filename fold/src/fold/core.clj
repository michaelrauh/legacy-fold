(ns fold.core
  (:gen-class))

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (pr (slurp "src/fold/text.txt")))
