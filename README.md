# ECS 160 - Fuzz Testing Assignment

Professor Devanbu

<details>
    <summary>Teaching Assistants</summary>
        <p>Ji Wang (Lead TA), Casey Casalnuovo</p>
</details>

UC Davis - Fall 2019

---

## Contributors

* [Yiping (Allison) Su](mailto:ypsu@ucdavis.edu)
* [Joanne Chang](mailto:joachang@ucdavis.edu)

---

## :email: To Our Testing Team

Please download the `zip` version of this repo because it will automatically remove unneeded development
and informative files for you.

We have provided a AFL-Compatible file structure, `aflCompat`, with a `Makefile` that will help make running commands/tests easier. After you download and extract the `zip` file, you can use `docker cp` to move `aflCompat` to your Docker Container.

1. **Leave your example csv test files in `aflCompat/in` -- this is where AFL will look for file parameters.**
2. **`make run` will tell the AFL Fuzz Tester to generate crash files and statistics to `aflCompat/out`.**

The `Makefile` has the following commands...

| Command         | What It Does                                                 |
|:----------------|:-------------------------------------------------------------|
| `make`          | Compiles the program for you -- Tweeter.exe                  |
| `make clean`    | Removes all object and executable files                      |
| `make run`      | Runs the AFL Fuzz Tester                                     |

Thanks!

---

## Assignment Details & Usage

The goal of this assignment is to create a program which counts the top 10 tweeters processed from a given tweet file and doesn't crash when given invalid inputs.

### Repo Details

* All of our testing files are located in `/tests`.
* Our program is documented according to `Doxygen` guidelines. Some of the annotations can be found [here](https://www.cs.cmu.edu/~410/doc/doxygen.html).

**The regular source code and Makefile (makefile -- not AFL-Compatible) are located in the main directory of the zip download. To use the AFL-compatible version, read the instructions [here](#email-to-our-testing-team).**

**To Compile:** `make`

_`make clean` will remove all maxTweeter related objects and executables from your directory._

**To Run:** `./maxTweeter.exe csvFile`

**Output:**

```code
<Tweeter>: <Count of Tweets>
Bob: 67
Jane: 56
```

For a more in-depth explanation of the assignment, check out the [pdf](Homework4Part1.pdf).

---

## Our Algorithm Implementation

Our algorithm uses a _doubly-linked list_. Every Tweeter is stored as a struct which contains a **name** and **count** field. Our other two structs define the linked list implementation. One defines the linked list node, and the other is a struct which holds the pointer to the _HEAD_ and _TAIL_ of our list.

The algorithm uses an in-place insertion sort to order the Tweeters by count. The entire program should take about O(n) because of the file scan. 

When the algorithm encounters a valid tweeter, it does the following:

* If the name is valid, check if the Tweeter is in our database
* If we found the Tweeter, increment its count and move the node left until whatever is on the left is the same or greater count
* If we haven't found the tweeter, create a new node and add it to the end of the list

While using an insertion sort isn't as efficient as using a logarithmic sort, sorting in place allows us to create a natural priority queue. Whenever we hit a user who has already been recorded as a frequent tweeter, it takes much less time to find him/her.

---

## :clipboard: Testing Files

| Filename                          | Description                                                       |
|:----------------------------------|:------------------------------------------------------------------|
| blank.csv                         | Blank/Empty file                                                  |
| cl-tweets-short-clean-20k.csv     | CSV with 20k lines                                                |
| cl-tweets-short-clean.csv         | CSV from HW3 -- a valid CSV                                       |
| doubleName.csv                    | CSV with more than one **name** field declared in the **header**  |
| headerOnly.csv                    | A CSV with only the **header** provided                           |
| maxHeaderLength.csv               | CSV with a **header** greater than max character count            |
| tooManyCommas.csv                 | CSV whose lines have too many commas -- not a valid CSV line      |
| nonAscii.csv                      | CSV with characters not from the ASCII set                        |
| noName.csv                        | File with no **name** field in the **header** column              |
| lastLine.csv                      | Check if last line is counted                                     |
| quotes.csv                        | Testing if quoted **names** are handled                           |

---

## :whale: Docker

[**Docker Commandline Documentation Link**](https://docs.docker.com/engine/reference/commandline/docker/)

Our fuzz testing is done using AFL -- [American Fuzzy Lop](http://lcamtuf.coredump.cx/afl/). The Professor and TA have provided us a docker image which already has AFL set up.

You can access the image using: `docker pull caseycas/afl-docker`.

### Some Important Docker Commands

#### `cp` -- [Copy](https://docs.docker.com/engine/reference/commandline/cp/)

If you're using `cp` to copy files from a Docker container to local machine, or vice versa, you should have two terminals open.

One terminal should be running a Docker Container, and the other, a terminal to your local filesystem.

|                         | Docker Copy Command                                                          |
| ----------------------- | -----------------------------------------------------------------------------|
| Filesystem to Container | `docker cp localFileLocation dockerContainerName:locationOnDockerFileSystem` |
| Container to Filesystem | `docker cp dockerContainerName:locationOnDockerFileSystem localFileLocation` |

**If you're copying from your filesystem to the Container, you should run the Filesystem to Container command on the terminal running the Container. Run the Container to Filesystem command in the terminal directly accessing your local filesystem.**

#### `ps` -- [Currently Running Docker Containers](https://docs.docker.com/engine/reference/commandline/ps/)

`docker ps` will list all of the currently running docker containers in your terminal. This command is helpful to check if you have a Container working/running or not.

**Running `docker ps -a` will show all Docker containers, including ones that aren't running on your local machine.**

#### Docker Image, Container, & Cleaning

List all of your "installed" Images using `docker image`. The command to list all of your docker containers (including inactive ones) is explained above: `docker ps -a`.

To clean all of your docker containers, use `docker rm $(docker ps -a -f status=exited -q)` where `-f` represents filter, and `-q` represents the docker container ID.

You can also run an Image (create a container instance) and remove it upon exiting using `docker run --rm image_name`.

For more Docker commands, visit [here](https://www.digitalocean.com/community/tutorials/how-to-remove-docker-images-containers-and-volumes).

### Docker Images & Containers

The `afl-docker` given to us for HW4 is a Docker Image; Docker Images are not the same as Docker Containers.

A Docker Image is a snapshot of container. It describes the environment to run the container.

A Container is a running _instance_ of an Image. They're portable encapsulations of an environment which run the applications provided.

**If you use the command, `docker ps -a`, and you see no Docker container existing, you must run the Docker container first using:**

`docker run --security-opt seccomp=unconfined -it caseycas/afl-docker`

_`--security-opt seccomp=unconfined` is needed if you want to run gdb in docker._

To run an existing docker container, use the command, `docker start -a -i [containerName]`, where `-a` represents attach, and `-i` represents interactive shell.

The [docker documentation page for `start`](https://docs.docker.com/engine/reference/commandline/start/) has more explanations on different types of flags and usage types.

More information about Docker Images, Containers, and their differences can be found [here](https://stackoverflow.com/questions/23735149/what-is-the-difference-between-a-docker-image-and-a-container).

---

## :rabbit: AFL -- [American Fuzzy Lop](http://lcamtuf.coredump.cx/afl/)

**If you're using our provided `aflCompat` to run our program, the `makefile` included within the folder automatically compiles and runs the program for you. [See here](#email-to-our-testing-team).**

To compile the program using AFL, use the command: `afl-clang -g -o csv_read csv_sample.c`

`afl-clang` is a special compiler which allows the AFL program to record statistics, etc.

Run the program with: `afl-fuzz -i afl/in -o afl/out -- ./csv_read @@`

`@@` signifies that you want to treat files in the `in` directory as parameters to your program.

* `-i` : input directory
* `-o` : output directory

More information about AFL Docker can be found [here](AFL-Docker-Guide.pdf).
