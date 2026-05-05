# This is addressing the comment made during the presentation

I got errors when I compiled the program without the `-std=c++11` flag because my C++ compiler defaults to an older standard that doesn't support features we used like `stoi`. I had to add `-std=c++11` to compile the program on my machine. My teammates didn't have this issue. 