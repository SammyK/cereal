# Cereal - arena-allocated PHP objects

A PoC extension to benchmark arena-allocated objects. This can be handy when you need many hundreds or even thousands of objects to persist in memory throughout the duration of a request.

## Installation

```bash
$ phpize \
    && ./configure \
    && make
$ php -dextension=$(pwd)/modules/cereal.so --ri=cereal
```

## Usage

When `cereal.use_arena=1`, all `Cereal` objects will be arena-allocated. See [the tests](tests/) for full examples.

## Benchmarking

Quick and dirty benchmarking done with [hyperfine](https://github.com/sharkdp/hyperfine).

```bash
$ hyperfine \
  "php -dextension=$(pwd)/modules/cereal.so -dcereal.use_arena=0 benchmark/benchmark.php" \
  "php -dextension=$(pwd)/modules/cereal.so -dcereal.use_arena=1 benchmark/benchmark.php"
```

The results aren't mind-blowing, but using an arena allocator does appear to be a bit faster.

```
Benchmark #1: php -dextension=/home/sammyk/Code/cereal/modules/cereal.so -dcereal.use_arena=0 benchmark/benchmark.php
  Time (mean ± σ):      16.3 ms ±   1.2 ms    [User: 12.2 ms, System: 4.1 ms]
  Range (min … max):    10.3 ms …  18.9 ms    171 runs
 
Benchmark #2: php -dextension=/home/sammyk/Code/cereal/modules/cereal.so -dcereal.use_arena=1 benchmark/benchmark.php
  Time (mean ± σ):      15.9 ms ±   1.8 ms    [User: 11.5 ms, System: 4.3 ms]
  Range (min … max):     9.9 ms …  18.6 ms    225 runs
 
Summary
  'php -dextension=/home/sammyk/Code/cereal/modules/cereal.so -dcereal.use_arena=1 benchmark/benchmark.php' ran
    1.02 ± 0.14 times faster than 'php -dextension=/home/sammyk/Code/cereal/modules/cereal.so -dcereal.use_arena=0 benchmark/benchmark.php'
```
