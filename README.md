# minisana
Miniature Simulated Annealing Network Aligner

![](https://img.shields.io/badge/style-24%20kb-green.svg?style=plastic&label=binary%20size)
![](https://img.shields.io/badge/style-dope%20as%20shit-green.svg?style=plastic&label=dope%20meter)

<img src="http://i.imgur.com/tSLzyQg.png" width="550px">

## Install

Running `make` will do a few things:

  1. compile object files in `_objs`
  2. build the binary `bin/mini`
  3. build the shared library `lib/libmini.so`
  4. create empty directories `output` where .json files will be stored

## Tests

Run tests with `make test`. Along with showing the test results, a coverage report will be generated as coverage/report.html. Open the report in your browser to see the total code coverage. `make docker` will also run the tests during image creation.

## Docker

You can build a minisana image with `make docker` or go straight to running it with:

```sh
MINIOPTS="-n input/yeast.gw -N input/human.gw -t 10000000" make docker_run
```

Then you can check the logs with `docker logs -f <container-id>`. Once it's finished, you can check the json file written to `~/output`.

## Usage

```sh
./bin/mini -n,-N [-s,-a,-o,-t]

  -n: path to the smaller network
  -N: path to the bigger network (small n, big N... get it?)
  -s: path to sequence file [defaults to "input/yeast_human.bitscores"]
  -a: alpha value [defaults to 0.5]
  -o: file to output json [defaults to ./mini.out.json]
  -t: amount of time to run simulation [defaults to 100,000,000 iterations]
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
