from threading import Thread, Event


class IntervalThread(Thread):
    def __init__(self, callback, interval: float, event):
        Thread.__init__(self)
        self.stopped = event
        self.interval = interval
        self.callback = callback

    def run(self):
        while not self.stopped.wait(self.interval):
            self.callback()


def setInterval(func, time):
    stopFlag = Event()

    thread = IntervalThread(func, time, stopFlag)
    thread.start()

    return stopFlag
