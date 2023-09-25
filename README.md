# What is this?
Just a repo I store solutions to problems I thought were interesting.

Maybe I saw these during an interview, interview prep, or while building some personal project. Keeping the problems and solutions here mainly as a study aide.

I have tried to keep each of the implementations time-boxed to approximate an interview question. After the initial implementation, only cleaning-up and adding documentaiton. To that end, the solutions may not be optimal or even very good. They should all build and pass their unit tests, though. Code here comes with no other waranty of fitness.

# Building
This repo includes a cmake definition which you may use to build the test binaries. It has been tested on Windows, linux, and MacOS.

# Running
There are a couple of test binaries which are defined in this repo.

```console
> git clone https://github.com/boingoing/samples/samples.git
> mkdir samples/build
> cd samples/build
> cmake ..
> make
> ./runner
> ./simple_optimizer
```
