# Session recorder

This tool pipes a raw session from the sensor into a file for further analysis.

## Requirements

The recorder and the analysis notebook require python and analysis libraries.
Virtualenv is warmly suggested.

* Flash the example firmware MAX30100_RawData to the microcontroller board
* Install the required libraries
    $ virtualenv max30100env
    $ source max30100env/bin/activate
    $ pip install -Ur requirements.txt
* Ensure the board is connected

## Record a session

Recording a session might be useful as a valuable debugging file when posting issues.
It can be done via command line interface:

    $ ./recorder.py --samples 1000 /dev/ttyACM0 test.out

the command above records at least 1000 samples of data from the serial device /dev/ttyACM0 to file test.out.

Each system might expose the microcontroller with different device paths.

## Run a beat analysis

Fire up a jupyter kernel and follow the instruction on the notebook:

    $ jupyter notebook beat_analysis.ipynb

## Use the provided sample data

A sample data file is provided to allow comparisons: test.out.sample
It can be opened from the provided notebooks by either copying it to test.out or by altering
the pd.read_csv() function exposes in each notebook.

The sample file shows an undisturbed 2000-samples worth of data that exposes an uncompensated
temperature drift.
