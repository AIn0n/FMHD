# FMHD
Fast MinHash Distances algorithms collection

The project was described in an engineering thesis titled: *Efektywne algorytmy aproksymacji podobieństwa dla sekwencji genomowych*.

## Getting started

1. Build a project.
2. Add all filenames or directories with data to compare to a single file, e.g. `data.txt`.
3. Sketch e.g. `./FMHD sketch -l data.txt -o sketches.bin`.
4. Compare e.g. `./FMHD ibf -l sketches.bin`

### build instructions 

Just run `build_FMHD.sh` file. You will get ready to use the executable `FMHD`.

## Configuration

Available flags:

- The **-c** argument determines how the algorithm processes data. For available commands see benchmarks, and the exact way they work will be described later in this work.
- The **-l** flag defines the name of the file that will be used as input for the algorithm. For the sketch command, this is a list of file names from which a sketch will be constructed. Other options take pre-existing sets of sketches and return similarity results for each pair in the sketch.
- Another option is **-k**, which allows you to specify the size of k-mers into which the genome will be divided during sketching.
- The **-o** flag is responsible for naming the sketch obtained as the program's output.
- Another parameter that can be configured is **-t**, indicating the number of CPU threads used for calculations.
- The **-E** flag allows you to display the data as an edge list, the default format is a triangular similarity matrix. Just to let you know, using this flag certainly slows down the algorithm's performance.

## Used libraries
- [CLI11](https://github.com/CLIUtils/CLI11)
- [fmt](https://github.com/fmtlib/fmt)
- [xxHash](https://github.com/stbrumme/xxhash)

## Benchmarks

![benchmarks charts](https://github.com/AIn0n/FMHD/blob/master/benchmarks.jpg)
