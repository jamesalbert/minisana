# minisana
mini Simulated Annealing Network Aligner

![](http://i.imgur.com/1p6bqAj.png)

## Usage

```sh
./bin/mini -n,-N [-s,-a]

  -n: path to the smaller network
  -N: path to the bigger network (small n, big N... get it?)
  -s: path to sequence file [defaults to "input/yeast_human.bitscores"]
  -a: alpha value [defaults to 0.5]
```

## Example

```sh
make
./bin/mini -n input/yeast.gw -N input/human.gw
# or
./bin/mini -n input/yeast.gw -N input/human.gw -s input/yeast_human.bitscores
# or
./bin/mini -n input/yeast.gw -N input/human.gw -s input/yeast_human.bitscores -a 0.995
# or
./bin/mini -n input/yeast.gw -N input/human.gw -a 0.995
```
