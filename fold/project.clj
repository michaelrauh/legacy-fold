(defproject fold "0.1.0-SNAPSHOT"
  :description "An implementation of the fold algorithm in clojure"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.6.0"]]
  :main ^:skip-aot fold.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
