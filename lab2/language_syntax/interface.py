class Interval():
    def __init__(self, start: float, end: float):
        self.start = start
        self.end = end


class File():
    def __init__(self, path: str):
        self.path = path

    def get_interval(self, start: float | str, end: float | str) -> Interval:
        return Interval(start, end)

    def write_interval(self, interval: Interval): pass


def open_wav(path: str) -> File: pass
def create_wav(path: str) -> File: pass
def exists(path: str) -> bool: pass
def time(time_str: str) -> float: pass