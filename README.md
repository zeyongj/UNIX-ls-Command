# UNIX ls Command

## Introduction
- SFU CMPT 300: Operating Systems I, Individual Project: The UNIX ls Command via C.
- This project is to implement a version of ls which supports only a limited set of options.
- The program UnixLs simulates the shell command ls.

## Execution
- Compile: `make`.
- To list current directory entries(just name): `./UnixLs`.

  Output based on csil: output_ls.txt.
- To list current directory entries with inode info: `./UnixLs -i`.

  Output based on csil: output_ls_i.txt.
- To list current directory entries with specific info(owner, group, file size, modification date...): `./UnixLs -l`.

  Output based on csil: output_ls_l.txt.
- To list current directory entries with subdirectory recursively: `./UnixLs -R`.

  Output based on csil: output_ls_R.txt.
- To list any directory: `./UnixLs /home`.

  Output based on csil: output_ls_home.txt, not included in the zip file.
- To clean: `make clean`.

## Test
- The program successfully run `./UnixLs`.

  Output based on csil: output_ls.txt.
- The program successfully run `./UnixLs -R ~/`.

  Output based on csil: output_ls_R_~.txt, not included in the zip file.
- The program successfully run `./UnixLs -l`.

  Output based on csil: output_ls_l.txt.
- The program successfully run `./UnixLs -Ri -l .. ~`.

  Output based on csil: output_ls_Ri_l.txt, not included in the zip file.
- The program successfully run `./UnixLs -liR . .. ~`.

## License

This work is licensed under [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0) (or any later version). 

`SPDX-License-Identifier: Apache-2.0-or-later`

## Disclaimer

**This repository is *ONLY* for backup. Students should *NEVER* use this repository to finish their works, *IN ANY WAY*.**

It is expected that within this course, the highest standards of academic integrity will be maintained, in
keeping with SFU’s Policy S10.01, `Code of Academic Integrity and Good Conduct`.

In this class, collaboration is encouraged for in-class exercises and the team components of the assignments, as well
as task preparation for group discussions. However, individual work should be completed by the person
who submits it. Any work that is independent work of the submitter should be clearly cited to make its
source clear. All referenced work in reports and presentations must be appropriately cited, to include
websites, as well as figures and graphs in presentations. If there are any questions whatsoever, feel free
to contact the course instructor about any possible grey areas.

Some examples of unacceptable behaviour:
- Handing in assignments/exercises that are not 100% your own work (in design, implementation,
wording, etc.), without a clear/visible citation of the source.
- Using another student's work as a template or reference for completing your own work.
- Using any unpermitted resources during an exam.
- Looking at, or attempting to look at, another student's answer during an exam.
- Submitting work that has been submitted before, for any course at any institution.

All instances of academic dishonesty will be dealt with severely and according to SFU policy. This means
that Student Services will be notified, and they will record the dishonesty in the student's file. Students
are strongly encouraged to review SFU’s Code of Academic Integrity and Good Conduct (S10.01) available
online at: http://www.sfu.ca/policies/gazette/student/s10-01.html.

## Author

Zeyong, JIN

August 23, 2021

