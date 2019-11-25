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

## Testing Files

* _blank.csv_: CSV with nothing in it
* _cl-tweets-short-clean-20k.csv_: CSV with 20k lines
* _cl-tweets-short-clean.csv_: CSV from HW3 (Valid CSV)
* _doubleName.csv_:
* _headerOnly.csv_:
* _maxHeaderLength.csv_:
* _test.csv_:
* _tooManyCommas.csv_:
