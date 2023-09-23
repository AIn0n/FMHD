# FMHD
Fast MinHash Distances algorithms collection

The project was described in an engineering thesis titled: *Efektywne algorytmy aproksymacji podobieństwa dla sekwencji genomowych*.

# Used libraries

- [CLI11](https://github.com/CLIUtils/CLI11)
- [fmt](https://github.com/fmtlib/fmt)
- [xxHash](https://github.com/stbrumme/xxhash)

# build instructions 

Just run `build_FMHD.sh` file. You will get ready to use executable `FMHD`.

# Getting started

1. Build project.
2. Add all filenames or directories with data to compare to single file, e.g. `data.txt`.
3. Sketch e.g. `./FMHD sketch -l data.txt -o sketches.bin`.
4. Compare e.g. `./FMHD ibf -l sketches.bin`

# TODO

A few words about flags.