#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import time
import threading
import Queue
import argparse
import logging
import serial

#
# Data logger
# to be used in conjunction with the MAX30100_RawData example
#


logger = logging.getLogger(__name__)


class SerialStreamer(threading.Thread):
    DEFAULT_SPEED = 115200
    DEFAULT_HOLDOFF = 3
    DEFAULT_TIMEOUT = .1

    def __init__(self, port, holdoff=DEFAULT_HOLDOFF, speed=DEFAULT_SPEED):
        super(SerialStreamer, self).__init__()
        self._s = serial.Serial(port, speed, timeout=.5)
        self._q = Queue.Queue()
        self._holdoff = holdoff
        self._time_started = 0

    def run(self):
        logger.info('Starting streamer thread, delaying acquisition by %.2fs' % self._holdoff)
        self._isRunning = True

        tstarted = time.time()

        while time.time() - tstarted < self._holdoff and self._isRunning:
            self._s.readline()

        logger.info('Started acquisition')
        self._time_started = time.time()

        while self._isRunning:
            self._poll()

    def stop(self):
        logger.info('Stopping streamer thread')
        self._isRunning = False
        self.join()

    def get_samples(self):
        samples = []
        while not self._q.empty():
            samples.append(self._q.get())

        return samples

    def _poll(self):
        line = self._s.readline().strip()
        if line:
            spl = line.split('\t')

            if len(spl) != 2:
                logger.warning('Ignoring line: %s' % line)
            else:
                try:
                    a, b = [int(v) for v in spl]
                except Exception, e:
                    logger.exception(e)
                else:
                    self._q.put((self._get_time_offs(), a, b))

    def _get_time_offs(self):
        return int((time.time() - self._time_started) * 1000.0)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('port', help='Serial port of the device')
    parser.add_argument('outfile', help='Output recording file')
    parser.add_argument('--holdoff', type=float, default=SerialStreamer.DEFAULT_HOLDOFF,
                        help='Seconds to wait after connection before starting '
                             'acquisition')
    parser.add_argument('--debug', action='store_true', help='Be verbose')
    parser.add_argument('--samples', type=int, default=0,
                        help='Define the number of samples to be fetched, 0 to infinite')

    return parser.parse_args()

def gather(streamer, fp, maxsamples):
    samples_count = 0

    while True:
        samples = streamer.get_samples()
        samples_count += len(samples)

        for sample in samples:
            fp.write('%d\t%d\t%d\n' % sample)
        logger.info('Gathered %d/%d/%d samples' % (len(samples), samples_count, maxsamples))

        if samples_count > maxsamples:
            logging.info('Acquired %d samples (requested: %d), stopping' %
                         (samples_count, maxsamples))
            return

        time.sleep(1)

def run(port, outfile, holdoff, debug, samples):

    if debug:
        level = logging.DEBUG
    else:
        level = logging.INFO

    logging.basicConfig(level=level)

    streamer = SerialStreamer(port, holdoff)
    streamer.start()

    fp = open(outfile, 'w')

    fp.write('timestamp\tir_level\tred_level\n')

    try:
        gather(streamer, fp, samples)
    except KeyboardInterrupt:
        logger.info('Terminating')
    finally:
        fp.close()
        streamer.stop()

def cli_run():
    args = parse_args()

    run(args.port, args.outfile, args.holdoff, args.debug, args.samples)

if __name__ == '__main__':
    cli_run()
