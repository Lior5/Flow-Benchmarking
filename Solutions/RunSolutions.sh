#!/bin/env -S bash
ulimit -s unlimited
shopt -s inherit_errexit
rm -f outputs/*
timeCSV="time_results.csv"
problemStatsCSV="problem_stats.csv"
echo "Problem, Dinics, Ford Fulkerson, Push Relabel, Tidal Flow" > $timeCSV
echo "Problem, Max nodes, Max edges, Density, Acyclic?, Unit?" > $problemStatsCSV

#problems: airports containment guards hezardastan openpit piece science steeplechase traffic
#algorithms: DINICS FORD_FULKERSON PUSH_RELABEL TIDAL_FLOW
for problem in airports alphabet containment steeplechase traffic guards hezardastan openpit piece science; do
  echo $problem
  max_nodes=1
  max_edges=1
  acyclic=0
  unit=0
  if [[ $problem == "guards" || $problem == "hezardastan" || $problem == "piece"
       || $problem == "steeplechase" || $problem == "traffic" || $problem == "openpit" 
       || $problem == "alphabet" ]]; then
      acyclic=1
  fi
  if [[ $problem == "airports" || $problem == "steeplechase" || $problem == "alphabet" 
       || $problem == "science" || $problem == "hezardastan" ]]; then
      unit=1
  fi
  echo -n "$problem, " >> $timeCSV
  for algo in DINICS FORD_FULKERSON PUSH_RELABEL TIDAL_FLOW; do
    g++ -O2 -DUSE_$algo -o code $problem.cpp
    echo $algo:
    mkdir -p outputs
    bleh=0
    function runCases(){
      for dir in ../Data/$problem/*/; do
          if [ -d "$dir" ]; then
          # Iterate over each .in file in the directory
          for input_file in "$dir"*.in; do
              # ((bleh++))
              # echo "starting procedure"
              if [ -f "$input_file" ]; then
                # echo "enter step 1"
                output=$(timeout 5s ./code < "$input_file" | tee -a "outputs/${problem}_${algo}.out")

                # echo "finish step 1"
                # ./code < "$input_file"
              # echo "finished running"
              while read -r line; do
                  node_count=$(echo "$line" | awk '{print $2}')
                  if [[ $algo == "DINICS" && $node_count -gt $max_nodes ]]; then
                      max_nodes=$node_count
                      # echo "Debug: Updated max_nodes = $max_nodes"
                  fi
              done < <(echo "$output" | grep "Nodes:")
              while read -r line; do
                  edge_count=$(echo "$line" | awk '{print $2}')
                  if [[ $algo == "DINICS" && $edge_count -gt $max_edges ]]; then
                      max_edges=$edge_count
                      # echo "Debug: Updated max_edges = $max_edges"
                  fi
              done < <(echo "$output" | grep "Edges:")
              # echo "Final max_nodes = $max_nodes, max_edges = $max_edges"
              fi
          done
          fi
      done
      export max_nodes
      export max_edges
    }
    export problem
    export algo
    export -f runCases
    runCases
    # timeout 5 bash -c 'runCases'
    # timeout 30s 'runCases'
    
    { time ( timeout 150s bash -c 'runCases' ) ; } 2>&1 | sed -zre 's/\nreal\t(.*)\nuser\t(.*)\nsys\t(.*)/\1/g' >>$timeCSV
    if [ "$algo" != "TIDAL_FLOW" ]; then
        echo -n "," >> $timeCSV
    fi
    echo -n " " >> $timeCSV 
  done
  density=`echo "scale = 2; $max_edges / $max_nodes" | bc`
  echo "$problem, $max_nodes, $max_edges, $density, $acyclic, $unit " >> $problemStatsCSV
  echo "" >> $timeCSV
done