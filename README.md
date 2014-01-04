fractals
========

One of the first programs I ever wrote, with two friends in 2002.

I'm porting this to libpng++ so that one of the co-authors can run it.
I'm told the old code generates bitmaps with bad headers (at least
when run in wine).

See also: comically ugly web page.

Genealogy
-----

There were four versions of the code in my backups.  I couldn't
determine the exact relationship between them, but here are my notes.

Call the main files

1. FinalWithGroup/julia.cpp

2. RigoNathanColor/julia.cpp

3. FINAL/FINALjulia.cpp

4. web-page/bin/juliaF.cpp


Then from we learn from the diffs:

- diff -u FINAL/FINALjulia.cpp web-page/bin/juliaF.cpp

  Conclude 3 < 4.

  The one in the web page is newer than FINAL, but a mostly minor
  modification. It adds generation of file-name based on params.

- diff -u FinalWithGroup/julia.cpp FINAL/FINALjulia.cpp

  Conclude 1 < 3.

  Significant differences.

- diff -u FinalWithGroup/julia.cpp RigoNathanColor/julia.cpp

  Conclude 1 and 2 have a common ancestor, possibly 1 itself.

  Introduces input processing where the user chooses the function to
  iterate.

  Removes nrdRGB stuff, which then gets readded in (3), indicating
  it's not in line with 1, 3, and 4.

  This is clearly derived from some common ancestor of 1, but I'm not
  if that ancestor is 1 itself, or strict ancestor of 1.

  257 lines of diff output.

- diff -u RigoNathanColor/julia.cpp FINAL/FINALjulia.cpp

  Conclude 2 is closer to 1 than to 3.

  339 lines of diff output.

- diff -u RigoNathanColor/julia.cpp web-page/bin/juliaF.cpp

  Conclude 2 is closer to 1 than to 4.

  385 lines of diff output.

In summary:

 3 < 4
 2 < 3
 1 < 3
 1 ? 2

So, either

   2
  /
 1 - 3 - 4

or

   2
  /
 M
  \
   1 - 3 - 4

for some missing link 'M'.
