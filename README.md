# minisana
Miniature Simulated Annealing Network Aligner

![](https://img.shields.io/badge/style-24%20kb-green.svg?style=plastic&label=binary%20size)
![](https://img.shields.io/badge/style-dope%20as%20shit-green.svg?style=plastic&label=cool%20level)

<img src="http://i.imgur.com/tSLzyQg.png" width="550px">

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

## Planning

Check out our [kanban board](https://github.com/jamesalbert/minisana/projects/1)
