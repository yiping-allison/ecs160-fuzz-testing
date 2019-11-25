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

Thanks!

---

## Assignment Details & Usage

The goal of this assignment is to create a program which counts the top 10 tweeters processed from a given tweet file.

The program should not crash even when given invalid inputs.

All of our testing files are located in `/tests`.

Our program is documented according to `Doxygen` guidelines. Some of the annotations can be found [here](https://www.cs.cmu.edu/~410/doc/doxygen.html).

**Usage:** `./maxTweeter.exe tests/filename.csv`

**Output:**

```code
<Tweeter>: <Count of Tweets>
Bob: 67
Jane: 56
```

For a more in-depth explanation of the assignment, check out the [pdf](Homework4Part1.pdf).

---

## Our Algorithm Implementation

TBD

---

## :clipboard: Testing Files

* _blank.csv_ - Blank file
* _cl-tweets-short-clean-20k.csv_ - CSV with 20k lines
* _cl-tweets-short-clean.csv_ - CSV from HW3 (Valid CSV)
* _doubleName.csv_ - CSV with more than one NAME field declared in the HEADER
* _headerOnly.csv_ - A CSV with only the HEADER provided
* _maxHeaderLength.csv_ - CSV with a HEADER greater than max character count
* _test.csv_ - CSV which contains custom tests during development
* _tooManyCommas.csv_ - CSV whose lines has too many commas (Doens't define valid Tweeter line)

---

## :whale: Docker

Our fuzz testing is done using AFL -- [American Fuzzy Lop](http://lcamtuf.coredump.cx/afl/). The Professor and TA have provided us a docker container which already has AFL set up.

You can access this container using: `docker pull caseycas/afl-docker`.

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

### Docker Images & Containers

The `afl-docker` given to us for HW4 is a Docker Image. Docker Images are not the same as a Docker Container.

A Docker Image is a snapshot of container. It's built on layers which describe the environment to run the container.
Therefore, a docker image is immutable.

A Container is a running _instance_ of an Image. They're portable encapsulations of an environment which run the applications provided.

**If you run `docker ps`, and you see no Docker container running, you must run the Docker container first using:**

`docker run --security-opt seccomp=unconfined -it caseycas/afl-docker`

_`--security-opt seccomp=unconfined` is needed if you want to run gdb in docker._

More information about Docker Images and Containers can be found [here](https://stackoverflow.com/questions/23735149/what-is-the-difference-between-a-docker-image-and-a-container).
