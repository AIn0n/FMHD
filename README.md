# FMHD
Fast MinHash Distances algorithms collection

The project was described in the following B.Sc. thesis (in Polish): *Efektywne algorytmy aproksymacji podobieństwa dla sekwencji genomowych*.

## Getting started

1. Build a project.
2. Add all filenames or directories with data to compare to a single file, e.g. `data.txt`.
3. Sketch e.g. `./FMHD sketch -l data.txt -o sketches.bin`.
4. Compare e.g. `./FMHD ibf -l sketches.bin`

### build instructions 

Just run `build_FMHD.sh` file. You will get ready to use the executable `FMHD`.

## Configuration

Available flags:

- The **-c** argument determines how the algorithm processes data. For available commands see benchmarks, and the exact way they work is described in the thesis.
- The **-l** flag defines the name of the file that will be used as input for the algorithm. For the sketch command, this is a list of file names from which a sketch will be constructed. Other commands take pre-existing sets of sketches and return similarity results for each pair in the sketch. By default, results are returned as a triangle similarity matrix.
- Another option is **-k**, which allows you to specify the k-mer length into which the genome will be divided during sketching.
- The **-o** parameter is for the name of the output file storing the produced sketch
- Another parameter that can be configured is **-t**, indicating the number of CPU threads used for calculations.
- The **-E** flag allows to display of the data as an edge list, the default format is a triangular similarity matrix. Using this flag certainly slows down the algorithm's performance.

## Used libraries
- [CLI11](https://github.com/CLIUtils/CLI11)
- [fmt](https://github.com/fmtlib/fmt)
- [xxHash](https://github.com/stbrumme/xxhash)

## Benchmarks

The following charts are a time comparison for pairwise sketch distance computation. Tests were applied for M = 1024. The results are the median from five independent computations.

![benchmarks charts](https://github.com/AIn0n/FMHD/blob/master/benchmarks.jpg)

Other programs mentioned in the benchmarks:
- [Mash](https://github.com/marbl/Mash)
- [BinDash](https://github.com/zhaoxiaofei/bindash)
- [Dashing](https://github.com/dnbaker/dashing)

