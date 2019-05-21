# Terminal Spaced Repetition App

## How to run:

Only tested on my Linux. Your milage may vary.

```
make
./main timetracking.csv
```

Use make to create the main executable. Then run it, passing the path of a csv file as an argument.
An example csv can be cound in the examples folder.

## Features

This app allows you to easily view where all your time has been going!

It accepts a csv file as input. The input csv file is in the format:

```
YYYY-MM-DD,HH:MM,Activity Name:Subactivity, Activity Location, Notes,
```

## Future Features:
 - Actually implement subactivities
 - Create a graphical output
 - add an activity to the CSV

## Coding Style
[Suckless Coding Style](https://suckless.org/coding_style/)


